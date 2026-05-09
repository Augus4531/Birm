/**
 * @file birm_jv_f.c
 * @brief Complex vector conjugate - Using ld1/st1 with alternating mask
 *        Avoids expensive ld2/st2 interleaving
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_jv_f(const float *__restrict__ x, const int nx, float *__restrict__ y)
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

    // 交替符号掩码: [0, 0x80000000, 0, 0x80000000]
    // 只翻转奇数位置（虚部）的符号
    const uint32_t mask_data[4] = {0, 0x80000000, 0, 0x80000000};
    uint32x4_t sign_mask = vld1q_u32(mask_data);

    // 使用 ld1/st1 连续加载，避免 ld2/st2 的交错开销
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

        // XOR 翻转虚部符号（奇数位置）
        v0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v0), sign_mask));
        v1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v1), sign_mask));
        v2 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2), sign_mask));
        v3 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v3), sign_mask));
        v4 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v4), sign_mask));
        v5 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v5), sign_mask));
        v6 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v6), sign_mask));
        v7 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v7), sign_mask));

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

    // 8个float
    for (; i <= total - 8; i += 8)
    {
        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);
        v0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v0), sign_mask));
        v1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v1), sign_mask));
        vst1q_f32(&y[i], v0);
        vst1q_f32(&y[i + 4], v1);
    }

    // 4个float
    for (; i <= total - 4; i += 4)
    {
        float32x4_t v = vld1q_f32(&x[i]);
        v = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v), sign_mask));
        vst1q_f32(&y[i], v);
    }

    // 标量尾部
    for (; i < total; i += 2)
    {
        y[i] = x[i];
        y[i + 1] = -x[i + 1];
    }

    return birmSuccess;
}