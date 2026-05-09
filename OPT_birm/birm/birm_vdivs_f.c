/*
 * Function: birm_vdivs_f
 * Optimization: Inline Assembly with Cache Optimization
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vdivs_f(const float *x, const float s, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // Calculate reciprocal: inv_s = 1.0 / s
    float inv_s = 1.0f / s;

    // Broadcast reciprocal to a vector register using C intrinsics
    // This avoids complex inline assembly syntax for element broadcasting
    float32x4_t v_inv = vdupq_n_f32(inv_s);

    const float *src = x;
    float *dst = y;
    int count = nx;

    // Use two paths: Large Array (Non-Temporal) vs Small Array (Temporal)
    // Threshold: 32000 elements (same as .S implementation)

    if (nx > 32000)
    {
        // -------------------------------------------------
        // Large Data Path: Use STNP (Non-Temporal Store)
        // -------------------------------------------------
        asm volatile(
            // Load v30 with the broadcasted value
            "mov v30.16b, %[v_inv].16b \n\t"

            // Main Loop: 32 elements per iteration
            "1: \n\t"
            "cmp %w[cnt], #32 \n\t"
            "blt 2f \n\t"

            "prfm pldl1keep, [%[src], #512] \n\t"

            "ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
            "ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src]], #64 \n\t"

            "fmul v0.4s, v0.4s, v30.4s \n\t"
            "fmul v1.4s, v1.4s, v30.4s \n\t"
            "fmul v2.4s, v2.4s, v30.4s \n\t"
            "fmul v3.4s, v3.4s, v30.4s \n\t"
            "fmul v4.4s, v4.4s, v30.4s \n\t"
            "fmul v5.4s, v5.4s, v30.4s \n\t"
            "fmul v6.4s, v6.4s, v30.4s \n\t"
            "fmul v7.4s, v7.4s, v30.4s \n\t"

            // Non-Temporal Stores
            "stnp q0, q1, [%[dst]] \n\t"
            "stnp q2, q3, [%[dst], #32] \n\t"
            "stnp q4, q5, [%[dst], #64] \n\t"
            "stnp q6, q7, [%[dst], #96] \n\t"

            "add %[dst], %[dst], #128 \n\t"
            "sub %w[cnt], %w[cnt], #32 \n\t"
            "b 1b \n\t"
            "2: \n\t"
            : [src] "+r"(src), [dst] "+r"(dst), [cnt] "+r"(count)
            : [v_inv] "w"(v_inv)
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v30");
    }
    else
    {
        // -------------------------------------------------
        // Small Data Path: Use ST1 (Temporal Store)
        // -------------------------------------------------
        asm volatile(
            "mov v30.16b, %[v_inv].16b \n\t"

            "1: \n\t"
            "cmp %w[cnt], #64 \n\t"
            "blt 2f \n\t"

            "prfm pldl1keep, [%[src], #320] \n\t"

            // Load 64 elements (8 * 128-bit)
            "ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
            "ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src]], #64 \n\t"
            "ld1 {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src]], #64 \n\t"
            "ld1 {v20.4s, v21.4s, v22.4s, v23.4s}, [%[src]], #64 \n\t"

            // Use interleaved multiplication to hide latency
            "fmul v0.4s, v0.4s, v30.4s \n\t"
            "fmul v1.4s, v1.4s, v30.4s \n\t"
            "fmul v2.4s, v2.4s, v30.4s \n\t"
            "fmul v3.4s, v3.4s, v30.4s \n\t"
            "fmul v4.4s, v4.4s, v30.4s \n\t"
            "fmul v5.4s, v5.4s, v30.4s \n\t"
            "fmul v6.4s, v6.4s, v30.4s \n\t"
            "fmul v7.4s, v7.4s, v30.4s \n\t"

            "fmul v16.4s, v16.4s, v30.4s \n\t"
            "fmul v17.4s, v17.4s, v30.4s \n\t"
            "fmul v18.4s, v18.4s, v30.4s \n\t"
            "fmul v19.4s, v19.4s, v30.4s \n\t"
            "fmul v20.4s, v20.4s, v30.4s \n\t"
            "fmul v21.4s, v21.4s, v30.4s \n\t"
            "fmul v22.4s, v22.4s, v30.4s \n\t"
            "fmul v23.4s, v23.4s, v30.4s \n\t"

            "st1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[dst]], #64 \n\t"
            "st1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[dst]], #64 \n\t"
            "st1 {v16.4s, v17.4s, v18.4s, v19.4s}, [%[dst]], #64 \n\t"
            "st1 {v20.4s, v21.4s, v22.4s, v23.4s}, [%[dst]], #64 \n\t"

            "sub %w[cnt], %w[cnt], #64 \n\t"
            "b 1b \n\t"
            "2: \n\t"
            : [src] "+r"(src), [dst] "+r"(dst), [cnt] "+r"(count)
            : [v_inv] "w"(v_inv)
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v30",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");
    }

    // Tail Handling (Common)
    // Uses C loop for simplicity, compiler will vectorize nicely
    for (int i = 0; i < count; i++)
    {
        dst[i] = src[i] * inv_s;
    }

    return birmSuccess;
}
