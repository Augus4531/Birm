/**
 * @file birm_vmulv_sf.c
 * @brief Vector multiply vector with stride optimized
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vmulv_sf(const float *__restrict__ x1, const int sa1,
                  const float *__restrict__ x2, const int sa2,
                  const int nx, float *__restrict__ y, const int sb)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa1 <= 0 || sa2 <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    int i = 0;

    // =========================================================
    // Case 1: 全连续 (sa1=1, sa2=1, sb=1) - 内联汇编
    // =========================================================
    if (sa1 == 1 && sa2 == 1 && sb == 1)
    {
        if (nx >= 32)
        {
            const float *p1 = x1;
            const float *p2 = x2;
            float *py = y;
            int loop_count = nx / 32;

            __asm__ __volatile__(
                ".align 4                           \n"
                "1:                                 \n"
                "prfm   pldl1strm, [%[p1], #256]    \n"
                "prfm   pldl1strm, [%[p2], #256]    \n"
                "prfm   pstl1strm, [%[py], #256]    \n"

                // 加载 x1 (32 floats = 8 vectors)
                "ldp    q0, q1, [%[p1]]             \n"
                "ldp    q2, q3, [%[p1], #32]        \n"
                "ldp    q4, q5, [%[p1], #64]        \n"
                "ldp    q6, q7, [%[p1], #96]        \n"

                // 加载 x2 并乘法 (交错执行)
                "ldp    q16, q17, [%[p2]]           \n"
                "fmul   v0.4s, v0.4s, v16.4s        \n"
                "fmul   v1.4s, v1.4s, v17.4s        \n"

                "ldp    q18, q19, [%[p2], #32]      \n"
                "fmul   v2.4s, v2.4s, v18.4s        \n"
                "fmul   v3.4s, v3.4s, v19.4s        \n"

                "ldp    q20, q21, [%[p2], #64]      \n"
                "fmul   v4.4s, v4.4s, v20.4s        \n"
                "fmul   v5.4s, v5.4s, v21.4s        \n"

                "ldp    q22, q23, [%[p2], #96]      \n"
                "fmul   v6.4s, v6.4s, v22.4s        \n"
                "fmul   v7.4s, v7.4s, v23.4s        \n"

                // 存储结果
                "stp    q0, q1, [%[py]]             \n"
                "stp    q2, q3, [%[py], #32]        \n"
                "stp    q4, q5, [%[py], #64]        \n"
                "stp    q6, q7, [%[py], #96]        \n"

                "add    %[p1], %[p1], #128          \n"
                "add    %[p2], %[p2], #128          \n"
                "add    %[py], %[py], #128          \n"
                "subs   %w[cnt], %w[cnt], #1        \n"
                "b.gt   1b                          \n"

                : [p1] "+r"(p1), [p2] "+r"(p2), [py] "+r"(py), [cnt] "+r"(loop_count)
                :
                : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
                  "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
                  "memory", "cc");

            i = (nx / 32) * 32;
        }

        // 8个float
        for (; i <= nx - 8; i += 8)
        {
            float32x4_t a0 = vld1q_f32(&x1[i]);
            float32x4_t a1 = vld1q_f32(&x1[i + 4]);
            float32x4_t b0 = vld1q_f32(&x2[i]);
            float32x4_t b1 = vld1q_f32(&x2[i + 4]);
            vst1q_f32(&y[i], vmulq_f32(a0, b0));
            vst1q_f32(&y[i + 4], vmulq_f32(a1, b1));
        }

        // 4个float
        for (; i <= nx - 4; i += 4)
        {
            vst1q_f32(&y[i], vmulq_f32(vld1q_f32(&x1[i]), vld1q_f32(&x2[i])));
        }

        // 标量尾部
        for (; i < nx; i++)
        {
            y[i] = x1[i] * x2[i];
        }
    }
    // =========================================================
    // Case 2: 输入步长2，输出连续 (sa1=2, sa2=2, sb=1)
    // =========================================================
    else if (sa1 == 2 && sa2 == 2 && sb == 1)
    {
        for (; i <= nx - 8; i += 8)
        {
            float32x4x2_t d1_0 = vld2q_f32(&x1[i * 2]);
            float32x4x2_t d2_0 = vld2q_f32(&x2[i * 2]);
            float32x4x2_t d1_1 = vld2q_f32(&x1[i * 2 + 8]);
            float32x4x2_t d2_1 = vld2q_f32(&x2[i * 2 + 8]);

            vst1q_f32(&y[i], vmulq_f32(d1_0.val[0], d2_0.val[0]));
            vst1q_f32(&y[i + 4], vmulq_f32(d1_1.val[0], d2_1.val[0]));
        }

        for (; i < nx; i++)
        {
            y[i] = x1[i * 2] * x2[i * 2];
        }
    }
    // =========================================================
    // Case 3: 通用路径
    // =========================================================
    else
    {
        const int sa1_4 = sa1 * 4;
        const int sa2_4 = sa2 * 4;
        const int sb_4 = sb * 4;

        const float *p1 = x1;
        const float *p2 = x2;
        float *py = y;

        for (; i <= nx - 4; i += 4)
        {
            py[0] = p1[0] * p2[0];
            py[sb] = p1[sa1] * p2[sa2];
            py[sb * 2] = p1[sa1 * 2] * p2[sa2 * 2];
            py[sb * 3] = p1[sa1 * 3] * p2[sa2 * 3];

            p1 += sa1_4;
            p2 += sa2_4;
            py += sb_4;
        }

        for (; i < nx; i++)
        {
            *py = (*p1) * (*p2);
            p1 += sa1;
            p2 += sa2;
            py += sb;
        }
    }

    return birmSuccess;
}