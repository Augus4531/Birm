#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include "armpl.h"

/**
 * @brief 复数共轭点积 (Complex Vector Dot Product with Conjugate)
 * * Formula: y = sum(x1[i] * conj(x2[i]))
 * (a+bi) * (c-di) = (ac + bd) + i(bc - ad)
 * * @param x1  Input complex vector x1
 * @param x2  Input complex vector x2
 * @param nx  Number of complex elements
 * @param y   Output result [real, imag]
 */
int birm_cvdotjv_f(const float *__restrict x1, const float *__restrict x2, const int nx, float *__restrict y)
{
    // --- 1. 参数检查 ---
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // --- 2. 策略选择 ---

    // 策略 A: 针对大规模数据 (> 256K points)，使用飞腾 VML 库
    // VML 库通常针对大块内存访问和多核/多流水线有内部优化
    if (nx <= 8192)
    {
        float32x4_t vacc_r0 = vdupq_n_f32(0.0f), vacc_i0 = vdupq_n_f32(0.0f);
        float32x4_t vacc_r1 = vdupq_n_f32(0.0f), vacc_i1 = vdupq_n_f32(0.0f);
        float32x4_t vacc_r2 = vdupq_n_f32(0.0f), vacc_i2 = vdupq_n_f32(0.0f);
        float32x4_t vacc_r3 = vdupq_n_f32(0.0f), vacc_i3 = vdupq_n_f32(0.0f);

        int idx = 0;

        // 主循环：16路处理 (4个向量组 x 4 floats)
        for (; idx <= nx - 16; idx += 16)
        {
            // S5000C 硬件预取通常足够好，但保留手动预取用于提示 L2 Cache
            __builtin_prefetch(&x1[2 * (idx + 32)], 0, 3);
            __builtin_prefetch(&x2[2 * (idx + 32)], 0, 3);

            // 加载 Block 0-3 (16个复数)
            float32x4x2_t d10 = vld2q_f32(&x1[2 * (idx + 0)]);
            float32x4x2_t d20 = vld2q_f32(&x2[2 * (idx + 0)]);
            float32x4x2_t d11 = vld2q_f32(&x1[2 * (idx + 4)]);
            float32x4x2_t d21 = vld2q_f32(&x2[2 * (idx + 4)]);
            float32x4x2_t d12 = vld2q_f32(&x1[2 * (idx + 8)]);
            float32x4x2_t d22 = vld2q_f32(&x2[2 * (idx + 8)]);
            float32x4x2_t d13 = vld2q_f32(&x1[2 * (idx + 12)]);
            float32x4x2_t d23 = vld2q_f32(&x2[2 * (idx + 12)]);

            // 计算 x1 * conj(x2)
            // Real = a*c + b*d
            // Imag = b*c - a*d

            // Accumulator 0
            vacc_r0 = vfmaq_f32(vacc_r0, d10.val[0], d20.val[0]);
            vacc_r0 = vfmaq_f32(vacc_r0, d10.val[1], d20.val[1]);
            vacc_i0 = vfmaq_f32(vacc_i0, d10.val[1], d20.val[0]);
            vacc_i0 = vfmsq_f32(vacc_i0, d10.val[0], d20.val[1]);

            // Accumulator 1
            vacc_r1 = vfmaq_f32(vacc_r1, d11.val[0], d21.val[0]);
            vacc_r1 = vfmaq_f32(vacc_r1, d11.val[1], d21.val[1]);
            vacc_i1 = vfmaq_f32(vacc_i1, d11.val[1], d21.val[0]);
            vacc_i1 = vfmsq_f32(vacc_i1, d11.val[0], d21.val[1]);

            // Accumulator 2
            vacc_r2 = vfmaq_f32(vacc_r2, d12.val[0], d22.val[0]);
            vacc_r2 = vfmaq_f32(vacc_r2, d12.val[1], d22.val[1]);
            vacc_i2 = vfmaq_f32(vacc_i2, d12.val[1], d22.val[0]);
            vacc_i2 = vfmsq_f32(vacc_i2, d12.val[0], d22.val[1]);

            // Accumulator 3
            vacc_r3 = vfmaq_f32(vacc_r3, d13.val[0], d23.val[0]);
            vacc_r3 = vfmaq_f32(vacc_r3, d13.val[1], d23.val[1]);
            vacc_i3 = vfmaq_f32(vacc_i3, d13.val[1], d23.val[0]);
            vacc_i3 = vfmsq_f32(vacc_i3, d13.val[0], d23.val[1]);
        }

        // 处理剩余 8 个复数
        for (; idx <= nx - 8; idx += 8)
        {
            float32x4x2_t d10 = vld2q_f32(&x1[2 * (idx + 0)]);
            float32x4x2_t d20 = vld2q_f32(&x2[2 * (idx + 0)]);
            float32x4x2_t d11 = vld2q_f32(&x1[2 * (idx + 4)]);
            float32x4x2_t d21 = vld2q_f32(&x2[2 * (idx + 4)]);

            vacc_r0 = vfmaq_f32(vacc_r0, d10.val[0], d20.val[0]);
            vacc_r0 = vfmaq_f32(vacc_r0, d10.val[1], d20.val[1]);
            vacc_i0 = vfmaq_f32(vacc_i0, d10.val[1], d20.val[0]);
            vacc_i0 = vfmsq_f32(vacc_i0, d10.val[0], d20.val[1]);

            vacc_r1 = vfmaq_f32(vacc_r1, d11.val[0], d21.val[0]);
            vacc_r1 = vfmaq_f32(vacc_r1, d11.val[1], d21.val[1]);
            vacc_i1 = vfmaq_f32(vacc_i1, d11.val[1], d21.val[0]);
            vacc_i1 = vfmsq_f32(vacc_i1, d11.val[0], d21.val[1]);
        }

        // 处理剩余 4 个复数
        for (; idx <= nx - 4; idx += 4)
        {
            float32x4x2_t d1 = vld2q_f32(&x1[2 * idx]);
            float32x4x2_t d2 = vld2q_f32(&x2[2 * idx]);

            vacc_r0 = vfmaq_f32(vacc_r0, d1.val[0], d2.val[0]);
            vacc_r0 = vfmaq_f32(vacc_r0, d1.val[1], d2.val[1]);
            vacc_i0 = vfmaq_f32(vacc_i0, d1.val[1], d2.val[0]);
            vacc_i0 = vfmsq_f32(vacc_i0, d1.val[0], d2.val[1]);
        }

        // 向量归约
        // 先将4路累加器合并
        vacc_r0 = vaddq_f32(vacc_r0, vacc_r1);
        vacc_r2 = vaddq_f32(vacc_r2, vacc_r3);
        vacc_i0 = vaddq_f32(vacc_i0, vacc_i1);
        vacc_i2 = vaddq_f32(vacc_i2, vacc_i3);

        vacc_r0 = vaddq_f32(vacc_r0, vacc_r2);
        vacc_i0 = vaddq_f32(vacc_i0, vacc_i2);

        // 使用 ARMv8 的水平加指令
        float sum_real = vaddvq_f32(vacc_r0);
        float sum_imag = vaddvq_f32(vacc_i0);

        // 处理尾部 (Tail Loop)
        for (; idx < nx; idx++)
        {
            float a = x1[2 * idx];
            float b = x1[2 * idx + 1];
            float c = x2[2 * idx];
            float d = x2[2 * idx + 1];

            // (a+bi)(c-di) = (ac+bd) + i(bc-ad)
            sum_real += (a * c + b * d);
            sum_imag += (b * c - a * d);
        }

        y[0] = sum_real;
        y[1] = sum_imag;

        return birmSuccess;
    }

    // 策略 B: 针对中小规模数据，使用手动优化的 NEON SIMD
    // 能够避免库函数调用的上下文切换开销，且针对流水线进行了排布
    else
    {
        armpl_int_t inc = 1;
        armpl_singlecomplex_t *_y = (armpl_singlecomplex_t *)y;
        _y[0] = cdotc_(&nx, (armpl_singlecomplex_t *)x2, &inc, (armpl_singlecomplex_t *)x1, &inc);

        return birmSuccess;
    }
}