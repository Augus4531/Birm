/**
 * @brief Real vector element-wise multiply - NEON optimized
 *
 * @param x1  Input vector pointer x1
 * @param x2  Input vector pointer x2
 * @param nx  Input vector length
 * @param y   Output vector pointer y
 * @return int 0 for success, -1 for error
 *
 * Formula: y[i] = x1[i] * x2[i], for i = 0, 1, ..., nx-1
 *
 */
#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
#include <stdint.h>

int birm_vmulv_f(const float *__restrict x1,
                 const float *__restrict x2,
                 const int nx,
                 float *__restrict y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // ---------------------------------------------------------
    // Case 1: 小数据量
    // ---------------------------------------------------------
    if (nx < 32768)
    {
        // 16路展开
        int idx = 0;
        for (; idx <= nx - 16; idx += 16)
        {
            float32x4_t v1_0 = vld1q_f32(&x1[idx]);
            float32x4_t v2_0 = vld1q_f32(&x2[idx]);
            float32x4_t v1_1 = vld1q_f32(&x1[idx + 4]);
            float32x4_t v2_1 = vld1q_f32(&x2[idx + 4]);
            float32x4_t v1_2 = vld1q_f32(&x1[idx + 8]);
            float32x4_t v2_2 = vld1q_f32(&x2[idx + 8]);
            float32x4_t v1_3 = vld1q_f32(&x1[idx + 12]);
            float32x4_t v2_3 = vld1q_f32(&x2[idx + 12]);

            vst1q_f32(&y[idx], vmulq_f32(v1_0, v2_0));
            vst1q_f32(&y[idx + 4], vmulq_f32(v1_1, v2_1));
            vst1q_f32(&y[idx + 8], vmulq_f32(v1_2, v2_2));
            vst1q_f32(&y[idx + 12], vmulq_f32(v1_3, v2_3));
        }
        // 处理剩余
        for (; idx < nx; idx++)
        {
            y[idx] = x1[idx] * x2[idx];
        }
        return birmSuccess;
    }

    // ---------------------------------------------------------
    // Case 2: 大数据量
    // ---------------------------------------------------------
    const float *src1 = x1;
    const float *src2 = x2;
    float *dst = y;
    int count = nx;

    // 1. 对齐优化 (Align dst to 16-byte boundary)
    while (count > 0 && ((uintptr_t)dst & 15))
    {
        *dst++ = *src1++ * *src2++;
        count--;
    }

    // 2. 主循环：每次处理 32 floats (128 bytes)
    int loop_cnt = count / 32;
    int remain = count % 32;

    if (loop_cnt > 0)
    {
        __asm__ volatile(
            "1:                                 \n\t"
            // --- Prefetch (预取距离 256-512 字节) ---
            "prfm   pldl1keep, [%[src1], #256]  \n\t"
            "prfm   pldl1keep, [%[src2], #256]  \n\t"

            // --- Load Block A (v0-v3, v16-v19) ---
            "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src1]], #64 \n\t"
            "ld1    {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src2]], #64 \n\t"

            // --- Load Block B (v4-v7, v20-v23) ---
            // 立即加载下一块，利用并发能力
            "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src1]], #64 \n\t"
            "ld1    {v20.4s, v21.4s, v22.4s, v23.4s}, [%[src2]], #64 \n\t"

            // --- Calc Block A (Hide Block B Load Latency) ---
            "fmul   v0.4s, v0.4s, v16.4s        \n\t"
            "fmul   v1.4s, v1.4s, v17.4s        \n\t"
            "fmul   v2.4s, v2.4s, v18.4s        \n\t"
            "fmul   v3.4s, v3.4s, v19.4s        \n\t"

            // --- Store Block A (使用 STP 替代 ST1) ---
            // 写入 A 组结果，此时流水线继续执行 B 组计算
            "stp    q0, q1, [%[dst]]            \n\t"
            "stp    q2, q3, [%[dst], #32]       \n\t"

            // --- Calc Block B (Hide Block A Store Latency) ---
            "fmul   v4.4s, v4.4s, v20.4s        \n\t"
            "fmul   v5.4s, v5.4s, v21.4s        \n\t"
            "fmul   v6.4s, v6.4s, v22.4s        \n\t"
            "fmul   v7.4s, v7.4s, v23.4s        \n\t"

            // --- Store Block B ---
            "stp    q4, q5, [%[dst], #64]       \n\t"
            "stp    q6, q7, [%[dst], #96]       \n\t"

            "add    %[dst], %[dst], #128        \n\t" // 更新 dst 指针
            "subs   %[cnt], %[cnt], #1          \n\t" // 循环计数
            "b.ne   1b                          \n\t"

            // 约束更新后的指针，供后续 C 代码使用
            : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst), [cnt] "+r"(loop_cnt)
            :
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");
    }

    // 3. 尾部处理 (C语言实现，更安全)
    // 处理剩余的 0-31 个元素
    if (remain > 0)
    {
        // 如果剩余的大于等于4个，用 Intrinsics 向量处理
        while (remain >= 4)
        {
            vst1q_f32(dst, vmulq_f32(vld1q_f32(src1), vld1q_f32(src2)));
            src1 += 4;
            src2 += 4;
            dst += 4;
            remain -= 4;
        }
        // 处理最后 1-3 个
        while (remain > 0)
        {
            *dst++ = *src1++ * *src2++;
            remain--;
        }
    }

    return birmSuccess;
}