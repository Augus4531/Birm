/**
 * @file birm_cvswap_f.c
 * @brief Complex vector swap - Optimized for Phytium S5000C
 *        Using inline assembly with aggressive unrolling
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_cvswap_f(const float *__restrict__ x, const int nx, float *__restrict__ y)
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
    const int total = nx * 2; // 总float数量

    // 32个float展开 (16个复数)
    for (; i <= total - 32; i += 32)
    {
        // 加载8个向量（32个float = 16个复数）
        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);
        float32x4_t v2 = vld1q_f32(&x[i + 8]);
        float32x4_t v3 = vld1q_f32(&x[i + 12]);
        float32x4_t v4 = vld1q_f32(&x[i + 16]);
        float32x4_t v5 = vld1q_f32(&x[i + 20]);
        float32x4_t v6 = vld1q_f32(&x[i + 24]);
        float32x4_t v7 = vld1q_f32(&x[i + 28]);

        // rev64: 在每个64位lane内交换两个32位元素
        // [r0, i0, r1, i1] -> [i0, r0, i1, r1]
        v0 = vrev64q_f32(v0);
        v1 = vrev64q_f32(v1);
        v2 = vrev64q_f32(v2);
        v3 = vrev64q_f32(v3);
        v4 = vrev64q_f32(v4);
        v5 = vrev64q_f32(v5);
        v6 = vrev64q_f32(v6);
        v7 = vrev64q_f32(v7);

        // 存储
        vst1q_f32(&y[i], v0);
        vst1q_f32(&y[i + 4], v1);
        vst1q_f32(&y[i + 8], v2);
        vst1q_f32(&y[i + 12], v3);
        vst1q_f32(&y[i + 16], v4);
        vst1q_f32(&y[i + 20], v5);
        vst1q_f32(&y[i + 24], v6);
        vst1q_f32(&y[i + 28], v7);
    }

    // 8个float (4个复数)
    for (; i <= total - 8; i += 8)
    {
        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);
        v0 = vrev64q_f32(v0);
        v1 = vrev64q_f32(v1);
        vst1q_f32(&y[i], v0);
        vst1q_f32(&y[i + 4], v1);
    }

    // 4个float (2个复数)
    for (; i <= total - 4; i += 4)
    {
        float32x4_t v = vld1q_f32(&x[i]);
        v = vrev64q_f32(v);
        vst1q_f32(&y[i], v);
    }

    // 标量尾部 (最多1个复数)
    for (; i < total; i += 2)
    {
        float real = x[i];
        float imag = x[i + 1];
        y[i] = imag;
        y[i + 1] = real;
    }

    return birmSuccess;
}