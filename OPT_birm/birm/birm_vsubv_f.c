/**
 * @brief Vector subtract vector - 64-way unrolling NEON optimized
 *
 * @param x1  Input vector pointer 1
 * @param x2  Input vector pointer 2
 * @param nx  Vector length
 * @param y   Output vector pointer
 * @return int 0 for success, -1 for error
 *
 * Formula: y[i] = x1[i] - x2[i]
 *
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <stdint.h>

int birm_vsubv_f(const float *__restrict x1,
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
    // 小数据量：直接使用编译器优化的 NEON Intrinsics
    // ---------------------------------------------------------
    if (nx < 32768)
    {
        int idx = 0;
        // 16路展开 (64字节/次)
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

            vst1q_f32(&y[idx], vsubq_f32(v1_0, v2_0));
            vst1q_f32(&y[idx + 4], vsubq_f32(v1_1, v2_1));
            vst1q_f32(&y[idx + 8], vsubq_f32(v1_2, v2_2));
            vst1q_f32(&y[idx + 12], vsubq_f32(v1_3, v2_3));
        }
        // 处理剩余
        for (; idx < nx; idx++)
        {
            y[idx] = x1[idx] - x2[idx];
        }
        return 0;
    }

    // ---------------------------------------------------------
    // 大数据量 (>= 32768)：内联汇编深度优化
    // ---------------------------------------------------------
    const float *src1 = x1;
    const float *src2 = x2;
    float *dst = y;
    int count = nx;

    // 1. 内存对齐处理 (Align dst to 16-byte/128-bit boundary for STP)
    while (count > 0 && ((uintptr_t)dst & 15))
    {
        *dst++ = *src1++ - *src2++;
        count--;
    }

    // 2. 主循环：每次处理 32 个 float (128 字节)
    int loop_cnt = count / 32;
    int remain = count % 32; // 计算出剩余个数，用于尾部

    if (loop_cnt > 0)
    {
        __asm__ volatile(
            "1:                                 \n\t"
            // --- 预取 (Prefetch) ---
            "prfm   pldl1keep, [%[src1], #256]  \n\t" // 预取 src1
            "prfm   pldl1keep, [%[src2], #256]  \n\t" // 预取 src2

            // --- 加载数据 (Load 128 Bytes total) ---
            // 使用 Post-index 自动更新指针 src1/src2
            // Group 1: 16 floats
            "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src1]], #64 \n\t"
            "ld1    {v16.4s, v17.4s, v18.4s, v19.4s}, [%[src2]], #64 \n\t"

            // Group 2: 16 floats
            "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src1]], #64 \n\t"
            "ld1    {v20.4s, v21.4s, v22.4s, v23.4s}, [%[src2]], #64 \n\t"

            // --- 计算 (Subtract) ---
            "fsub   v0.4s, v0.4s, v16.4s        \n\t"
            "fsub   v1.4s, v1.4s, v17.4s        \n\t"
            "fsub   v2.4s, v2.4s, v18.4s        \n\t"
            "fsub   v3.4s, v3.4s, v19.4s        \n\t"

            "fsub   v4.4s, v4.4s, v20.4s        \n\t"
            "fsub   v5.4s, v5.4s, v21.4s        \n\t"
            "fsub   v6.4s, v6.4s, v22.4s        \n\t"
            "fsub   v7.4s, v7.4s, v23.4s        \n\t"

            // --- 存储 (Store) ---
            // 改用 stp (Store Pair)，写入 Cache，避免一致性错误
            // stp 每次存2个q寄存器(32字节)，所以这里手动计算偏移
            "stp    q0, q1, [%[dst]]            \n\t" // 偏移 0
            "stp    q2, q3, [%[dst], #32]       \n\t" // 偏移 32
            "stp    q4, q5, [%[dst], #64]       \n\t" // 偏移 64
            "stp    q6, q7, [%[dst], #96]       \n\t" // 偏移 96

            // 更新 dst 指针和循环计数
            "add    %[dst], %[dst], #128        \n\t"
            "subs   %[cnt], %[cnt], #1          \n\t"
            "b.ne   1b                          \n\t"

            // 输出操作数约束：确保 src1, src2, dst 在汇编结束后被更新回 C变量
            : [src1] "+r"(src1), [src2] "+r"(src2), [dst] "+r"(dst), [cnt] "+r"(loop_cnt)
            :
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23");
    }

    // 3. 尾部处理 (Tail Handling)
    // 使用 C 语言处理剩余的 0~31 个点
    // 这里的 remain 是之前计算好的 count % 32
    if (remain > 0)
    {
        // 如果剩余超过16个，先用 NEON 处理
        while (remain >= 4)
        {
            vst1q_f32(dst, vsubq_f32(vld1q_f32(src1), vld1q_f32(src2)));
            src1 += 4;
            src2 += 4;
            dst += 4;
            remain -= 4;
        }
        // 处理最后的 1-3 个
        while (remain > 0)
        {
            *dst++ = *src1++ - *src2++;
            remain--;
        }
    }

    return birmSuccess;
}