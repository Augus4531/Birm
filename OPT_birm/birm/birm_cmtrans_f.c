#include <arm_neon.h>
#include <stddef.h>

// 辅助函数：处理 4x4 复数块的转置
// 从 src 读取 4行4列，转置后写入 dest (dest stride 为 nn)
static inline void transpose_4x4_complex(const float *src, float *dest, int src_stride, int dest_stride) {
    // 这是一个 4x4 的复数矩阵。
    // 每个复数看作 64位 (2个float)。
    // 我们需要加载 4 行，每行 4 个复数。
    // 一行 4 个复数 = 8 个 float = 2 个 Q 寄存器 (128bit)。
    
    // 加载数据: Row 0
    float32x4_t r0a = vld1q_f32(src);
    float32x4_t r0b = vld1q_f32(src + 4);
    src += src_stride;
    // Row 1
    float32x4_t r1a = vld1q_f32(src);
    float32x4_t r1b = vld1q_f32(src + 4);
    src += src_stride;
    // Row 2
    float32x4_t r2a = vld1q_f32(src);
    float32x4_t r2b = vld1q_f32(src + 4);
    src += src_stride;
    // Row 3
    float32x4_t r3a = vld1q_f32(src);
    float32x4_t r3b = vld1q_f32(src + 4);

    // 转置逻辑：将 float32x4 视为 uint64x2 (每个元素是一个复数)
    uint64x2_t v0a = vreinterpretq_u64_f32(r0a); // Row 0, C0, C1
    uint64x2_t v0b = vreinterpretq_u64_f32(r0b); // Row 0, C2, C3
    uint64x2_t v1a = vreinterpretq_u64_f32(r1a); // Row 1, C0, C1
    uint64x2_t v1b = vreinterpretq_u64_f32(r1b); // ...
    uint64x2_t v2a = vreinterpretq_u64_f32(r2a);
    uint64x2_t v2b = vreinterpretq_u64_f32(r2b);
    uint64x2_t v3a = vreinterpretq_u64_f32(r3a);
    uint64x2_t v3b = vreinterpretq_u64_f32(r3b);

    // 第一次交换 (2x2 block 内部交换)
    // T0 = {R0C0, R1C0}, T1 = {R0C1, R1C1}
    uint64x2_t t0 = vtrn1q_u64(v0a, v1a); 
    uint64x2_t t1 = vtrn2q_u64(v0a, v1a);
    uint64x2_t t2 = vtrn1q_u64(v2a, v3a); 
    uint64x2_t t3 = vtrn2q_u64(v2a, v3a);
    
    uint64x2_t t4 = vtrn1q_u64(v0b, v1b);
    uint64x2_t t5 = vtrn2q_u64(v0b, v1b);
    uint64x2_t t6 = vtrn1q_u64(v2b, v3b);
    uint64x2_t t7 = vtrn2q_u64(v2b, v3b);

    // 此时我们有了局部列，但需要组合成完整列
    // t0: R0C0, R1C0;  t2: R2C0, R3C0
    // 写入 Dest 的 Row 0 (即原矩阵的 Col 0): R0C0, R1C0, R2C0, R3C0
    
    // 写入 Dest Row 0 (Col 0)
    vst1q_f32(dest, vreinterpretq_f32_u64(t0));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t2));
    dest += dest_stride;

    // 写入 Dest Row 1 (Col 1)
    vst1q_f32(dest, vreinterpretq_f32_u64(t1));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t3));
    dest += dest_stride;

    // 写入 Dest Row 2 (Col 2)
    vst1q_f32(dest, vreinterpretq_f32_u64(t4));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t6));
    dest += dest_stride;

    // 写入 Dest Row 3 (Col 3)
    vst1q_f32(dest, vreinterpretq_f32_u64(t5));
    vst1q_f32(dest + 4, vreinterpretq_f32_u64(t7));
}

int birm_cmtrans_f(const float *x, int nn, int mm, float *y)
{
    if (x == NULL || y == NULL) return -1;
    if (nn <= 0 || mm <= 0) return -2;

    const int complex_stride = 2; // float numbers per complex

    // 选择 Block Size。对于 32x32 和 64x64，16 或 32 都是不错的选择。
    // 为了通用性且适配 L1 Cache，选择 16 或 32。这里选 32。
    const int BLK = 32; 

    for (int i = 0; i < nn; i += BLK) {
        for (int j = 0; j < mm; j += BLK) {
            
            // 处理块内的边界
            int i_lim = (i + BLK < nn) ? i + BLK : nn;
            int j_lim = (j + BLK < mm) ? j + BLK : mm;

            int ii = i;
            // 4x4 循环展开主逻辑
            for (; ii <= i_lim - 4; ii += 4) {
                int jj = j;
                for (; jj <= j_lim - 4; jj += 4) {
                    // 计算当前 4x4 块的指针
                    const float *src_ptr = x + (ii * mm + jj) * complex_stride;
                    float *dest_ptr = y + (jj * nn + ii) * complex_stride;
                    
                    // stride 是元素的个数（float数），不是字节
                    // src一行跳过 mm 个复数 (mm * 2 floats)
                    // dest一行跳过 nn 个复数 (nn * 2 floats)
                    transpose_4x4_complex(src_ptr, dest_ptr, mm * 2, nn * 2);
                }
                // 处理列剩余 (block 内不足4列的部分)
                for (; jj < j_lim; ++jj) {
                    for (int k = 0; k < 4; ++k) {
                        const float *s = x + ((ii + k) * mm + jj) * complex_stride;
                        float *d = y + (jj * nn + (ii + k)) * complex_stride;
                        vst1_f32(d, vld1_f32(s));
                    }
                }
            }
            // 处理行剩余 (block 内不足4行的部分)
            for (; ii < i_lim; ++ii) {
                 for (int jj = j; jj < j_lim; ++jj) {
                    const float *s = x + (ii * mm + jj) * complex_stride;
                    float *d = y + (jj * nn + ii) * complex_stride;
                    vst1_f32(d, vld1_f32(s));
                 }
            }
        }
    }
    return 0;
}
