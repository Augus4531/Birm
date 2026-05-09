/**
 * @file birm_vvar_f.c
 * @brief Vector variance calculation - Optimized with AArch64 Inline Assembly (One-Pass)
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vvar_f(const float *__restrict__ x, const int nx, float *__restrict__ y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;
    if (nx == 1)
    {
        *y = 0.0f;
        return birmSuccess;
    }

    int i = 0;

    // 单精度累加器 (4组，减少依赖)
    float32x4_t sum0 = vdupq_n_f32(0.0f);
    float32x4_t sum1 = vdupq_n_f32(0.0f);
    float32x4_t sum2_0 = vdupq_n_f32(0.0f);
    float32x4_t sum2_1 = vdupq_n_f32(0.0f);

    // 32个float展开
    for (; i <= nx - 32; i += 32)
    {
        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);
        float32x4_t v2 = vld1q_f32(&x[i + 8]);
        float32x4_t v3 = vld1q_f32(&x[i + 12]);
        float32x4_t v4 = vld1q_f32(&x[i + 16]);
        float32x4_t v5 = vld1q_f32(&x[i + 20]);
        float32x4_t v6 = vld1q_f32(&x[i + 24]);
        float32x4_t v7 = vld1q_f32(&x[i + 28]);

        // 累加 sum
        sum0 = vaddq_f32(sum0, v0);
        sum1 = vaddq_f32(sum1, v1);
        sum0 = vaddq_f32(sum0, v2);
        sum1 = vaddq_f32(sum1, v3);
        sum0 = vaddq_f32(sum0, v4);
        sum1 = vaddq_f32(sum1, v5);
        sum0 = vaddq_f32(sum0, v6);
        sum1 = vaddq_f32(sum1, v7);

        // 累加 sum² (fmla: sum2 += v * v)
        sum2_0 = vfmaq_f32(sum2_0, v0, v0);
        sum2_1 = vfmaq_f32(sum2_1, v1, v1);
        sum2_0 = vfmaq_f32(sum2_0, v2, v2);
        sum2_1 = vfmaq_f32(sum2_1, v3, v3);
        sum2_0 = vfmaq_f32(sum2_0, v4, v4);
        sum2_1 = vfmaq_f32(sum2_1, v5, v5);
        sum2_0 = vfmaq_f32(sum2_0, v6, v6);
        sum2_1 = vfmaq_f32(sum2_1, v7, v7);
    }

    // 8个float
    for (; i <= nx - 8; i += 8)
    {
        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);

        sum0 = vaddq_f32(sum0, v0);
        sum1 = vaddq_f32(sum1, v1);
        sum2_0 = vfmaq_f32(sum2_0, v0, v0);
        sum2_1 = vfmaq_f32(sum2_1, v1, v1);
    }

    // 4个float
    for (; i <= nx - 4; i += 4)
    {
        float32x4_t v = vld1q_f32(&x[i]);
        sum0 = vaddq_f32(sum0, v);
        sum2_0 = vfmaq_f32(sum2_0, v, v);
    }

    // 向量归约
    sum0 = vaddq_f32(sum0, sum1);
    sum2_0 = vaddq_f32(sum2_0, sum2_1);
    float sum_x = vaddvq_f32(sum0);
    float sum_x2 = vaddvq_f32(sum2_0);

    // 标量尾部
    for (; i < nx; i++)
    {
        float val = x[i];
        sum_x += val;
        sum_x2 += val * val;
    }

    // 计算方差: Var = E[x²] - E[x]²
    float inv_n = 1.0f / nx;
    float mean = sum_x * inv_n;
    float mean_sq = sum_x2 * inv_n;
    *y = mean_sq - mean * mean;

    return birmSuccess;
}