/**
 * @brief Vector multiply scalar - 32-way unrolling NEON optimized
 *
 * @param x1  Input vector pointer
 * @param x2  Scalar value
 * @param nx  Vector length
 * @param y   Output vector pointer
 * @return int 0 for success, -1 for error
 *
 * Formula: y[i] = x1[i] * x2
 *
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
int birm_vmuls_f(const float *__restrict__ x1, const float x2, const int nx, float *__restrict__ y)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    const float *src = x1;
    float *dst = y;
    int count = nx;

    // ---------------------------------------------------------
    // 1. 小数据量 (nx <= 32000)
    //    数据在 L1/L2 Cache 中，使用普通 NEON Intrinsic
    // ---------------------------------------------------------
    if (nx <= 32768)
    {
        float32x4_t vscalar = vdupq_n_f32(x2);

        // 处理头部不对齐 (保证写入地址 128-bit 对齐)
        while (count > 0 && ((uintptr_t)dst & 0xF))
        {
            *dst++ = *src++ * x2;
            count--;
        }

        // 主循环：每次处理 16 个 float (4个向量)
        while (count >= 16)
        {
            float32x4_t v0 = vld1q_f32(src);
            float32x4_t v1 = vld1q_f32(src + 4);
            float32x4_t v2 = vld1q_f32(src + 8);
            float32x4_t v3 = vld1q_f32(src + 12);

            vst1q_f32(dst, vmulq_f32(v0, vscalar));
            vst1q_f32(dst + 4, vmulq_f32(v1, vscalar));
            vst1q_f32(dst + 8, vmulq_f32(v2, vscalar));
            vst1q_f32(dst + 12, vmulq_f32(v3, vscalar));

            src += 16;
            dst += 16;
            count -= 16;
        }

        // 处理剩余的向量 (4个一组)
        while (count >= 4)
        {
            vst1q_f32(dst, vmulq_f32(vld1q_f32(src), vscalar));
            src += 4;
            dst += 4;
            count -= 4;
        }
        // 剩余的 0-3 个元素会落入最后的标量尾部处理
    }
    // ---------------------------------------------------------
    // 2. 大数据量 (nx > 32768)
    //    数据溢出到 DDR，使用 STNP 绕过 Cache 分配 (Zero-Copy)
    // ---------------------------------------------------------
    else
    {
        float32x4_t vscalar = vdupq_n_f32(x2);

        // 【优化1】Alignment Peeling (对齐剥离)
        // 强制 dst 对齐到 64 字节 (Cache Line)，这是 STNP 生效的关键
        while (count > 0 && ((uintptr_t)dst & 63))
        {
            *dst++ = *src++ * x2;
            count--;
        }

        // 【优化2】核心汇编循环
        // 每次处理 32 个 float (128 字节)
        while (count >= 32)
        {
            __asm__ volatile(
                // 1. Load & Prefetch
                "prfm   pldl1keep, [%[src], #512]    \n\t"
                "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
                "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src]], #64 \n\t"

                // 2. Compute
                "fmul   v0.4s, v0.4s, %[vval].4s     \n\t"
                "fmul   v1.4s, v1.4s, %[vval].4s     \n\t"
                "fmul   v2.4s, v2.4s, %[vval].4s     \n\t"
                "fmul   v3.4s, v3.4s, %[vval].4s     \n\t"
                "fmul   v4.4s, v4.4s, %[vval].4s     \n\t"
                "fmul   v5.4s, v5.4s, %[vval].4s     \n\t"
                "fmul   v6.4s, v6.4s, %[vval].4s     \n\t"
                "fmul   v7.4s, v7.4s, %[vval].4s     \n\t"

                // 3. Store using STNP (Bypass Cache)
                "stnp   q0, q1, [%[dst]]             \n\t"
                "stnp   q2, q3, [%[dst], #32]        \n\t"
                "stnp   q4, q5, [%[dst], #64]        \n\t"
                "stnp   q6, q7, [%[dst], #96]        \n\t"

                "add    %[dst], %[dst], #128         \n\t"

                : [src] "+r"(src), [dst] "+r"(dst)
                : [vval] "w"(vscalar)
                : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");

            count -= 32;
        }

        // 【优化3】汇编尾部处理 (处理 < 32 的剩余元素)
        if (count > 0)
        {
            __asm__ volatile(
                "tbz    %[n], #4, 1f                \n\t"
                "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
                "fmul   v0.4s, v0.4s, %[vval].4s    \n\t"
                "fmul   v1.4s, v1.4s, %[vval].4s    \n\t"
                "fmul   v2.4s, v2.4s, %[vval].4s    \n\t"
                "fmul   v3.4s, v3.4s, %[vval].4s    \n\t"
                "st1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[dst]], #64 \n\t"
                "1:                                 \n\t"

                "tbz    %[n], #3, 2f                \n\t"
                "ld1    {v0.4s, v1.4s}, [%[src]], #32 \n\t"
                "fmul   v0.4s, v0.4s, %[vval].4s    \n\t"
                "fmul   v1.4s, v1.4s, %[vval].4s    \n\t"
                "st1    {v0.4s, v1.4s}, [%[dst]], #32 \n\t"
                "2:                                 \n\t"

                "tbz    %[n], #2, 3f                \n\t"
                "ld1    {v0.4s}, [%[src]], #16      \n\t"
                "fmul   v0.4s, v0.4s, %[vval].4s    \n\t"
                "st1    {v0.4s}, [%[dst]], #16      \n\t"
                "3:                                 \n\t"

                "tbz    %[n], #1, 4f                \n\t"
                "ld1    {v0.2s}, [%[src]], #8       \n\t"
                "fmul   v0.2s, v0.2s, %[vval].2s    \n\t"
                "st1    {v0.2s}, [%[dst]], #8       \n\t"
                "4:                                 \n\t"

                "tbz    %[n], #0, 5f                \n\t"
                "ld1    {v0.s}[0], [%[src]]         \n\t"
                "fmul   s0, s0, %s[vval]            \n\t"
                "st1    {v0.s}[0], [%[dst]]         \n\t"
                "5:                                 \n\t"

                : [src] "+r"(src), [dst] "+r"(dst)
                : [n] "r"(count), [vval] "w"(vscalar)
                : "cc", "memory", "v0", "v1", "v2", "v3");

            // Count 清零
            // 防止跳出 if 后，下方的 while(count-->0) 再次执行导致堆溢出
            count = 0;
        }
    }

    // 标量尾部 (处理剩余)
    while (count-- > 0)
    {
        *dst++ = *src++ * x2;
    }

    return birmSuccess;
}