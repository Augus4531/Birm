
#include "birm_arm_float_User.h"
#include <arm_neon.h>

// 核心优化函数：处理 4x4 复数块的转置
// 利用 NEON 寄存器进行数据交换，减少内存跨步访问开销
static inline void transpose_4x4_complex_neon(const float *src, float *dest, int src_stride_elements, int dest_stride_elements)
{
    // src_stride_elements: 源矩阵一行的 float 元素个数 (mm * 2)
    // dest_stride_elements: 目标矩阵一行的 float 元素个数 (sa * 2)

    // 加载 4 行数据，每行 4 个复数（8个 float）
    // 每个 float32x4_t 保存 2 个复数
    float32x4_t r0_0 = vld1q_f32(src);
    float32x4_t r0_1 = vld1q_f32(src + 4);
    src += src_stride_elements;

    float32x4_t r1_0 = vld1q_f32(src);
    float32x4_t r1_1 = vld1q_f32(src + 4);
    src += src_stride_elements;

    float32x4_t r2_0 = vld1q_f32(src);
    float32x4_t r2_1 = vld1q_f32(src + 4);
    src += src_stride_elements;

    float32x4_t r3_0 = vld1q_f32(src);
    float32x4_t r3_1 = vld1q_f32(src + 4);

    // ---------------------------------------------------------
    // 转置逻辑：将 float32x4 视为 uint64x2 (每个 uint64 是一个复数)
    // 这样可以保持实部和虚部在一起移动，无需拆分
    // ---------------------------------------------------------
    uint64x2_t v0a = vreinterpretq_u64_f32(r0_0); // Row 0: C0, C1
    uint64x2_t v0b = vreinterpretq_u64_f32(r0_1); // Row 0: C2, C3
    uint64x2_t v1a = vreinterpretq_u64_f32(r1_0); // Row 1: C0, C1
    uint64x2_t v1b = vreinterpretq_u64_f32(r1_1); // ...
    uint64x2_t v2a = vreinterpretq_u64_f32(r2_0);
    uint64x2_t v2b = vreinterpretq_u64_f32(r2_1);
    uint64x2_t v3a = vreinterpretq_u64_f32(r3_0);
    uint64x2_t v3b = vreinterpretq_u64_f32(r3_1);

    // 第一级交换 (2x2 block 内部交换)
    // trn1 取两个向量的低64位，trn2 取高64位
    // 结果：t0 = {R0C0, R1C0}, t1 = {R0C1, R1C1}
    uint64x2_t t0 = vtrn1q_u64(v0a, v1a);
    uint64x2_t t1 = vtrn2q_u64(v0a, v1a);
    uint64x2_t t2 = vtrn1q_u64(v2a, v3a);
    uint64x2_t t3 = vtrn2q_u64(v2a, v3a);

    uint64x2_t t4 = vtrn1q_u64(v0b, v1b);
    uint64x2_t t5 = vtrn2q_u64(v0b, v1b);
    uint64x2_t t6 = vtrn1q_u64(v2b, v3b);
    uint64x2_t t7 = vtrn2q_u64(v2b, v3b);

    // 第二级交换并写入 (组合成 4x1 的列)
    // Dest Row 0 应该是原矩阵的 Col 0: {R0C0, R1C0, R2C0, R3C0}
    // t0 包含前两个，t2 包含后两个

    // 写入 Dest Row 0 (Col 0)
    vst1q_f32(dest, vreinterpretq_f32_u64(t0));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t2));
    dest += dest_stride_elements;

    // 写入 Dest Row 1 (Col 1)
    vst1q_f32(dest, vreinterpretq_f32_u64(t1));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t3));
    dest += dest_stride_elements;

    // 写入 Dest Row 2 (Col 2)
    vst1q_f32(dest, vreinterpretq_f32_u64(t4));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t6));
    dest += dest_stride_elements;

    // 写入 Dest Row 3 (Col 3)
    vst1q_f32(dest, vreinterpretq_f32_u64(t5));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t7));
}

// 通用处理函数，增加了对小矩阵的特化处理
int birm_cmtrans_sf(const float *x, const int nn, const int mm,
                    const int sa, const int sb, float *y)
{
    if (x == NULL || y == NULL)
        return birmParamNullError;
    // sa是输出行数(原列数)，sb是输出列数(原行数)
    if (nn <= 0 || mm <= 0 || sa <= 0 || sb <= 0 || sb > nn || sa > mm)
        return birmParamLengthInvalidError;

    // 针对 32x32, 64x64, 128x128 等中小尺寸矩阵的优化路径
    // 使用 16x16 的分块策略（Block Tiling）来适配 L1 Cache
    const int BLOCK_SIZE = 16;

    for (int i = 0; i < sb; i += BLOCK_SIZE)
    {
        for (int j = 0; j < sa; j += BLOCK_SIZE)
        {

            // 处理边界情况
            int i_limit = (i + BLOCK_SIZE < sb) ? i + BLOCK_SIZE : sb;
            int j_limit = (j + BLOCK_SIZE < sa) ? j + BLOCK_SIZE : sa;

            int ii = i;
            // 4x4 主循环：一次处理 4 行 4 列
            for (; ii <= i_limit - 4; ii += 4)
            {
                int jj = j;
                for (; jj <= j_limit - 4; jj += 4)
                {
                    // 计算源地址和目标地址
                    // 源矩阵是 mm 列 (每行 mm 个复数 = 2*mm float)
                    // 目标矩阵是 sa 列 (每行 sa 个复数 = 2*sa float)
                    const float *src_ptr = x + (ii * mm + jj) * 2;
                    float *dest_ptr = y + (jj * sa + ii) * 2; // 注意转置后的索引计算

                    // 调用 NEON 优化核
                    transpose_4x4_complex_neon(src_ptr, dest_ptr, mm * 2, sa * 2);
                }
                // 处理列剩余 (不足4列的部分)
                for (; jj < j_limit; ++jj)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        const float *s = x + ((ii + k) * mm + jj) * 2;
                        float *d = y + (jj * sa + (ii + k)) * 2;
                        // 复制一个复数
                        d[0] = s[0];
                        d[1] = s[1];
                    }
                }
            }

            // 处理行剩余 (不足4行的部分)
            for (; ii < i_limit; ++ii)
            {
                for (int jj = j; jj < j_limit; ++jj)
                {
                    const float *s = x + (ii * mm + jj) * 2;
                    float *d = y + (jj * sa + ii) * 2;
                    d[0] = s[0];
                    d[1] = s[1];
                }
            }
        }
    }

    return birmSuccess;
}
