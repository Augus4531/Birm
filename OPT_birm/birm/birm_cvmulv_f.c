/**
 * @brief Complex vector multiply by real vector - Adaptive Multi-Tier Optimization
 *
 * @param x1  Input complex vector (interleaved: [r0,i0,r1,i1,...])
 * @param x2  Input real vector
 * @param nx  Number of complex numbers (1 to 1M+)
 * @param y   Output complex vector (interleaved)
 * @return int 0 for success, error code otherwise
 *
 * Optimization tiers:
 * - Tiny (1-15):     Direct scalar, no loop overhead
 * - Small (16-255):  4-way NEON, minimal prefetch
 * - Medium (256-8K): 16-way NEON, aggressive prefetch
 * - Large (>8K):     16-way + cache blocking + multi-level prefetch
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

// Cache block size for large data (fits in L1 cache)
#define CACHE_BLOCK_SIZE 2048

int birm_cvmulv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    const float *src1 = x1;
    const float *src2 = x2;
    float *dst = y;
    int n = nx;

    // ========== Large data path (>8K): Cache-aware processing ==========
    if (n > 8192)
    {
        while (n >= CACHE_BLOCK_SIZE)
        {
            // Prefetch next cache block to L2
            __builtin_prefetch(src1 + 2 * CACHE_BLOCK_SIZE, 0, 2);
            __builtin_prefetch(src2 + CACHE_BLOCK_SIZE, 0, 2);

            int block_n = CACHE_BLOCK_SIZE;
            while (block_n >= 16)
            {
                asm volatile(
                    "prfm   pldl1strm, [%[src1], #256]   \n\t"
                    "prfm   pldl1strm, [%[src2], #128]   \n\t"

                    "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src2]], #64 \n\t"

                    "zip1   v4.4s, v0.4s, v0.4s          \n\t"
                    "zip2   v5.4s, v0.4s, v0.4s          \n\t"
                    "zip1   v6.4s, v1.4s, v1.4s          \n\t"
                    "zip2   v7.4s, v1.4s, v1.4s          \n\t"
                    "zip1   v8.4s, v2.4s, v2.4s          \n\t"
                    "zip2   v9.4s, v2.4s, v2.4s          \n\t"
                    "zip1   v10.4s, v3.4s, v3.4s         \n\t"
                    "zip2   v11.4s, v3.4s, v3.4s         \n\t"

                    "ld1    {v12.4s, v13.4s, v14.4s, v15.4s}, [%[src1]], #64 \n\t"
                    "ld1    {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src1]], #64 \n\t"

                    "fmul   v20.4s, v12.4s, v4.4s        \n\t"
                    "fmul   v21.4s, v13.4s, v5.4s        \n\t"
                    "fmul   v22.4s, v14.4s, v6.4s        \n\t"
                    "fmul   v23.4s, v15.4s, v7.4s        \n\t"
                    "fmul   v24.4s, v16.4s, v8.4s        \n\t"
                    "fmul   v25.4s, v17.4s, v9.4s        \n\t"
                    "fmul   v26.4s, v18.4s, v10.4s       \n\t"
                    "fmul   v27.4s, v19.4s, v11.4s       \n\t"

                    "st1    {v20.4s, v21.4s, v22.4s, v23.4s}, [%[dst]], #64 \n\t"
                    "st1    {v24.4s, v25.4s, v26.4s, v27.4s}, [%[dst]], #64 \n\t"

                    : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst)
                    :
                    : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
                      "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
                      "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
                      "v24", "v25", "v26", "v27");
                block_n -= 16;
            }
            n -= CACHE_BLOCK_SIZE;
        }
    }

    // ========== Medium data path (16-8K): Standard 16-way NEON ==========
    while (n >= 16)
    {
        asm volatile(
            "prfm   pldl1keep, [%[src1], #256]   \n\t"
            "prfm   pldl1keep, [%[src2], #128]   \n\t"

            "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src2]], #64 \n\t"

            "zip1   v4.4s, v0.4s, v0.4s          \n\t"
            "zip2   v5.4s, v0.4s, v0.4s          \n\t"
            "zip1   v6.4s, v1.4s, v1.4s          \n\t"
            "zip2   v7.4s, v1.4s, v1.4s          \n\t"
            "zip1   v8.4s, v2.4s, v2.4s          \n\t"
            "zip2   v9.4s, v2.4s, v2.4s          \n\t"
            "zip1   v10.4s, v3.4s, v3.4s         \n\t"
            "zip2   v11.4s, v3.4s, v3.4s         \n\t"

            "ld1    {v12.4s, v13.4s, v14.4s, v15.4s}, [%[src1]], #64 \n\t"
            "ld1    {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src1]], #64 \n\t"

            "fmul   v20.4s, v12.4s, v4.4s        \n\t"
            "fmul   v21.4s, v13.4s, v5.4s        \n\t"
            "fmul   v22.4s, v14.4s, v6.4s        \n\t"
            "fmul   v23.4s, v15.4s, v7.4s        \n\t"
            "fmul   v24.4s, v16.4s, v8.4s        \n\t"
            "fmul   v25.4s, v17.4s, v9.4s        \n\t"
            "fmul   v26.4s, v18.4s, v10.4s       \n\t"
            "fmul   v27.4s, v19.4s, v11.4s       \n\t"

            "st1    {v20.4s, v21.4s, v22.4s, v23.4s}, [%[dst]], #64 \n\t"
            "st1    {v24.4s, v25.4s, v26.4s, v27.4s}, [%[dst]], #64 \n\t"

            : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst)
            :
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
              "v24", "v25", "v26", "v27");
        n -= 16;
    }

    // ========== Small data path (4-15): 4-way NEON ==========
    while (n >= 4)
    {
        asm volatile(
            "ld1    {v0.4s}, [%[src2]], #16      \n\t"
            "ld1    {v1.4s, v2.4s}, [%[src1]], #32 \n\t"

            "zip1   v3.4s, v0.4s, v0.4s          \n\t"
            "zip2   v4.4s, v0.4s, v0.4s          \n\t"

            "fmul   v5.4s, v1.4s, v3.4s          \n\t"
            "fmul   v6.4s, v2.4s, v4.4s          \n\t"

            "st1    {v5.4s, v6.4s}, [%[dst]], #32 \n\t"

            : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst)
            :
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6");
        n -= 4;
    }

    // ========== Tiny data path (1-3): Scalar ==========
    while (n > 0)
    {
        float real = *src1++;
        float imag = *src1++;
        float scalar = *src2++;
        *dst++ = real * scalar;
        *dst++ = imag * scalar;
        n--;
    }

    return birmSuccess;
}