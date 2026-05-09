#if 1
/**
 * @file birm_cvdotjv_sf.c
 * @brief Complex conjugate dot product - Optimized for Phytium S5000C
 *        计算 sum(conj(x2) * x1) = sum((c-di)(a+bi)) = sum((ac+bd) + (bc-ad)i)
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_cvdotjv_sf(const float *__restrict__ x1, const int sa,
                    const float *__restrict__ x2, const int sb,
                    const int nx, float *__restrict__ y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    float sum_real = 0.0f;
    float sum_imag = 0.0f;
    int idx = 0;

    // 符号掩码
    const uint32_t mask_data[4] = {0, 0x80000000, 0, 0x80000000};
    uint32x4_t sign_mask = vld1q_u32(mask_data);

    // ========== 步长 sa==1 && sb==1 ==========
    if (sa == 1 && sb == 1)
    {
        float32x4_t acc_r0 = vdupq_n_f32(0), acc_i0 = vdupq_n_f32(0);
        float32x4_t acc_r1 = vdupq_n_f32(0), acc_i1 = vdupq_n_f32(0);
        float32x4_t acc_r2 = vdupq_n_f32(0), acc_i2 = vdupq_n_f32(0);
        float32x4_t acc_r3 = vdupq_n_f32(0), acc_i3 = vdupq_n_f32(0);

        // 32复数展开
        for (; idx <= nx - 32; idx += 32)
        {
            __builtin_prefetch(&x1[(idx + 64) * 2], 0, 0);
            __builtin_prefetch(&x2[(idx + 64) * 2], 0, 0);

            // 批次0-1: 前16个复数
            float32x4_t v1_0 = vld1q_f32(&x1[idx * 2]);
            float32x4_t v1_1 = vld1q_f32(&x1[idx * 2 + 4]);
            float32x4_t v2_0 = vld1q_f32(&x2[idx * 2]);
            float32x4_t v2_1 = vld1q_f32(&x2[idx * 2 + 4]);
            v2_0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_0), sign_mask));
            v2_1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_1), sign_mask));

            float32x4_t a0 = vuzp1q_f32(v1_0, v1_1);
            float32x4_t b0 = vuzp2q_f32(v1_0, v1_1);
            float32x4_t c0 = vuzp1q_f32(v2_0, v2_1);
            float32x4_t d0 = vuzp2q_f32(v2_0, v2_1);

            acc_r0 = vfmaq_f32(acc_r0, c0, a0);
            acc_r0 = vfmsq_f32(acc_r0, d0, b0);
            acc_i0 = vfmaq_f32(acc_i0, c0, b0);
            acc_i0 = vfmaq_f32(acc_i0, d0, a0);

            float32x4_t v1_2 = vld1q_f32(&x1[idx * 2 + 8]);
            float32x4_t v1_3 = vld1q_f32(&x1[idx * 2 + 12]);
            float32x4_t v2_2 = vld1q_f32(&x2[idx * 2 + 8]);
            float32x4_t v2_3 = vld1q_f32(&x2[idx * 2 + 12]);
            v2_2 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_2), sign_mask));
            v2_3 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_3), sign_mask));

            float32x4_t a1 = vuzp1q_f32(v1_2, v1_3);
            float32x4_t b1 = vuzp2q_f32(v1_2, v1_3);
            float32x4_t c1 = vuzp1q_f32(v2_2, v2_3);
            float32x4_t d1 = vuzp2q_f32(v2_2, v2_3);

            acc_r1 = vfmaq_f32(acc_r1, c1, a1);
            acc_r1 = vfmsq_f32(acc_r1, d1, b1);
            acc_i1 = vfmaq_f32(acc_i1, c1, b1);
            acc_i1 = vfmaq_f32(acc_i1, d1, a1);

            float32x4_t v1_4 = vld1q_f32(&x1[idx * 2 + 16]);
            float32x4_t v1_5 = vld1q_f32(&x1[idx * 2 + 20]);
            float32x4_t v2_4 = vld1q_f32(&x2[idx * 2 + 16]);
            float32x4_t v2_5 = vld1q_f32(&x2[idx * 2 + 20]);
            v2_4 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_4), sign_mask));
            v2_5 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_5), sign_mask));

            float32x4_t a2 = vuzp1q_f32(v1_4, v1_5);
            float32x4_t b2 = vuzp2q_f32(v1_4, v1_5);
            float32x4_t c2 = vuzp1q_f32(v2_4, v2_5);
            float32x4_t d2 = vuzp2q_f32(v2_4, v2_5);

            acc_r2 = vfmaq_f32(acc_r2, c2, a2);
            acc_r2 = vfmsq_f32(acc_r2, d2, b2);
            acc_i2 = vfmaq_f32(acc_i2, c2, b2);
            acc_i2 = vfmaq_f32(acc_i2, d2, a2);

            float32x4_t v1_6 = vld1q_f32(&x1[idx * 2 + 24]);
            float32x4_t v1_7 = vld1q_f32(&x1[idx * 2 + 28]);
            float32x4_t v2_6 = vld1q_f32(&x2[idx * 2 + 24]);
            float32x4_t v2_7 = vld1q_f32(&x2[idx * 2 + 28]);
            v2_6 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_6), sign_mask));
            v2_7 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_7), sign_mask));

            float32x4_t a3 = vuzp1q_f32(v1_6, v1_7);
            float32x4_t b3 = vuzp2q_f32(v1_6, v1_7);
            float32x4_t c3 = vuzp1q_f32(v2_6, v2_7);
            float32x4_t d3 = vuzp2q_f32(v2_6, v2_7);

            acc_r3 = vfmaq_f32(acc_r3, c3, a3);
            acc_r3 = vfmsq_f32(acc_r3, d3, b3);
            acc_i3 = vfmaq_f32(acc_i3, c3, b3);
            acc_i3 = vfmaq_f32(acc_i3, d3, a3);

            // 批次2-3: 后16个复数
            v1_0 = vld1q_f32(&x1[idx * 2 + 32]);
            v1_1 = vld1q_f32(&x1[idx * 2 + 36]);
            v2_0 = vld1q_f32(&x2[idx * 2 + 32]);
            v2_1 = vld1q_f32(&x2[idx * 2 + 36]);
            v2_0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_0), sign_mask));
            v2_1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_1), sign_mask));

            a0 = vuzp1q_f32(v1_0, v1_1);
            b0 = vuzp2q_f32(v1_0, v1_1);
            c0 = vuzp1q_f32(v2_0, v2_1);
            d0 = vuzp2q_f32(v2_0, v2_1);
            acc_r0 = vfmaq_f32(acc_r0, c0, a0);
            acc_r0 = vfmsq_f32(acc_r0, d0, b0);
            acc_i0 = vfmaq_f32(acc_i0, c0, b0);
            acc_i0 = vfmaq_f32(acc_i0, d0, a0);

            v1_2 = vld1q_f32(&x1[idx * 2 + 40]);
            v1_3 = vld1q_f32(&x1[idx * 2 + 44]);
            v2_2 = vld1q_f32(&x2[idx * 2 + 40]);
            v2_3 = vld1q_f32(&x2[idx * 2 + 44]);
            v2_2 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_2), sign_mask));
            v2_3 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_3), sign_mask));

            a1 = vuzp1q_f32(v1_2, v1_3);
            b1 = vuzp2q_f32(v1_2, v1_3);
            c1 = vuzp1q_f32(v2_2, v2_3);
            d1 = vuzp2q_f32(v2_2, v2_3);
            acc_r1 = vfmaq_f32(acc_r1, c1, a1);
            acc_r1 = vfmsq_f32(acc_r1, d1, b1);
            acc_i1 = vfmaq_f32(acc_i1, c1, b1);
            acc_i1 = vfmaq_f32(acc_i1, d1, a1);

            v1_4 = vld1q_f32(&x1[idx * 2 + 48]);
            v1_5 = vld1q_f32(&x1[idx * 2 + 52]);
            v2_4 = vld1q_f32(&x2[idx * 2 + 48]);
            v2_5 = vld1q_f32(&x2[idx * 2 + 52]);
            v2_4 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_4), sign_mask));
            v2_5 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_5), sign_mask));

            a2 = vuzp1q_f32(v1_4, v1_5);
            b2 = vuzp2q_f32(v1_4, v1_5);
            c2 = vuzp1q_f32(v2_4, v2_5);
            d2 = vuzp2q_f32(v2_4, v2_5);
            acc_r2 = vfmaq_f32(acc_r2, c2, a2);
            acc_r2 = vfmsq_f32(acc_r2, d2, b2);
            acc_i2 = vfmaq_f32(acc_i2, c2, b2);
            acc_i2 = vfmaq_f32(acc_i2, d2, a2);

            v1_6 = vld1q_f32(&x1[idx * 2 + 56]);
            v1_7 = vld1q_f32(&x1[idx * 2 + 60]);
            v2_6 = vld1q_f32(&x2[idx * 2 + 56]);
            v2_7 = vld1q_f32(&x2[idx * 2 + 60]);
            v2_6 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_6), sign_mask));
            v2_7 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_7), sign_mask));

            a3 = vuzp1q_f32(v1_6, v1_7);
            b3 = vuzp2q_f32(v1_6, v1_7);
            c3 = vuzp1q_f32(v2_6, v2_7);
            d3 = vuzp2q_f32(v2_6, v2_7);
            acc_r3 = vfmaq_f32(acc_r3, c3, a3);
            acc_r3 = vfmsq_f32(acc_r3, d3, b3);
            acc_i3 = vfmaq_f32(acc_i3, c3, b3);
            acc_i3 = vfmaq_f32(acc_i3, d3, a3);
        }

        // 16复数
        for (; idx <= nx - 16; idx += 16)
        {
            float32x4_t v1_0 = vld1q_f32(&x1[idx * 2]);
            float32x4_t v1_1 = vld1q_f32(&x1[idx * 2 + 4]);
            float32x4_t v2_0 = vld1q_f32(&x2[idx * 2]);
            float32x4_t v2_1 = vld1q_f32(&x2[idx * 2 + 4]);
            v2_0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_0), sign_mask));
            v2_1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_1), sign_mask));

            float32x4_t a0 = vuzp1q_f32(v1_0, v1_1), b0 = vuzp2q_f32(v1_0, v1_1);
            float32x4_t c0 = vuzp1q_f32(v2_0, v2_1), d0 = vuzp2q_f32(v2_0, v2_1);
            acc_r0 = vfmaq_f32(acc_r0, c0, a0);
            acc_r0 = vfmsq_f32(acc_r0, d0, b0);
            acc_i0 = vfmaq_f32(acc_i0, c0, b0);
            acc_i0 = vfmaq_f32(acc_i0, d0, a0);

            float32x4_t v1_2 = vld1q_f32(&x1[idx * 2 + 8]);
            float32x4_t v1_3 = vld1q_f32(&x1[idx * 2 + 12]);
            float32x4_t v2_2 = vld1q_f32(&x2[idx * 2 + 8]);
            float32x4_t v2_3 = vld1q_f32(&x2[idx * 2 + 12]);
            v2_2 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_2), sign_mask));
            v2_3 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_3), sign_mask));

            float32x4_t a1 = vuzp1q_f32(v1_2, v1_3), b1 = vuzp2q_f32(v1_2, v1_3);
            float32x4_t c1 = vuzp1q_f32(v2_2, v2_3), d1 = vuzp2q_f32(v2_2, v2_3);
            acc_r1 = vfmaq_f32(acc_r1, c1, a1);
            acc_r1 = vfmsq_f32(acc_r1, d1, b1);
            acc_i1 = vfmaq_f32(acc_i1, c1, b1);
            acc_i1 = vfmaq_f32(acc_i1, d1, a1);

            float32x4_t v1_4 = vld1q_f32(&x1[idx * 2 + 16]);
            float32x4_t v1_5 = vld1q_f32(&x1[idx * 2 + 20]);
            float32x4_t v2_4 = vld1q_f32(&x2[idx * 2 + 16]);
            float32x4_t v2_5 = vld1q_f32(&x2[idx * 2 + 20]);
            v2_4 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_4), sign_mask));
            v2_5 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_5), sign_mask));

            float32x4_t a2 = vuzp1q_f32(v1_4, v1_5), b2 = vuzp2q_f32(v1_4, v1_5);
            float32x4_t c2 = vuzp1q_f32(v2_4, v2_5), d2 = vuzp2q_f32(v2_4, v2_5);
            acc_r2 = vfmaq_f32(acc_r2, c2, a2);
            acc_r2 = vfmsq_f32(acc_r2, d2, b2);
            acc_i2 = vfmaq_f32(acc_i2, c2, b2);
            acc_i2 = vfmaq_f32(acc_i2, d2, a2);

            float32x4_t v1_6 = vld1q_f32(&x1[idx * 2 + 24]);
            float32x4_t v1_7 = vld1q_f32(&x1[idx * 2 + 28]);
            float32x4_t v2_6 = vld1q_f32(&x2[idx * 2 + 24]);
            float32x4_t v2_7 = vld1q_f32(&x2[idx * 2 + 28]);
            v2_6 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_6), sign_mask));
            v2_7 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_7), sign_mask));

            float32x4_t a3 = vuzp1q_f32(v1_6, v1_7), b3 = vuzp2q_f32(v1_6, v1_7);
            float32x4_t c3 = vuzp1q_f32(v2_6, v2_7), d3 = vuzp2q_f32(v2_6, v2_7);
            acc_r3 = vfmaq_f32(acc_r3, c3, a3);
            acc_r3 = vfmsq_f32(acc_r3, d3, b3);
            acc_i3 = vfmaq_f32(acc_i3, c3, b3);
            acc_i3 = vfmaq_f32(acc_i3, d3, a3);
        }

        // 4复数
        for (; idx <= nx - 4; idx += 4)
        {
            float32x4_t v1_0 = vld1q_f32(&x1[idx * 2]);
            float32x4_t v1_1 = vld1q_f32(&x1[idx * 2 + 4]);
            float32x4_t v2_0 = vld1q_f32(&x2[idx * 2]);
            float32x4_t v2_1 = vld1q_f32(&x2[idx * 2 + 4]);
            v2_0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_0), sign_mask));
            v2_1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v2_1), sign_mask));

            float32x4_t a = vuzp1q_f32(v1_0, v1_1), b = vuzp2q_f32(v1_0, v1_1);
            float32x4_t c = vuzp1q_f32(v2_0, v2_1), d = vuzp2q_f32(v2_0, v2_1);
            acc_r0 = vfmaq_f32(acc_r0, c, a);
            acc_r0 = vfmsq_f32(acc_r0, d, b);
            acc_i0 = vfmaq_f32(acc_i0, c, b);
            acc_i0 = vfmaq_f32(acc_i0, d, a);
        }

        acc_r0 = vaddq_f32(vaddq_f32(acc_r0, acc_r1), vaddq_f32(acc_r2, acc_r3));
        acc_i0 = vaddq_f32(vaddq_f32(acc_i0, acc_i1), vaddq_f32(acc_i2, acc_i3));
        sum_real = vaddvq_f32(acc_r0);
        sum_imag = vaddvq_f32(acc_i0);
    }
    // ========== 步长 sa==2 && sb==2：使用 ldp+trn 替代 ld4 ==========
    else if (sa == 2 && sb == 2)
    {
        const float *ptr_x1 = x1;
        const float *ptr_x2 = x2;
        long loop_cnt = nx;

        // Accumulators
        float32x4_t vacc_r0 = vdupq_n_f32(0.0f);
        float32x4_t vacc_i0 = vdupq_n_f32(0.0f);
        float32x4_t vacc_r1 = vdupq_n_f32(0.0f);
        float32x4_t vacc_i1 = vdupq_n_f32(0.0f);

        // Inline ASM for optimal instruction scheduling and zero-overhead addressing
        // Math: Sum += conj(x2) * x1
        // (c - di) * (a + bi) = (ac + bd) + i(cb - da)
        // Real += x2.re*x1.re + x2.im*x1.im
        // Imag += x2.re*x1.im - x2.im*x1.re

        asm volatile(
            "1: \n\t"
            "cmp %0, #8 \n\t"
            "blt 2f \n\t"

            // --- Load Block 0 (Indices 0, 2, 4, 6) ---
            "ld2 {v0.2d, v1.2d}, [%[px1]], #32 \n\t" // v0={x1[0], x1[2]}
            "ld2 {v2.2d, v3.2d}, [%[px1]], #32 \n\t" // v2={x1[4], x1[6]}
            "ld2 {v4.2d, v5.2d}, [%[px2]], #32 \n\t" // v4={x2[0], x2[2]}
            "ld2 {v6.2d, v7.2d}, [%[px2]], #32 \n\t" // v6={x2[4], x2[6]}

            // Shuffle
            "uzp1 v8.4s, v0.4s, v2.4s \n\t"  // x1_re
            "uzp2 v9.4s, v0.4s, v2.4s \n\t"  // x1_im
            "uzp1 v10.4s, v4.4s, v6.4s \n\t" // x2_re
            "uzp2 v11.4s, v4.4s, v6.4s \n\t" // x2_im

            // --- Load Block 1 (Indices 8, 10, 12, 14) ---
            "ld2 {v0.2d, v1.2d}, [%[px1]], #32 \n\t"
            "ld2 {v2.2d, v3.2d}, [%[px1]], #32 \n\t"
            "ld2 {v4.2d, v5.2d}, [%[px2]], #32 \n\t"
            "ld2 {v6.2d, v7.2d}, [%[px2]], #32 \n\t"

            // Shuffle
            "uzp1 v12.4s, v0.4s, v2.4s \n\t" // x1_re
            "uzp2 v13.4s, v0.4s, v2.4s \n\t" // x1_im
            "uzp1 v14.4s, v4.4s, v6.4s \n\t" // x2_re
            "uzp2 v15.4s, v4.4s, v6.4s \n\t" // x2_im

            // --- Calc Block 0 ---
            // Real: ac + bd
            "fmla %[vr0].4s, v10.4s, v8.4s \n\t" // + x2.re * x1.re
            "fmla %[vr0].4s, v11.4s, v9.4s \n\t" // + x2.im * x1.im
            // Imag: cb - da
            "fmla %[vi0].4s, v10.4s, v9.4s \n\t" // + x2.re * x1.im
            "fmls %[vi0].4s, v11.4s, v8.4s \n\t" // - x2.im * x1.re

            // --- Calc Block 1 ---
            "fmla %[vr1].4s, v14.4s, v12.4s \n\t"
            "fmla %[vr1].4s, v15.4s, v13.4s \n\t"
            "fmla %[vi1].4s, v14.4s, v13.4s \n\t"
            "fmls %[vi1].4s, v15.4s, v12.4s \n\t"

            "sub %0, %0, #8 \n\t"
            "b 1b \n\t"
            "2: \n\t"
            : "+r"(loop_cnt), [px1] "+r"(ptr_x1), [px2] "+r"(ptr_x2),
              [vr0] "+w"(vacc_r0), [vi0] "+w"(vacc_i0),
              [vr1] "+w"(vacc_r1), [vi1] "+w"(vacc_i1)
            :
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15");

        idx = nx - loop_cnt;

        vacc_r0 = vaddq_f32(vacc_r0, vacc_r1);
        vacc_i0 = vaddq_f32(vacc_i0, vacc_i1);
        sum_real = vaddvq_f32(vacc_r0);
        sum_imag = vaddvq_f32(vacc_i0);
    }

    // 标量尾部
    for (; idx < nx; idx++)
    {
        int idx1 = idx * sa;
        int idx2 = idx * sb;
        float a = x1[2 * idx1];
        float b = x1[2 * idx1 + 1];
        float c = x2[2 * idx2];
        float d = x2[2 * idx2 + 1];
        sum_real += c * a + d * b;
        sum_imag += c * b - d * a;
    }

    y[0] = sum_real;
    y[1] = sum_imag;

    return birmSuccess;
}