/**
 * @file birm_vfill_f.c
 * @brief Vector fill - Ultimate optimization
 *        Features: DC ZVA for zero-fill, STNP for large data, aggressive unrolling
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <stdint.h>

// 大规模数据阈值
#define LARGE_DATA_THRESHOLD 65536

// DC ZVA 块大小
#define DC_ZVA_BLOCK_SIZE 64

int birm_vfill_f(const float x, const int nx, float *y)
{
    if (!y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int idx = 0;

    // ========== 特殊优化：零值填充使用 DC ZVA ==========
    if (x == 0.0f && nx >= 256)
    {
        // 先处理到64字节对齐
        uintptr_t addr = (uintptr_t)y;
        while ((addr & 63) != 0 && idx < nx)
        {
            y[idx++] = 0.0f;
            addr += 4;
        }

        // DC ZVA: 每次清零64字节(16个float)
        float *p = &y[idx];
        int zva_count = (nx - idx) / 16;

        if (zva_count > 0)
        {
            __asm__ __volatile__(
                ".align 4                      \n"
                "1:                            \n"
                "dc     zva, %[p]              \n"
                "add    %[p], %[p], #64        \n"
                "subs   %w[cnt], %w[cnt], #1   \n"
                "b.gt   1b                     \n"
                : [p] "+r"(p), [cnt] "+r"(zva_count)
                :
                : "memory", "cc");
            idx += zva_count * 16;
        }

        // 处理剩余元素
        for (; idx < nx; idx++)
        {
            y[idx] = 0.0f;
        }
        return birmSuccess;
    }

    // ========== 大规模非零数据：STNP + 128元素展开 ==========
    if (nx >= LARGE_DATA_THRESHOLD)
    {
        float *p = y;
        int loop_count = nx / 128; // 每次128个元素 = 512字节

        __asm__ __volatile__(
            // 广播标量到所有寄存器
            "dup    v0.4s, %w[val]         \n"
            "mov    v1.16b, v0.16b         \n"
            "mov    v2.16b, v0.16b         \n"
            "mov    v3.16b, v0.16b         \n"
            "mov    v4.16b, v0.16b         \n"
            "mov    v5.16b, v0.16b         \n"
            "mov    v6.16b, v0.16b         \n"
            "mov    v7.16b, v0.16b         \n"

            ".align 4                      \n"
            "1:                            \n"

            // 128元素 = 512字节，使用16次STNP
            "stnp   q0, q1, [%[p]]         \n"
            "stnp   q2, q3, [%[p], #32]    \n"
            "stnp   q4, q5, [%[p], #64]    \n"
            "stnp   q6, q7, [%[p], #96]    \n"
            "stnp   q0, q1, [%[p], #128]   \n"
            "stnp   q2, q3, [%[p], #160]   \n"
            "stnp   q4, q5, [%[p], #192]   \n"
            "stnp   q6, q7, [%[p], #224]   \n"
            "stnp   q0, q1, [%[p], #256]   \n"
            "stnp   q2, q3, [%[p], #288]   \n"
            "stnp   q4, q5, [%[p], #320]   \n"
            "stnp   q6, q7, [%[p], #352]   \n"
            "stnp   q0, q1, [%[p], #384]   \n"
            "stnp   q2, q3, [%[p], #416]   \n"
            "stnp   q4, q5, [%[p], #448]   \n"
            "stnp   q6, q7, [%[p], #480]   \n"

            "add    %[p], %[p], #512       \n"
            "subs   %w[cnt], %w[cnt], #1   \n"
            "b.gt   1b                     \n"

            : [p] "+r"(p), [cnt] "+r"(loop_count)
            : [val] "r"(x)
            : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "memory", "cc");

        idx = (nx / 128) * 128;
    }

    // ========== 中小规模 + 剩余处理 ==========
    float32x4_t vx = vdupq_n_f32(x);

    // 64路展开
    for (; idx <= nx - 64; idx += 64)
    {
        vst1q_f32(&y[idx], vx);
        vst1q_f32(&y[idx + 4], vx);
        vst1q_f32(&y[idx + 8], vx);
        vst1q_f32(&y[idx + 12], vx);
        vst1q_f32(&y[idx + 16], vx);
        vst1q_f32(&y[idx + 20], vx);
        vst1q_f32(&y[idx + 24], vx);
        vst1q_f32(&y[idx + 28], vx);
        vst1q_f32(&y[idx + 32], vx);
        vst1q_f32(&y[idx + 36], vx);
        vst1q_f32(&y[idx + 40], vx);
        vst1q_f32(&y[idx + 44], vx);
        vst1q_f32(&y[idx + 48], vx);
        vst1q_f32(&y[idx + 52], vx);
        vst1q_f32(&y[idx + 56], vx);
        vst1q_f32(&y[idx + 60], vx);
    }

    // 16路展开
    for (; idx <= nx - 16; idx += 16)
    {
        vst1q_f32(&y[idx], vx);
        vst1q_f32(&y[idx + 4], vx);
        vst1q_f32(&y[idx + 8], vx);
        vst1q_f32(&y[idx + 12], vx);
    }

    // 4路
    for (; idx <= nx - 4; idx += 4)
    {
        vst1q_f32(&y[idx], vx);
    }

    // 标量尾部
    for (; idx < nx; idx++)
    {
        y[idx] = x;
    }

    return birmSuccess;
}