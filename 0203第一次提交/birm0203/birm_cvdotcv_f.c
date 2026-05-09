/**
 * @file birm_cvdotcv_f.c
 * @brief Complex vector dot product optimized with AArch64 Inline Assembly
 * @details
 * Optimization:
 * - Hand-written Assembly for maximum pipeline utilization.
 * - 8 independent accumulators to hide FMA latency.
 * - Interleaved Load/Calculate instructions.
 * - Direct memory store for results to avoid register constraints issues.
 * @date 2025-11-28
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>

/**
 * @brief Complex Vector Dot Product (Unconjugated): y = sum(x1 * x2)
 * Formula: (a+bi)(c+di) = (ac - bd) + i(ad + bc)
 *
 * Optimization: Phytium S5000C
 * - 32-way Unrolling (Strictly matched to loop counter)
 * - 8 Parallel Accumulators
 * - LD2 zero-overhead de-interleaving
 */
int birm_cvdotcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    const float *p_x1 = x1;
    const float *p_x2 = x2;

    // 最终结果累加器
    float res_real = 0.0f;
    float res_imag = 0.0f;

    long loops = nx / 32;  // 32 complex numbers per main loop
    long remain = nx % 32; // Remaining items

    // -----------------------------------------------------------------------
    // Main Assembly Block
    // -----------------------------------------------------------------------
    __asm__ volatile(
        // --- 1. Initialize Accumulators (v16-v19 Real, v20-v23 Imag) ---
        "eor    v16.16b, v16.16b, v16.16b \n\t"
        "eor    v17.16b, v17.16b, v17.16b \n\t"
        "eor    v18.16b, v18.16b, v18.16b \n\t"
        "eor    v19.16b, v19.16b, v19.16b \n\t"
        "eor    v20.16b, v20.16b, v20.16b \n\t"
        "eor    v21.16b, v21.16b, v21.16b \n\t"
        "eor    v22.16b, v22.16b, v22.16b \n\t"
        "eor    v23.16b, v23.16b, v23.16b \n\t"

        // Check if main loop is needed
        "cbz    %[loops], .Lcheck_16 \n\t"

        // Align loop start
        ".p2align 5 \n\t"
        ".Lloop_32: \n\t"
        // =========================================================
        // Block 1 (Items 0-3)
        // =========================================================
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v16.4s, v0.4s, v2.4s \n\t" // R += ac
        "fmls   v16.4s, v1.4s, v3.4s \n\t" // R -= bd
        "fmla   v20.4s, v0.4s, v3.4s \n\t" // I += ad
        "fmla   v20.4s, v1.4s, v2.4s \n\t" // I += bc

        // =========================================================
        // Block 2 (Items 4-7)
        // =========================================================
        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v17.4s, v4.4s, v6.4s \n\t"
        "fmls   v17.4s, v5.4s, v7.4s \n\t"
        "fmla   v21.4s, v4.4s, v7.4s \n\t"
        "fmla   v21.4s, v5.4s, v6.4s \n\t"

        // =========================================================
        // Block 3 (Items 8-11)
        // =========================================================
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v18.4s, v0.4s, v2.4s \n\t"
        "fmls   v18.4s, v1.4s, v3.4s \n\t"
        "fmla   v22.4s, v0.4s, v3.4s \n\t"
        "fmla   v22.4s, v1.4s, v2.4s \n\t"

        // =========================================================
        // Block 4 (Items 12-15)
        // =========================================================
        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v19.4s, v4.4s, v6.4s \n\t"
        "fmls   v19.4s, v5.4s, v7.4s \n\t"
        "fmla   v23.4s, v4.4s, v7.4s \n\t"
        "fmla   v23.4s, v5.4s, v6.4s \n\t"

        // =========================================================
        // Block 5 (Items 16-19) - REUSE v16, v20 accumulator
        // =========================================================
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v16.4s, v0.4s, v2.4s \n\t"
        "fmls   v16.4s, v1.4s, v3.4s \n\t"
        "fmla   v20.4s, v0.4s, v3.4s \n\t"
        "fmla   v20.4s, v1.4s, v2.4s \n\t"

        // =========================================================
        // Block 6 (Items 20-23) - REUSE v17, v21 accumulator
        // =========================================================
        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v17.4s, v4.4s, v6.4s \n\t"
        "fmls   v17.4s, v5.4s, v7.4s \n\t"
        "fmla   v21.4s, v4.4s, v7.4s \n\t"
        "fmla   v21.4s, v5.4s, v6.4s \n\t"

        // =========================================================
        // Block 7 (Items 24-27) - REUSE v18, v22 accumulator
        // =========================================================
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v18.4s, v0.4s, v2.4s \n\t"
        "fmls   v18.4s, v1.4s, v3.4s \n\t"
        "fmla   v22.4s, v0.4s, v3.4s \n\t"
        "fmla   v22.4s, v1.4s, v2.4s \n\t"

        // =========================================================
        // Block 8 (Items 28-31) - REUSE v19, v23 accumulator
        // =========================================================
        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v19.4s, v4.4s, v6.4s \n\t"
        "fmls   v19.4s, v5.4s, v7.4s \n\t"
        "fmla   v23.4s, v4.4s, v7.4s \n\t"
        "fmla   v23.4s, v5.4s, v6.4s \n\t"

        "subs   %[loops], %[loops], #1 \n\t"
        "b.ne   .Lloop_32 \n\t"

        // ---------------------------------------------------------
        // Vectorized Tail Handling (16 / 8 / 4 items)
        // ---------------------------------------------------------
        ".Lcheck_16: \n\t"
        "tst    %[rem], #16 \n\t"
        "b.eq   .Lcheck_8 \n\t"

        // Process 16 items
        // G1
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v16.4s, v0.4s, v2.4s \n\t"
        "fmls   v16.4s, v1.4s, v3.4s \n\t"
        "fmla   v20.4s, v0.4s, v3.4s \n\t"
        "fmla   v20.4s, v1.4s, v2.4s \n\t"
        // G2
        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v17.4s, v4.4s, v6.4s \n\t"
        "fmls   v17.4s, v5.4s, v7.4s \n\t"
        "fmla   v21.4s, v4.4s, v7.4s \n\t"
        "fmla   v21.4s, v5.4s, v6.4s \n\t"
        // G3
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v18.4s, v0.4s, v2.4s \n\t"
        "fmls   v18.4s, v1.4s, v3.4s \n\t"
        "fmla   v22.4s, v0.4s, v3.4s \n\t"
        "fmla   v22.4s, v1.4s, v2.4s \n\t"
        // G4
        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v19.4s, v4.4s, v6.4s \n\t"
        "fmls   v19.4s, v5.4s, v7.4s \n\t"
        "fmla   v23.4s, v4.4s, v7.4s \n\t"
        "fmla   v23.4s, v5.4s, v6.4s \n\t"

        ".Lcheck_8: \n\t"
        "tst    %[rem], #8 \n\t"
        "b.eq   .Lcheck_4 \n\t"

        // Process 8 items
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v16.4s, v0.4s, v2.4s \n\t"
        "fmls   v16.4s, v1.4s, v3.4s \n\t"
        "fmla   v20.4s, v0.4s, v3.4s \n\t"
        "fmla   v20.4s, v1.4s, v2.4s \n\t"

        "ld2    {v4.4s, v5.4s}, [%[x1]], #32 \n\t"
        "ld2    {v6.4s, v7.4s}, [%[x2]], #32 \n\t"
        "fmla   v17.4s, v4.4s, v6.4s \n\t"
        "fmls   v17.4s, v5.4s, v7.4s \n\t"
        "fmla   v21.4s, v4.4s, v7.4s \n\t"
        "fmla   v21.4s, v5.4s, v6.4s \n\t"

        ".Lcheck_4: \n\t"
        "tst    %[rem], #4 \n\t"
        "b.eq   .Lreduce \n\t"

        // Process 4 items
        "ld2    {v0.4s, v1.4s}, [%[x1]], #32 \n\t"
        "ld2    {v2.4s, v3.4s}, [%[x2]], #32 \n\t"
        "fmla   v18.4s, v0.4s, v2.4s \n\t"
        "fmls   v18.4s, v1.4s, v3.4s \n\t"
        "fmla   v22.4s, v0.4s, v3.4s \n\t"
        "fmla   v22.4s, v1.4s, v2.4s \n\t"

        // ---------------------------------------------------------
        // Reduction
        // ---------------------------------------------------------
        ".Lreduce: \n\t"
        // Combine Real (v16, v17, v18, v19)
        "fadd   v16.4s, v16.4s, v18.4s \n\t"
        "fadd   v17.4s, v17.4s, v19.4s \n\t"
        "fadd   v16.4s, v16.4s, v17.4s \n\t"

        // Combine Imag (v20, v21, v22, v23)
        "fadd   v20.4s, v20.4s, v22.4s \n\t"
        "fadd   v21.4s, v21.4s, v23.4s \n\t"
        "fadd   v20.4s, v20.4s, v21.4s \n\t"

        // Horizontal Add
        "faddp  v16.4s, v16.4s, v16.4s \n\t"
        "faddp  v16.4s, v16.4s, v16.4s \n\t"

        "faddp  v20.4s, v20.4s, v20.4s \n\t"
        "faddp  v20.4s, v20.4s, v20.4s \n\t"

        // Export to C variables (using scalar view)
        "fmov   %s[out_re], s16 \n\t"
        "fmov   %s[out_im], s20 \n\t"

        : [out_re] "=w"(res_real), [out_im] "=w"(res_imag),
          [x1] "+r"(p_x1), [x2] "+r"(p_x2), [loops] "+r"(loops)
        : [rem] "r"(remain)
        : "cc", "memory",
          "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
          "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");

    // -----------------------------------------------------------------------
    // Scalar Tail Loop (Handle 0-3 remaining items)
    // -----------------------------------------------------------------------
    int tail = remain & 3;
    for (int k = 0; k < tail; k++)
    {
        float ar = p_x1[0];
        float ai = p_x1[1];
        float br = p_x2[0];
        float bi = p_x2[1];

        // Real = ac - bd
        res_real += (ar * br - ai * bi);
        // Imag = ad + bc
        res_imag += (ar * bi + ai * br);

        p_x1 += 2;
        p_x2 += 2;
    }

    y[0] = res_real;
    y[1] = res_imag;

    return birmSuccess;
}