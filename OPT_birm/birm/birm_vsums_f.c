/**
 * @brief Vector sum of squares - Optimized for Phytium S5000C
 *
 * @param x   Input vector pointer
 * @param nx  Vector length
 * @param y   Output scalar pointer (sum of squares result)
 * @return int 0 for success, -1 for error
 *
 * Formula: y = sum(x[i]^2) for i = 0 to nx-1
 *
 * Platform: Phytium S5000C
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vsums_f(const float *__restrict__ x, const int nx, float *__restrict__ y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    // 4组累加器
    float32x4_t sum0 = vdupq_n_f32(0.0f);
    float32x4_t sum1 = vdupq_n_f32(0.0f);
    float32x4_t sum2 = vdupq_n_f32(0.0f);
    float32x4_t sum3 = vdupq_n_f32(0.0f);

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

        sum0 = vfmaq_f32(sum0, v0, v0);
        sum1 = vfmaq_f32(sum1, v1, v1);
        sum2 = vfmaq_f32(sum2, v2, v2);
        sum3 = vfmaq_f32(sum3, v3, v3);
        sum0 = vfmaq_f32(sum0, v4, v4);
        sum1 = vfmaq_f32(sum1, v5, v5);
        sum2 = vfmaq_f32(sum2, v6, v6);
        sum3 = vfmaq_f32(sum3, v7, v7);
    }

    // 8个float
    for (; i <= nx - 8; i += 8)
    {
        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);
        sum0 = vfmaq_f32(sum0, v0, v0);
        sum1 = vfmaq_f32(sum1, v1, v1);
    }

    // 4个float
    for (; i <= nx - 4; i += 4)
    {
        float32x4_t v = vld1q_f32(&x[i]);
        sum0 = vfmaq_f32(sum0, v, v);
    }

    // 向量归约
    sum0 = vaddq_f32(sum0, sum1);
    sum2 = vaddq_f32(sum2, sum3);
    sum0 = vaddq_f32(sum0, sum2);
    float sum = vaddvq_f32(sum0);

    // 标量尾部
    for (; i < nx; i++)
    {
        sum += x[i] * x[i];
    }

    *y = sum;
    return birmSuccess;
}