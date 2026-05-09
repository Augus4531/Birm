/**
 * @brief Vector add scalar - Ultra-optimized with OpenMP and NEON
 *
 * @param x1 Input vector
 * @param x2 Scalar value to add
 * @param nx Vector length
 * @param y Output vector
 * @return 0 for success, -1 for error
 *
 * Formula: y[i] = x1[i] + x2
 *
 */
#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vadds_f(const float *__restrict__ x1, const float x2, const int nx, float *__restrict__ y)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    const float *src = x1;
    float *dst = y;
    int count = nx;
    float32x4_t vscalar = vdupq_n_f32(x2);

    // =========================================================
    // 1. 小数据量优化 (nx < 32k)
    //    特点：数据在 Cache 中，瓶颈是指令延迟。
    //    优化：增加展开度到 64，使用 Intrinsic。
    // =========================================================
    if (nx < 32768)
    {
        // 头部对齐 (Alignment Peeling)
        while (count > 0 && ((uintptr_t)dst & 0xF))
        {
            *dst++ = *src++ + x2;
            count--;
        }

        // 64路循环展开 (256 字节/次)
        while (count >= 64)
        {
            // 1. Load (利用多端口发射)
            float32x4_t v0 = vld1q_f32(src);
            float32x4_t v1 = vld1q_f32(src + 4);
            float32x4_t v2 = vld1q_f32(src + 8);
            float32x4_t v3 = vld1q_f32(src + 12);
            float32x4_t v4 = vld1q_f32(src + 16);
            float32x4_t v5 = vld1q_f32(src + 20);
            float32x4_t v6 = vld1q_f32(src + 24);
            float32x4_t v7 = vld1q_f32(src + 28);

            float32x4_t v8 = vld1q_f32(src + 32);
            float32x4_t v9 = vld1q_f32(src + 36);
            float32x4_t v10 = vld1q_f32(src + 40);
            float32x4_t v11 = vld1q_f32(src + 44);
            float32x4_t v12 = vld1q_f32(src + 48);
            float32x4_t v13 = vld1q_f32(src + 52);
            float32x4_t v14 = vld1q_f32(src + 56);
            float32x4_t v15 = vld1q_f32(src + 60);

            // 2. Compute & Store (Add Scalar)
            vst1q_f32(dst, vaddq_f32(v0, vscalar));
            vst1q_f32(dst + 4, vaddq_f32(v1, vscalar));
            vst1q_f32(dst + 8, vaddq_f32(v2, vscalar));
            vst1q_f32(dst + 12, vaddq_f32(v3, vscalar));
            vst1q_f32(dst + 16, vaddq_f32(v4, vscalar));
            vst1q_f32(dst + 20, vaddq_f32(v5, vscalar));
            vst1q_f32(dst + 24, vaddq_f32(v6, vscalar));
            vst1q_f32(dst + 28, vaddq_f32(v7, vscalar));

            vst1q_f32(dst + 32, vaddq_f32(v8, vscalar));
            vst1q_f32(dst + 36, vaddq_f32(v9, vscalar));
            vst1q_f32(dst + 40, vaddq_f32(v10, vscalar));
            vst1q_f32(dst + 44, vaddq_f32(v11, vscalar));
            vst1q_f32(dst + 48, vaddq_f32(v12, vscalar));
            vst1q_f32(dst + 52, vaddq_f32(v13, vscalar));
            vst1q_f32(dst + 56, vaddq_f32(v14, vscalar));
            vst1q_f32(dst + 60, vaddq_f32(v15, vscalar));

            src += 64;
            dst += 64;
            count -= 64;
        }

        // Tail Loop (4 floats)
        while (count >= 4)
        {
            vst1q_f32(dst, vaddq_f32(vld1q_f32(src), vscalar));
            src += 4;
            dst += 4;
            count -= 4;
        }

        // Scalar Tail
        while (count-- > 0)
        {
            *dst++ = *src++ + x2;
        }
    }
    // =========================================================
    // 2. 大数据量 (nx >= 32k)
    // =========================================================
    else
    {
        // Main loop: process 16 floats (4 vectors) per iteration
        while (count >= 16)
        {
            __asm__ volatile(
                "prfm   pldl1keep, [%[src], #256]   \n\t"                 // Prefetch ahead
                "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t" // Load 4 vectors
                "fadd   v0.4s, v0.4s, %[vval].4s    \n\t"                 // Add scalar
                "fadd   v1.4s, v1.4s, %[vval].4s    \n\t"
                "fadd   v2.4s, v2.4s, %[vval].4s    \n\t"
                "fadd   v3.4s, v3.4s, %[vval].4s    \n\t"
                "st1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[dst]], #64 \n\t" // Store result
                : [src] "+r"(src), [dst] "+r"(dst)
                : [vval] "w"(vscalar)
                : "memory", "v0", "v1", "v2", "v3");
            count -= 16;
        }

        // Handle remaining elements using Assembly (Vectorized Tail)
        if (count > 0)
        {
            __asm__ volatile(
                "tbz    %[n], #3, 2f                \n\t"
                "ld1    {v0.4s, v1.4s}, [%[src]], #32 \n\t"
                "fadd   v0.4s, v0.4s, %[vval].4s    \n\t"
                "fadd   v1.4s, v1.4s, %[vval].4s    \n\t"
                "st1    {v0.4s, v1.4s}, [%[dst]], #32 \n\t"
                "2:                                 \n\t"

                "tbz    %[n], #2, 3f                \n\t"
                "ld1    {v0.4s}, [%[src]], #16      \n\t"
                "fadd   v0.4s, v0.4s, %[vval].4s    \n\t"
                "st1    {v0.4s}, [%[dst]], #16      \n\t"
                "3:                                 \n\t"

                "tbz    %[n], #1, 4f                \n\t"
                "ld1    {v0.2s}, [%[src]], #8       \n\t"
                "fadd   v0.2s, v0.2s, %[vval].2s    \n\t"
                "st1    {v0.2s}, [%[dst]], #8       \n\t"
                "4:                                 \n\t"

                "tbz    %[n], #0, 5f                \n\t"
                "ld1    {v0.s}[0], [%[src]]         \n\t"
                "fadd   s0, s0, %s[vval]            \n\t"
                "st1    {v0.s}[0], [%[dst]]         \n\t"
                "5:                                 \n\t"

                : [src] "+r"(src), [dst] "+r"(dst)
                : [n] "r"(count), [vval] "w"(vscalar)
                : "cc", "memory", "v0", "v1");
        }
    }

    return birmSuccess;
}