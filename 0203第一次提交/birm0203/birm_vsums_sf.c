/**
 * @brief Vector sum of squares with stride - NEON optimized
 *
 * @param x   Input vector pointer
 * @param sa  Stride for input vector x
 * @param nx  Number of elements to process
 * @param y   Output scalar pointer (sum of squares result)
 * @return int 0 for success, -1 for error
 *
 * Formula: y = sum(x[i*sa]^2) for i = 0 to nx-1
 *
 * Optimization: Stride=2 case is now fully optimized with vld2q_f32.
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
int birm_vsums_sf(const float *x, const int sa, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0)
    {
        return birmParamLengthInvalidError;
    }

    float sum_squares = 0.0f;
    int idx = 0;

    // Optimized case: continuous memory (stride = 1)
    if (sa == 1)
    {
        float32x4_t vsum = vdupq_n_f32(0.0f);

        // 32-way unrolling: process 32 elements at a time
        for (; idx <= nx - 32; idx += 32)
        {
            __builtin_prefetch(&x[idx + 64], 0, 0);

            float32x4_t v0 = vld1q_f32(&x[idx]);
            float32x4_t v1 = vld1q_f32(&x[idx + 4]);
            float32x4_t v2 = vld1q_f32(&x[idx + 8]);
            float32x4_t v3 = vld1q_f32(&x[idx + 12]);
            float32x4_t v4 = vld1q_f32(&x[idx + 16]);
            float32x4_t v5 = vld1q_f32(&x[idx + 20]);
            float32x4_t v6 = vld1q_f32(&x[idx + 24]);
            float32x4_t v7 = vld1q_f32(&x[idx + 28]);

            vsum = vfmaq_f32(vsum, v0, v0);
            vsum = vfmaq_f32(vsum, v1, v1);
            vsum = vfmaq_f32(vsum, v2, v2);
            vsum = vfmaq_f32(vsum, v3, v3);
            vsum = vfmaq_f32(vsum, v4, v4);
            vsum = vfmaq_f32(vsum, v5, v5);
            vsum = vfmaq_f32(vsum, v6, v6);
            vsum = vfmaq_f32(vsum, v7, v7);
        }

        // 4-way NEON processing
        for (; idx <= nx - 4; idx += 4)
        {
            float32x4_t v = vld1q_f32(&x[idx]);
            vsum = vfmaq_f32(vsum, v, v);
        }

        sum_squares = vaddvq_f32(vsum);
    }
    // ===== Optimized stride=2 case using vld2q_f32 =====
    else if (sa == 2)
    {
        float32x4_t vsum0 = vdupq_n_f32(0.0f);
        float32x4_t vsum1 = vdupq_n_f32(0.0f);
        float32x4_t vsum2 = vdupq_n_f32(0.0f);
        float32x4_t vsum3 = vdupq_n_f32(0.0f);
        float32x4_t vsum4 = vdupq_n_f32(0.0f);
        float32x4_t vsum5 = vdupq_n_f32(0.0f);
        float32x4_t vsum6 = vdupq_n_f32(0.0f);
        float32x4_t vsum7 = vdupq_n_f32(0.0f);

        for (; idx <= nx - 32; idx += 32)
        {
            __builtin_prefetch(&x[(idx + 32) * 2], 0, 0);

            float32x4x2_t d0 = vld2q_f32(&x[idx * 2]);
            float32x4x2_t d1 = vld2q_f32(&x[(idx + 4) * 2]);
            float32x4x2_t d2 = vld2q_f32(&x[(idx + 8) * 2]);
            float32x4x2_t d3 = vld2q_f32(&x[(idx + 12) * 2]);
            float32x4x2_t d4 = vld2q_f32(&x[(idx + 16) * 2]);
            float32x4x2_t d5 = vld2q_f32(&x[(idx + 20) * 2]);
            float32x4x2_t d6 = vld2q_f32(&x[(idx + 24) * 2]);
            float32x4x2_t d7 = vld2q_f32(&x[(idx + 28) * 2]);

            vsum0 = vfmaq_f32(vsum0, d0.val[0], d0.val[0]);
            vsum1 = vfmaq_f32(vsum1, d1.val[0], d1.val[0]);
            vsum2 = vfmaq_f32(vsum2, d2.val[0], d2.val[0]);
            vsum3 = vfmaq_f32(vsum3, d3.val[0], d3.val[0]);
            vsum4 = vfmaq_f32(vsum4, d4.val[0], d4.val[0]);
            vsum5 = vfmaq_f32(vsum5, d5.val[0], d5.val[0]);
            vsum6 = vfmaq_f32(vsum6, d6.val[0], d6.val[0]);
            vsum7 = vfmaq_f32(vsum7, d7.val[0], d7.val[0]);
        }

        for (; idx <= nx - 16; idx += 16)
        {
            float32x4x2_t d0 = vld2q_f32(&x[idx * 2]);
            float32x4x2_t d1 = vld2q_f32(&x[(idx + 4) * 2]);
            float32x4x2_t d2 = vld2q_f32(&x[(idx + 8) * 2]);
            float32x4x2_t d3 = vld2q_f32(&x[(idx + 12) * 2]);

            vsum0 = vfmaq_f32(vsum0, d0.val[0], d0.val[0]);
            vsum1 = vfmaq_f32(vsum1, d1.val[0], d1.val[0]);
            vsum2 = vfmaq_f32(vsum2, d2.val[0], d2.val[0]);
            vsum3 = vfmaq_f32(vsum3, d3.val[0], d3.val[0]);
        }

        for (; idx <= nx - 4; idx += 4)
        {
            float32x4x2_t d0 = vld2q_f32(&x[idx * 2]);
            vsum0 = vfmaq_f32(vsum0, d0.val[0], d0.val[0]);
        }

        vsum0 = vaddq_f32(vsum0, vsum1);
        vsum2 = vaddq_f32(vsum2, vsum3);
        vsum4 = vaddq_f32(vsum4, vsum5);
        vsum6 = vaddq_f32(vsum6, vsum7);
        vsum0 = vaddq_f32(vsum0, vsum2);
        vsum4 = vaddq_f32(vsum4, vsum6);
        vsum0 = vaddq_f32(vsum0, vsum4);

        sum_squares = vaddvq_f32(vsum0);
    }

    for (; idx < nx; idx++)
    {
        float val = x[idx * sa];
        sum_squares += val * val;
    }

    *y = sum_squares;
    return birmSuccess;
}