/**
 * @brief Complex vector multiply by real scalar (in-place) - Assembly Optimized
 *
 * Optimizations:
 * - Alignment Peeling: Ensures 16-byte alignment for SIMD loads.
 * - Assembly Kernel: Uses LDP/STNP for maximum bandwidth.
 * - Unrolling: 32x unrolling (32 complex numbers / 64 floats per loop).
 * - Registers: Uses v0-v15 for data, v30 for scalar.
 * - Scheduling: Fine-grained software pipelining.
 * - Prefetching: Streaming prefetch.
 * - Cleanup: Efficient SIMD cleanup for tails.
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_cvmuls_if(float *x1, const float x2, const int nx)
{
    if (!x1)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    float scalar = x2;
    float *ptr = x1;
    // Treat as float array of size 2 * nx
    int total_floats = nx * 2;

    // Alignment Peeling: Align ptr to 16-byte boundary (128-bit)
    // We process 1 float at a time until aligned
    while (total_floats > 0 && ((uintptr_t)ptr & 15))
    {
        *ptr++ *= scalar;
        total_floats--;
    }

    // Assembly Kernel for Large Sizes (>= 64 floats / 32 complex)
    int loop_cnt = total_floats / 64;
    int remain_floats = total_floats % 64;

    if (loop_cnt > 0)
    {
        __asm__ volatile(
            // Prepare scalar register v30 = [s, s, s, s]
            "dup    v30.4s, %w[scalar]          \n\t"

            ".p2align 6                         \n\t" // Align loop to 64-byte boundary
            "1:                                 \n\t"
            // Prefetch (512 bytes ahead)
            "prfm   pldl1strm, [%[ptr], #512]   \n\t"

            // Block 1: Load v0-v3, Load v4-v7
            "ldp    q0, q1, [%[ptr]]            \n\t"
            "ldp    q2, q3, [%[ptr], #32]       \n\t"
            "ldp    q4, q5, [%[ptr], #64]       \n\t"
            "ldp    q6, q7, [%[ptr], #96]       \n\t"

            // Block 2: Compute v0-v3, Load v8-v11
            "fmul   v0.4s, v0.4s, v30.4s        \n\t"
            "fmul   v1.4s, v1.4s, v30.4s        \n\t"
            "fmul   v2.4s, v2.4s, v30.4s        \n\t"
            "fmul   v3.4s, v3.4s, v30.4s        \n\t"
            "ldp    q8, q9, [%[ptr], #128]      \n\t"
            "ldp    q10, q11, [%[ptr], #160]    \n\t"

            // Block 3: Compute v4-v7, Store v0-v3
            "fmul   v4.4s, v4.4s, v30.4s        \n\t"
            "fmul   v5.4s, v5.4s, v30.4s        \n\t"
            "fmul   v6.4s, v6.4s, v30.4s        \n\t"
            "fmul   v7.4s, v7.4s, v30.4s        \n\t"
            "stnp   q0, q1, [%[ptr]]            \n\t"
            "stnp   q2, q3, [%[ptr], #32]       \n\t"

            // Block 4: Load v12-v15, Compute v8-v11
            "ldp    q12, q13, [%[ptr], #192]    \n\t"
            "ldp    q14, q15, [%[ptr], #224]    \n\t"
            "fmul   v8.4s, v8.4s, v30.4s        \n\t"
            "fmul   v9.4s, v9.4s, v30.4s        \n\t"
            "fmul   v10.4s, v10.4s, v30.4s      \n\t"
            "fmul   v11.4s, v11.4s, v30.4s      \n\t"

            // Block 5: Store v4-v7, Compute v12-v15
            "stnp   q4, q5, [%[ptr], #64]       \n\t"
            "stnp   q6, q7, [%[ptr], #96]       \n\t"
            "fmul   v12.4s, v12.4s, v30.4s      \n\t"
            "fmul   v13.4s, v13.4s, v30.4s      \n\t"
            "fmul   v14.4s, v14.4s, v30.4s      \n\t"
            "fmul   v15.4s, v15.4s, v30.4s      \n\t"

            // Block 6: Store v8-v11, Store v12-v15
            "stnp   q8, q9, [%[ptr], #128]      \n\t"
            "stnp   q10, q11, [%[ptr], #160]    \n\t"
            "stnp   q12, q13, [%[ptr], #192]    \n\t"
            "stnp   q14, q15, [%[ptr], #224]    \n\t"

            // Increment pointers
            "add    %[ptr], %[ptr], #256        \n\t"

            "subs   %[cnt], %[cnt], #1          \n\t"
            "b.ne   1b                          \n\t"

            : [ptr] "+r"(ptr), [cnt] "+r"(loop_cnt)
            : [scalar] "r"(scalar)
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15", "v30");
    }

    // SIMD Cleanup for remaining blocks of 8 floats (4 complex)
    // We can process 8 floats at a time using 2 Q-registers
    while (remain_floats >= 8)
    {
        float32x4_t v_s = vdupq_n_f32(scalar);
        float32x4_t v0 = vld1q_f32(ptr);
        float32x4_t v1 = vld1q_f32(ptr + 4);

        v0 = vmulq_f32(v0, v_s);
        v1 = vmulq_f32(v1, v_s);

        vst1q_f32(ptr, v0);
        vst1q_f32(ptr + 4, v1);

        ptr += 8;
        remain_floats -= 8;
    }

    // Scalar Cleanup for remaining floats (< 8)
    while (remain_floats > 0)
    {
        *ptr++ *= scalar;
        remain_floats--;
    }

    return birmSuccess;
}