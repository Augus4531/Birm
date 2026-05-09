/**
 * @brief Vector sum with stride - Fully Optimized with vld2q_f32
 *
 * @param x   Input vector pointer
 * @param sa  Stride for input vector x
 * @param nx  Number of elements to sum
 * @param y   Output scalar pointer (sum result)
 * @return int 0 for success, -1 for error
 *
 * Formula: y = sum(x[i*sa]) for i = 0 to nx-1
 *
 * Platform: Phytium S5000C
 */
#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
int birm_vsum_sf(const float *__restrict__ x, const int sa, const int nx, float *__restrict__ y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0)
    {
        return birmParamLengthInvalidError;
    }

    float sum = 0.0f;
    int idx = 0;

    // ===== Optimized case: continuous memory (stride = 1) =====
    if (sa == 1)
    {
        // Use 8 accumulators for maximum ILP
        float32x4_t vsum0 = vdupq_n_f32(0.0f);
        float32x4_t vsum1 = vdupq_n_f32(0.0f);
        float32x4_t vsum2 = vdupq_n_f32(0.0f);
        float32x4_t vsum3 = vdupq_n_f32(0.0f);
        float32x4_t vsum4 = vdupq_n_f32(0.0f);
        float32x4_t vsum5 = vdupq_n_f32(0.0f);
        float32x4_t vsum6 = vdupq_n_f32(0.0f);
        float32x4_t vsum7 = vdupq_n_f32(0.0f);

        // 128-way unrolling: process 128 elements at a time
        for (; idx <= nx - 128; idx += 128)
        {
            __builtin_prefetch(&x[idx + 128], 0, 0);

            // Load and add in groups of 8 vectors
            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 4]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 8]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 12]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 16]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 20]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 24]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 28]));

            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx + 32]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 36]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 40]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 44]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 48]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 52]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 56]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 60]));

            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx + 64]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 68]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 72]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 76]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 80]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 84]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 88]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 92]));

            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx + 96]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 100]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 104]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 108]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 112]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 116]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 120]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 124]));
        }

        // 64-way unrolling
        for (; idx <= nx - 64; idx += 64)
        {
            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 4]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 8]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 12]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 16]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 20]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 24]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 28]));

            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx + 32]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 36]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 40]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 44]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 48]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 52]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 56]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 60]));
        }

        // 32-way unrolling
        for (; idx <= nx - 32; idx += 32)
        {
            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 4]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 8]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 12]));
            vsum4 = vaddq_f32(vsum4, vld1q_f32(&x[idx + 16]));
            vsum5 = vaddq_f32(vsum5, vld1q_f32(&x[idx + 20]));
            vsum6 = vaddq_f32(vsum6, vld1q_f32(&x[idx + 24]));
            vsum7 = vaddq_f32(vsum7, vld1q_f32(&x[idx + 28]));
        }

        // 16-way unrolling
        for (; idx <= nx - 16; idx += 16)
        {
            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 4]));
            vsum2 = vaddq_f32(vsum2, vld1q_f32(&x[idx + 8]));
            vsum3 = vaddq_f32(vsum3, vld1q_f32(&x[idx + 12]));
        }

        // 8-way unrolling
        for (; idx <= nx - 8; idx += 8)
        {
            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx]));
            vsum1 = vaddq_f32(vsum1, vld1q_f32(&x[idx + 4]));
        }

        // 4-way NEON processing
        for (; idx <= nx - 4; idx += 4)
        {
            vsum0 = vaddq_f32(vsum0, vld1q_f32(&x[idx]));
        }

        // Combine 8 accumulators
        vsum0 = vaddq_f32(vsum0, vsum1);
        vsum2 = vaddq_f32(vsum2, vsum3);
        vsum4 = vaddq_f32(vsum4, vsum5);
        vsum6 = vaddq_f32(vsum6, vsum7);
        vsum0 = vaddq_f32(vsum0, vsum2);
        vsum4 = vaddq_f32(vsum4, vsum6);
        vsum0 = vaddq_f32(vsum0, vsum4);

        sum = vaddvq_f32(vsum0);
    }
    // ===== Optimized stride=2 case using vld2q_f32 (TRUE stride-2 access) =====
    else if (sa == 2)
    {
        // Use 8 accumulators for maximum ILP
        float32x4_t vsum0 = vdupq_n_f32(0.0f);
        float32x4_t vsum1 = vdupq_n_f32(0.0f);
        float32x4_t vsum2 = vdupq_n_f32(0.0f);
        float32x4_t vsum3 = vdupq_n_f32(0.0f);
        float32x4_t vsum4 = vdupq_n_f32(0.0f);
        float32x4_t vsum5 = vdupq_n_f32(0.0f);
        float32x4_t vsum6 = vdupq_n_f32(0.0f);
        float32x4_t vsum7 = vdupq_n_f32(0.0f);

        // 64-way unrolling for stride=2: process 64 elements at a time
        // vld2q_f32 loads 8 floats and deinterleaves into 2 lanes
        // For stride-2: lane[0] contains the stride-2 elements
        for (; idx <= nx - 64; idx += 64)
        {
            __builtin_prefetch(&x[(idx + 64) * 2], 0, 0);

            // Each vld2q_f32 loads 8 floats and gives us 4 stride-2 elements
            float32x4x2_t data0 = vld2q_f32(&x[idx * 2]);
            float32x4x2_t data1 = vld2q_f32(&x[(idx + 4) * 2]);
            float32x4x2_t data2 = vld2q_f32(&x[(idx + 8) * 2]);
            float32x4x2_t data3 = vld2q_f32(&x[(idx + 12) * 2]);
            float32x4x2_t data4 = vld2q_f32(&x[(idx + 16) * 2]);
            float32x4x2_t data5 = vld2q_f32(&x[(idx + 20) * 2]);
            float32x4x2_t data6 = vld2q_f32(&x[(idx + 24) * 2]);
            float32x4x2_t data7 = vld2q_f32(&x[(idx + 28) * 2]);

            // Add val[0] which contains the stride-2 elements
            vsum0 = vaddq_f32(vsum0, data0.val[0]);
            vsum1 = vaddq_f32(vsum1, data1.val[0]);
            vsum2 = vaddq_f32(vsum2, data2.val[0]);
            vsum3 = vaddq_f32(vsum3, data3.val[0]);
            vsum4 = vaddq_f32(vsum4, data4.val[0]);
            vsum5 = vaddq_f32(vsum5, data5.val[0]);
            vsum6 = vaddq_f32(vsum6, data6.val[0]);
            vsum7 = vaddq_f32(vsum7, data7.val[0]);

            // Next 32 elements
            float32x4x2_t data8 = vld2q_f32(&x[(idx + 32) * 2]);
            float32x4x2_t data9 = vld2q_f32(&x[(idx + 36) * 2]);
            float32x4x2_t data10 = vld2q_f32(&x[(idx + 40) * 2]);
            float32x4x2_t data11 = vld2q_f32(&x[(idx + 44) * 2]);
            float32x4x2_t data12 = vld2q_f32(&x[(idx + 48) * 2]);
            float32x4x2_t data13 = vld2q_f32(&x[(idx + 52) * 2]);
            float32x4x2_t data14 = vld2q_f32(&x[(idx + 56) * 2]);
            float32x4x2_t data15 = vld2q_f32(&x[(idx + 60) * 2]);

            vsum0 = vaddq_f32(vsum0, data8.val[0]);
            vsum1 = vaddq_f32(vsum1, data9.val[0]);
            vsum2 = vaddq_f32(vsum2, data10.val[0]);
            vsum3 = vaddq_f32(vsum3, data11.val[0]);
            vsum4 = vaddq_f32(vsum4, data12.val[0]);
            vsum5 = vaddq_f32(vsum5, data13.val[0]);
            vsum6 = vaddq_f32(vsum6, data14.val[0]);
            vsum7 = vaddq_f32(vsum7, data15.val[0]);
        }

        // 32-way unrolling
        for (; idx <= nx - 32; idx += 32)
        {
            float32x4x2_t data0 = vld2q_f32(&x[idx * 2]);
            float32x4x2_t data1 = vld2q_f32(&x[(idx + 4) * 2]);
            float32x4x2_t data2 = vld2q_f32(&x[(idx + 8) * 2]);
            float32x4x2_t data3 = vld2q_f32(&x[(idx + 12) * 2]);
            float32x4x2_t data4 = vld2q_f32(&x[(idx + 16) * 2]);
            float32x4x2_t data5 = vld2q_f32(&x[(idx + 20) * 2]);
            float32x4x2_t data6 = vld2q_f32(&x[(idx + 24) * 2]);
            float32x4x2_t data7 = vld2q_f32(&x[(idx + 28) * 2]);

            vsum0 = vaddq_f32(vsum0, data0.val[0]);
            vsum1 = vaddq_f32(vsum1, data1.val[0]);
            vsum2 = vaddq_f32(vsum2, data2.val[0]);
            vsum3 = vaddq_f32(vsum3, data3.val[0]);
            vsum4 = vaddq_f32(vsum4, data4.val[0]);
            vsum5 = vaddq_f32(vsum5, data5.val[0]);
            vsum6 = vaddq_f32(vsum6, data6.val[0]);
            vsum7 = vaddq_f32(vsum7, data7.val[0]);
        }

        // 16-way unrolling
        for (; idx <= nx - 16; idx += 16)
        {
            float32x4x2_t data0 = vld2q_f32(&x[idx * 2]);
            float32x4x2_t data1 = vld2q_f32(&x[(idx + 4) * 2]);
            float32x4x2_t data2 = vld2q_f32(&x[(idx + 8) * 2]);
            float32x4x2_t data3 = vld2q_f32(&x[(idx + 12) * 2]);

            vsum0 = vaddq_f32(vsum0, data0.val[0]);
            vsum1 = vaddq_f32(vsum1, data1.val[0]);
            vsum2 = vaddq_f32(vsum2, data2.val[0]);
            vsum3 = vaddq_f32(vsum3, data3.val[0]);
        }

        // 8-way unrolling
        for (; idx <= nx - 8; idx += 8)
        {
            float32x4x2_t data0 = vld2q_f32(&x[idx * 2]);
            float32x4x2_t data1 = vld2q_f32(&x[(idx + 4) * 2]);

            vsum0 = vaddq_f32(vsum0, data0.val[0]);
            vsum1 = vaddq_f32(vsum1, data1.val[0]);
        }

        // 4-way processing
        for (; idx <= nx - 4; idx += 4)
        {
            float32x4x2_t data = vld2q_f32(&x[idx * 2]);
            vsum0 = vaddq_f32(vsum0, data.val[0]);
        }

        // Combine 8 accumulators
        vsum0 = vaddq_f32(vsum0, vsum1);
        vsum2 = vaddq_f32(vsum2, vsum3);
        vsum4 = vaddq_f32(vsum4, vsum5);
        vsum6 = vaddq_f32(vsum6, vsum7);
        vsum0 = vaddq_f32(vsum0, vsum2);
        vsum4 = vaddq_f32(vsum4, vsum6);
        vsum0 = vaddq_f32(vsum0, vsum4);

        sum = vaddvq_f32(vsum0);
    }

    // Scalar processing for remaining elements or general stride
    for (; idx < nx; idx++)
    {
        sum += x[idx * sa];
    }

    *y = sum;
    return birmSuccess;
}