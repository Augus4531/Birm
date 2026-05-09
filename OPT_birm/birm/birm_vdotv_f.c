/**
 * @brief Vector dot product - Optimized with 8-way Parallel Accumulators
 *
 * Optimization Strategy:
 * - 8-way Parallel Sum Accumulation: Uses 8 independent NEON registers (sum0-sum7)
 *   to break dependency chains and hide FMLA instruction latency (typically 4-5 cycles).
 * - Aggressive Unrolling: Processes 32 floats per iteration (8 vectors).
 * - Software Pipelining: Detailed instruction scheduling (Load -> FMLA).
 * - Cache Friendly: Sequential access pattern maximizing hardware prefetching.
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vdotv_f(const float *__restrict__ x1, const float *__restrict__ x2, const int nx, float *__restrict__ y)
{
    // 1. Parameter check
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 2. Main Processing
    int i = 0;

    // Initialize 8 independent accumulators to break dependency chains
    float32x4_t sum0 = vdupq_n_f32(0.0f);
    float32x4_t sum1 = vdupq_n_f32(0.0f);
    float32x4_t sum2 = vdupq_n_f32(0.0f);
    float32x4_t sum3 = vdupq_n_f32(0.0f);
    float32x4_t sum4 = vdupq_n_f32(0.0f);
    float32x4_t sum5 = vdupq_n_f32(0.0f);
    float32x4_t sum6 = vdupq_n_f32(0.0f);
    float32x4_t sum7 = vdupq_n_f32(0.0f);

    // Main Loop: 32 floats per iteration (8 vectors)
    // This allows 8 FMLA instructions to be in flight simultaneously
    for (; i <= nx - 32; i += 32)
    {
        // Block 1: Load 4 vectors (16 floats)
        float32x4_t a0 = vld1q_f32(&x1[i + 0]);
        float32x4_t b0 = vld1q_f32(&x2[i + 0]);
        float32x4_t a1 = vld1q_f32(&x1[i + 4]);
        float32x4_t b1 = vld1q_f32(&x2[i + 4]);
        float32x4_t a2 = vld1q_f32(&x1[i + 8]);
        float32x4_t b2 = vld1q_f32(&x2[i + 8]);
        float32x4_t a3 = vld1q_f32(&x1[i + 12]);
        float32x4_t b3 = vld1q_f32(&x2[i + 12]);

        // Compute 4 vectors
        sum0 = vmlaq_f32(sum0, a0, b0);
        sum1 = vmlaq_f32(sum1, a1, b1);
        sum2 = vmlaq_f32(sum2, a2, b2);
        sum3 = vmlaq_f32(sum3, a3, b3);

        // Block 2: Load next 4 vectors (16 floats)
        float32x4_t a4 = vld1q_f32(&x1[i + 16]);
        float32x4_t b4 = vld1q_f32(&x2[i + 16]);
        float32x4_t a5 = vld1q_f32(&x1[i + 20]);
        float32x4_t b5 = vld1q_f32(&x2[i + 20]);
        float32x4_t a6 = vld1q_f32(&x1[i + 24]);
        float32x4_t b6 = vld1q_f32(&x2[i + 24]);
        float32x4_t a7 = vld1q_f32(&x1[i + 28]);
        float32x4_t b7 = vld1q_f32(&x2[i + 28]);

        // Compute next 4 vectors
        sum4 = vmlaq_f32(sum4, a4, b4);
        sum5 = vmlaq_f32(sum5, a5, b5);
        sum6 = vmlaq_f32(sum6, a6, b6);
        sum7 = vmlaq_f32(sum7, a7, b7);
    }

    // Residual handling: 16 floats (4 vectors)
    for (; i <= nx - 16; i += 16)
    {
        float32x4_t a0 = vld1q_f32(&x1[i + 0]);
        float32x4_t b0 = vld1q_f32(&x2[i + 0]);
        float32x4_t a1 = vld1q_f32(&x1[i + 4]);
        float32x4_t b1 = vld1q_f32(&x2[i + 4]);
        float32x4_t a2 = vld1q_f32(&x1[i + 8]);
        float32x4_t b2 = vld1q_f32(&x2[i + 8]);
        float32x4_t a3 = vld1q_f32(&x1[i + 12]);
        float32x4_t b3 = vld1q_f32(&x2[i + 12]);

        sum0 = vmlaq_f32(sum0, a0, b0);
        sum1 = vmlaq_f32(sum1, a1, b1);
        sum2 = vmlaq_f32(sum2, a2, b2);
        sum3 = vmlaq_f32(sum3, a3, b3);
    }

    // Residual handling: 4 floats (1 vector)
    for (; i <= nx - 4; i += 4)
    {
        float32x4_t a = vld1q_f32(&x1[i]);
        float32x4_t b = vld1q_f32(&x2[i]);
        sum0 = vmlaq_f32(sum0, a, b);
    }

    // 3. Reduction
    // Step 1: Reduce 8 accumulators to 4
    sum0 = vaddq_f32(sum0, sum4);
    sum1 = vaddq_f32(sum1, sum5);
    sum2 = vaddq_f32(sum2, sum6);
    sum3 = vaddq_f32(sum3, sum7);

    // Step 2: Reduce 4 accumulators to 1
    sum0 = vaddq_f32(sum0, sum1);
    sum2 = vaddq_f32(sum2, sum3);
    sum0 = vaddq_f32(sum0, sum2);

    // Step 3: Horizontal reduction (vector -> scalar)
    float final_sum = vaddvq_f32(sum0);

    // 4. Scalar Residual Handling
    for (; i < nx; i++)
    {
        final_sum += x1[i] * x2[i];
    }

    *y = final_sum;
    return birmSuccess;
}