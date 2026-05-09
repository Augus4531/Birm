#include <arm_neon.h>
#include "birm_arm_float_User.h"

// 核心优化：4x4 float 块转置
static inline void transpose_4x4_neon(const float *src, float *dst, int src_stride, int dst_stride)
{
    // 加载 4 行，每行 4 个 float
    float32x4_t row0 = vld1q_f32(src);
    float32x4_t row1 = vld1q_f32(src + src_stride);
    float32x4_t row2 = vld1q_f32(src + 2 * src_stride);
    float32x4_t row3 = vld1q_f32(src + 3 * src_stride);

    // ----------------------------------------------------
    // 使用 NEON 指令进行 4x4 矩阵转置
    // ----------------------------------------------------

    // 第一次交换：处理 2x2 块内部
    // t0 = {r0[0], r1[0], r0[2], r1[2]} (交叉) -> 错误，vtrn是按对取
    // vtrn1: 取两个向量的低半部分交叉; vtrn2: 取高半部分交叉

    // trn0 = {r0[0], r1[0], r0[2], r1[2]}
    // trn1 = {r0[1], r1[1], r0[3], r1[3]}
    // 上面的注释是针对交错的理解。实际 vtrn1q_f32(a, b) 结果是:
    // {a[0], b[0], a[2], b[2]}

    float32x4_t t0 = vtrn1q_f32(row0, row1); // {r0[0], r1[0], r0[2], r1[2]}
    float32x4_t t1 = vtrn2q_f32(row0, row1); // {r0[1], r1[1], r0[3], r1[3]}
    float32x4_t t2 = vtrn1q_f32(row2, row3); // {r2[0], r3[0], r2[2], r3[2]}
    float32x4_t t3 = vtrn2q_f32(row2, row3); // {r2[1], r3[1], r2[3], r3[3]}

    // 第二次交换：组合上下两半
    // 现在的 t0: Col 0 的上半部, Col 2 的上半部
    // 现在的 t2: Col 0 的下半部, Col 2 的下半部

    // 我们需要将 Col 0 的上下部拼在一起 -> Dest Row 0
    // vtrn1q_64 (将两个向量视为 64位 元素进行转置) 可以完美解决
    // 但为了通用性（防止某些编译器对 cast 的严格限制），我们可以继续用 f32 指令或者 cast

    // 使用 vreinterpret 转换为 64位整数操作进行块交换 (低64位和高64位)
    // Dest Row 0 = {t0_low, t2_low}
    float32x4_t out0 = vcombine_f32(vget_low_f32(t0), vget_low_f32(t2));
    // Dest Row 1 = {t1_low, t3_low}
    float32x4_t out1 = vcombine_f32(vget_low_f32(t1), vget_low_f32(t3));
    // Dest Row 2 = {t0_high, t2_high}
    float32x4_t out2 = vcombine_f32(vget_high_f32(t0), vget_high_f32(t2));
    // Dest Row 3 = {t1_high, t3_high}
    float32x4_t out3 = vcombine_f32(vget_high_f32(t1), vget_high_f32(t3));

    // 写入目标：现在的 out0..3 已经是转置好的行
    vst1q_f32(dst, out0);
    vst1q_f32(dst + dst_stride, out1);
    vst1q_f32(dst + 2 * dst_stride, out2);
    vst1q_f32(dst + 3 * dst_stride, out3);
}

int birm_mtrans_f(const float *x, const int nn, const int mm, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nn <= 0 || mm <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 针对缓存大小选择合适的 Block Size
    // 32 或 16 都是常见选择，适配 L1 Cache
    const int BLOCK_SIZE = 32;

    for (int i = 0; i < nn; i += BLOCK_SIZE)
    {
        for (int j = 0; j < mm; j += BLOCK_SIZE)
        {

            // 计算当前 Block 的边界
            int i_lim = (i + BLOCK_SIZE < nn) ? (i + BLOCK_SIZE) : nn;
            int j_lim = (j + BLOCK_SIZE < mm) ? (j + BLOCK_SIZE) : mm;

            // 核心优化路径：处理 4x4 对齐块
            int ii = i;
            for (; ii <= i_lim - 4; ii += 4)
            {
                int jj = j;
                for (; jj <= j_lim - 4; jj += 4)
                {
                    const float *src_ptr = x + ii * mm + jj;
                    // 注意：转置后，原矩阵的 (ii, jj) 变成目标矩阵的 (jj, ii)
                    // 目标矩阵每行长度是 nn
                    float *dst_ptr = y + jj * nn + ii;

                    transpose_4x4_neon(src_ptr, dst_ptr, mm, nn);
                }

                // 处理列剩余 (Block 内部不足 4 的列)
                for (; jj < j_lim; ++jj)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        y[jj * nn + (ii + k)] = x[(ii + k) * mm + jj];
                    }
                }
            }

            // 处理行剩余 (Block 内部不足 4 的行)
            for (; ii < i_lim; ++ii)
            {
                for (int jj = j; jj < j_lim; ++jj)
                {
                    y[jj * nn + ii] = x[ii * mm + jj];
                }
            }
        }
    }

    return birmSuccess;
}
