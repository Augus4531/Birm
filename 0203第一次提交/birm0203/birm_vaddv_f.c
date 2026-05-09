/**
 * @brief Vector add vector - 64-way unrolling NEON optimized
 *
 * @param x1  Input vector pointer 1
 * @param x2  Input vector pointer 2
 * @param nx  Vector length
 * @param y   Output vector pointer
 * @return int 0 for success, -1 for error
 *
 * Formula: y[i] = x1[i] + x2[i]
 *
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
int birm_vaddv_f(const float *__restrict x1, const float *__restrict x2, const int nx, float *__restrict y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // Hybrid Approach: Dispatch based on size
    // Case 1: Small Size (< 32000) - Use NEON Intrinsics with L1 Blocking
    if (nx < 32000)
    {
        const int BLOCK_SIZE = 4096; // L1 cache friendly block size
        int total_blocks = (nx + BLOCK_SIZE - 1) / BLOCK_SIZE;

        for (int block = 0; block < total_blocks; block++)
        {
            int start = block * BLOCK_SIZE;
            int end = (block == total_blocks - 1) ? nx : start + BLOCK_SIZE;

            // Vectorized loop within block
            for (int idx = start; idx <= end - 16; idx += 16)
            {
                float32x4_t v1_0 = vld1q_f32(&x1[idx]);
                float32x4_t v2_0 = vld1q_f32(&x2[idx]);
                float32x4_t v1_1 = vld1q_f32(&x1[idx + 4]);
                float32x4_t v2_1 = vld1q_f32(&x2[idx + 4]);
                float32x4_t v1_2 = vld1q_f32(&x1[idx + 8]);
                float32x4_t v2_2 = vld1q_f32(&x2[idx + 8]);
                float32x4_t v1_3 = vld1q_f32(&x1[idx + 12]);
                float32x4_t v2_3 = vld1q_f32(&x2[idx + 12]);

                vst1q_f32(&y[idx], vaddq_f32(v1_0, v2_0));
                vst1q_f32(&y[idx + 4], vaddq_f32(v1_1, v2_1));
                vst1q_f32(&y[idx + 8], vaddq_f32(v1_2, v2_2));
                vst1q_f32(&y[idx + 12], vaddq_f32(v1_3, v2_3));
            }

            // Handle remaining elements in block
            int remaining_start = start + ((end - start) / 16) * 16;
            for (int idx = remaining_start; idx < end; idx++)
            {
                y[idx] = x1[idx] + x2[idx];
            }
        }
        return birmSuccess;
    }

    // Case 2: Large Size (>= 32000) - Use Optimized Assembly (8x Unrolling)
    const float *src1 = x1;
    const float *src2 = x2;
    float *dst = y;
    int count = nx;

    // Main loop: process 32 floats (8 vectors) per iteration
    // 8x Unrolling with Instruction Scheduling
    while (count >= 32)
    {
        __asm__ volatile(
            "prfm   pldl1strm, [%[src1], #256]  \n\t" // Streaming Prefetch src1
            "prfm   pldl1strm, [%[src2], #256]  \n\t" // Streaming Prefetch src2

            // Group 1: Load 4 vectors
            "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src1]], #64 \n\t"
            "ld1    {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src2]], #64 \n\t"

            // Group 2: Load next 4 vectors (Interleaved)
            "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src1]], #64 \n\t"
            "ld1    {v20.4s, v21.4s, v22.4s, v23.4s}, [%[src2]], #64 \n\t"

            // Group 1: Add
            "fadd   v0.4s, v0.4s, v16.4s        \n\t"
            "fadd   v1.4s, v1.4s, v17.4s        \n\t"
            "fadd   v2.4s, v2.4s, v18.4s        \n\t"
            "fadd   v3.4s, v3.4s, v19.4s        \n\t"

            // Group 1: Store (Non-temporal)
            "stnp   q0, q1, [%[dst]]            \n\t"
            "stnp   q2, q3, [%[dst], #32]       \n\t"

            // Group 2: Add
            "fadd   v4.4s, v4.4s, v20.4s        \n\t"
            "fadd   v5.4s, v5.4s, v21.4s        \n\t"
            "fadd   v6.4s, v6.4s, v22.4s        \n\t"
            "fadd   v7.4s, v7.4s, v23.4s        \n\t"

            // Group 2: Store (Non-temporal)
            "stnp   q4, q5, [%[dst], #64]       \n\t"
            "stnp   q6, q7, [%[dst], #96]       \n\t"

            "add    %[dst], %[dst], #128        \n\t" // Advance dst pointer

            : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst)
            :
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");
        count -= 32;
    }

    // Handle remaining elements using Assembly (Vectorized Tail)
    if (count > 0)
    {
        __asm__ volatile(
            // Bit 4: 16 elements
            "tbz    %[n], #4, 1f                \n\t"
            "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src1]], #64 \n\t"
            "ld1    {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src2]], #64 \n\t"
            "fadd   v0.4s, v0.4s, v16.4s        \n\t"
            "fadd   v1.4s, v1.4s, v17.4s        \n\t"
            "fadd   v2.4s, v2.4s, v18.4s        \n\t"
            "fadd   v3.4s, v3.4s, v19.4s        \n\t"
            "st1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[dst]], #64 \n\t"
            "1:                                 \n\t"

            // Bit 3: 8 elements
            "tbz    %[n], #3, 2f                \n\t"
            "ld1    {v0.4s, v1.4s}, [%[src1]], #32 \n\t"
            "ld1    {v16.4s, v17.4s}, [%[src2]], #32 \n\t"
            "fadd   v0.4s, v0.4s, v16.4s        \n\t"
            "fadd   v1.4s, v1.4s, v17.4s        \n\t"
            "st1    {v0.4s, v1.4s}, [%[dst]], #32 \n\t"
            "2:                                 \n\t"

            // Bit 2: 4 elements
            "tbz    %[n], #2, 3f                \n\t"
            "ld1    {v0.4s}, [%[src1]], #16     \n\t"
            "ld1    {v16.4s}, [%[src2]], #16    \n\t"
            "fadd   v0.4s, v0.4s, v16.4s        \n\t"
            "st1    {v0.4s}, [%[dst]], #16      \n\t"
            "3:                                 \n\t"

            // Bit 1: 2 elements
            "tbz    %[n], #1, 4f                \n\t"
            "ld1    {v0.2s}, [%[src1]], #8      \n\t"
            "ld1    {v16.2s}, [%[src2]], #8     \n\t"
            "fadd   v0.2s, v0.2s, v16.2s        \n\t"
            "st1    {v0.2s}, [%[dst]], #8       \n\t"
            "4:                                 \n\t"

            // Bit 0: 1 element
            "tbz    %[n], #0, 5f                \n\t"
            "ld1    {v0.s}[0], [%[src1]], #4    \n\t"
            "ld1    {v16.s}[0], [%[src2]], #4   \n\t"
            "fadd   s0, s0, s16                 \n\t"
            "st1    {v0.s}[0], [%[dst]], #4     \n\t"
            "5:                                 \n\t"

            : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst)
            : [n] "r"(count)
            : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");
    }

    return birmSuccess;
}