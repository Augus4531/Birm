/**
 * @file birm_vmean_f.c
 * @brief Vector mean (average) - Optimized with AArch64 Inline Assembly (Double Precision)
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vmean_f(const float *__restrict__ x, const int nx, float *__restrict__ y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    long n_remain = nx;
    const float *p_x = x;
    double total_sum = 0.0;

    // -----------------------------------------------------------------------
    // Assembly Block
    // Process 32 floats per loop iteration
    // -----------------------------------------------------------------------
    if (n_remain >= 32)
    {
        long loops = n_remain / 32;
        n_remain = n_remain % 32;

        __asm__ volatile(
            // --- Initialize Accumulators (16 doubles) ---
            "movi    v16.2d, #0\n"
            "movi    v17.2d, #0\n"
            "movi    v18.2d, #0\n"
            "movi    v19.2d, #0\n"
            "movi    v20.2d, #0\n"
            "movi    v21.2d, #0\n"
            "movi    v22.2d, #0\n"
            "movi    v23.2d, #0\n"

            "1:\n"
            // Prefetch
            "prfm    pldl1keep, [%[x], #512]\n"

            // --- Load 32 floats (8 vectors) ---
            "ld1     {v0.4s, v1.4s, v2.4s, v3.4s}, [%[x]], #64\n"
            "ld1     {v4.4s, v5.4s, v6.4s, v7.4s}, [%[x]], #64\n"

            // --- Convert and Accumulate Group 1 (v0-v3) ---
            // v0 -> v8(low), v9(high)
            "fcvtl   v8.2d, v0.2s\n"
            "fcvtl2  v9.2d, v0.4s\n"
            "fadd    v16.2d, v16.2d, v8.2d\n"
            "fadd    v17.2d, v17.2d, v9.2d\n"

            // v1 -> v10, v11
            "fcvtl   v10.2d, v1.2s\n"
            "fcvtl2  v11.2d, v1.4s\n"
            "fadd    v18.2d, v18.2d, v10.2d\n"
            "fadd    v19.2d, v19.2d, v11.2d\n"

            // v2 -> v12, v13
            "fcvtl   v12.2d, v2.2s\n"
            "fcvtl2  v13.2d, v2.4s\n"
            "fadd    v20.2d, v20.2d, v12.2d\n"
            "fadd    v21.2d, v21.2d, v13.2d\n"

            // v3 -> v14, v15
            "fcvtl   v14.2d, v3.2s\n"
            "fcvtl2  v15.2d, v3.4s\n"
            "fadd    v22.2d, v22.2d, v14.2d\n"
            "fadd    v23.2d, v23.2d, v15.2d\n"

            // --- Convert and Accumulate Group 2 (v4-v7) ---
            // v4
            "fcvtl   v8.2d, v4.2s\n"
            "fcvtl2  v9.2d, v4.4s\n"
            "fadd    v16.2d, v16.2d, v8.2d\n"
            "fadd    v17.2d, v17.2d, v9.2d\n"

            // v5
            "fcvtl   v10.2d, v5.2s\n"
            "fcvtl2  v11.2d, v5.4s\n"
            "fadd    v18.2d, v18.2d, v10.2d\n"
            "fadd    v19.2d, v19.2d, v11.2d\n"

            // v6
            "fcvtl   v12.2d, v6.2s\n"
            "fcvtl2  v13.2d, v6.4s\n"
            "fadd    v20.2d, v20.2d, v12.2d\n"
            "fadd    v21.2d, v21.2d, v13.2d\n"

            // v7
            "fcvtl   v14.2d, v7.2s\n"
            "fcvtl2  v15.2d, v7.4s\n"
            "fadd    v22.2d, v22.2d, v14.2d\n"
            "fadd    v23.2d, v23.2d, v15.2d\n"

            // --- Loop Control ---
            "subs    %[loops], %[loops], #1\n"
            "b.gt    1b\n"

            // --- Reduction ---
            // Sum v16-v23
            "fadd    v16.2d, v16.2d, v17.2d\n"
            "fadd    v18.2d, v18.2d, v19.2d\n"
            "fadd    v20.2d, v20.2d, v21.2d\n"
            "fadd    v22.2d, v22.2d, v23.2d\n"

            "fadd    v16.2d, v16.2d, v18.2d\n"
            "fadd    v20.2d, v20.2d, v22.2d\n"

            "fadd    v16.2d, v16.2d, v20.2d\n"

            "faddp   d16, v16.2d\n"

            "str     d16, [%[sum]]\n"

            : [x] "+r"(p_x), [loops] "+r"(loops)
            : [sum] "r"(&total_sum)
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");
    }

    // Scalar Tail
    for (int i = 0; i < n_remain; i++)
    {
        total_sum += (double)(*p_x++);
    }

    *y = (float)(total_sum / nx);
    return birmSuccess;
}