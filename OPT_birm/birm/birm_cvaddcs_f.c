#include <arm_neon.h>
#include "birm_arm_float_User.h"
#include <stdint.h>

int birm_cvaddcs_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !y || !x2)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    const float *src = x1;
    float *dst = y;
    int n_loop = nx;

    // 构造标量向量 {R, I, R, I}
    const float32x4_t v_scalar = {x2[0], x2[1], x2[0], x2[1]};

    // 小数组直接用标量处理
    if (n_loop < 4)
    {
        for (int i = 0; i < n_loop; i++)
        {
            dst[2 * i] = src[2 * i] + x2[0];         // 实部
            dst[2 * i + 1] = src[2 * i + 1] + x2[1]; // 虚部
        }
        return birmSuccess;
    }

    // 预取策略
    if (n_loop > 16)
    {
        __builtin_prefetch(src + 32, 0, 3);
        __builtin_prefetch(dst + 32, 1, 3);
    }

    // 主循环：处理8个复数（16个float）
    while (n_loop >= 8)
    {
        if (n_loop > 16)
        {
            __builtin_prefetch(src + 16, 0, 3);
        }

        // 处理8个复数（16个float）
        float32x4_t a0 = vld1q_f32(src);
        float32x4_t a1 = vld1q_f32(src + 4);
        float32x4_t a2 = vld1q_f32(src + 8);
        float32x4_t a3 = vld1q_f32(src + 12);

        float32x4_t r0 = vaddq_f32(a0, v_scalar);
        float32x4_t r1 = vaddq_f32(a1, v_scalar);
        float32x4_t r2 = vaddq_f32(a2, v_scalar);
        float32x4_t r3 = vaddq_f32(a3, v_scalar);

        vst1q_f32(dst, r0);
        vst1q_f32(dst + 4, r1);
        vst1q_f32(dst + 8, r2);
        vst1q_f32(dst + 12, r3);

        src += 16; // 8个复数 = 16个float
        dst += 16;
        n_loop -= 8;
    }

    // 处理剩余4个复数
    if (n_loop >= 4)
    {
        float32x4_t a0 = vld1q_f32(src);
        float32x4_t a1 = vld1q_f32(src + 4);

        float32x4_t r0 = vaddq_f32(a0, v_scalar);
        float32x4_t r1 = vaddq_f32(a1, v_scalar);

        vst1q_f32(dst, r0);
        vst1q_f32(dst + 4, r1);

        src += 8; // 4个复数 = 8个float
        dst += 8;
        n_loop -= 4;
    }

    // 处理剩余2个复数
    if (n_loop >= 2)
    {
        float32x4_t a = vld1q_f32(src); // 加载4个float（2个复数）
        float32x4_t r = vaddq_f32(a, v_scalar);
        vst1q_f32(dst, r); // 存储4个float（2个复数）

        src += 4; // 2个复数 = 4个float
        dst += 4;
        n_loop -= 2;
    }

    // 处理剩余1个复数
    if (n_loop >= 1)
    {
        dst[0] = src[0] + x2[0]; // 实部
        dst[1] = src[1] + x2[1]; // 虚部
    }

    return birmSuccess;
}
