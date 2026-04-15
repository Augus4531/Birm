/**
 * @brief Complex vector subtract complex scalar - Pipeline Optimized
 *
 * Optimizations:
 * - Software Pipelining: Interleaved Load/Compute/Store to hide latency.
 * - LD1 + STNP: High bandwidth instructions.
 * - No RFO: Explicitly avoiding store prefetch.
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_cvsubcs_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !y || !x2)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    float scalar_real = x2[0];
    float scalar_imag = x2[1];

    const float *src = x1;
    float *dst = y;
    int count = nx;

    // Alignment Peeling
    while (count > 0 && ((uintptr_t)dst & 63))
    {
        *dst++ = *src++ - scalar_real;
        *dst++ = *src++ - scalar_imag;
        count--;
    }

    // Main Loop (16 Complex / 32 Floats / 128 Bytes)
    // Matches the stride of the assembly kernel
    int loop_cnt = count / 16;
    count %= 16;

    if (loop_cnt > 0)
    {
        __asm__ volatile(
            "dup    v30.2s, %w[s_real]          \n\t"
            "ins    v30.s[1], %w[s_imag]        \n\t"
            "dup    v30.2d, v30.d[0]            \n\t"

            ".p2align 6                         \n\t"
            "1:                                 \n\t"
            // Prefetch (Match vmuls_f: 128 bytes per loop, 2 prefetches)
            "prfm   pldl1keep, [%[src], #768]   \n\t"
            "prfm   pldl1keep, [%[src], #576]   \n\t"

            // 1. Load A (64 bytes)
            "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
            // 2. Load B (64 bytes)
            "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src]], #64 \n\t"

            // 3. Compute A
            "fsub   v0.4s, v0.4s, v30.4s        \n\t"
            "fsub   v1.4s, v1.4s, v30.4s        \n\t"
            "fsub   v2.4s, v2.4s, v30.4s        \n\t"
            "fsub   v3.4s, v3.4s, v30.4s        \n\t"

            // 4. Compute B
            "fsub   v4.4s, v4.4s, v30.4s        \n\t"
            "fsub   v5.4s, v5.4s, v30.4s        \n\t"
            "fsub   v6.4s, v6.4s, v30.4s        \n\t"
            "fsub   v7.4s, v7.4s, v30.4s        \n\t"

            // 5. Store A (Using STNP)
            "stnp   q0, q1, [%[dst]]            \n\t"
            "stnp   q2, q3, [%[dst], #32]       \n\t"

            // 6. Store B (Using STNP)
            "stnp   q4, q5, [%[dst], #64]       \n\t"
            "stnp   q6, q7, [%[dst], #96]       \n\t"

            // Update Pointers (Stride = 128 bytes)
            "add    %[dst], %[dst], #128        \n\t"
            "subs   %[cnt], %[cnt], #1          \n\t"
            "b.ne   1b                          \n\t"

            : [src] "+r"(src), [dst] "+r"(dst), [cnt] "+r"(loop_cnt)
            : [s_real] "r"(scalar_real), [s_imag] "r"(scalar_imag)
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v30");
    }

    // Scalar Tail
    while (count > 0)
    {
        *dst++ = *src++ - scalar_real;
        *dst++ = *src++ - scalar_imag;
        count--;
    }

    return birmSuccess;
}