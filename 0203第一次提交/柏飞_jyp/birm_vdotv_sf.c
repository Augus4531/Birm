/**
 * @brief Vector dot product with stride - NEON optimized
 * @note Final Corrected Logic: 'nx' is INPUT length. Processing length is nx/sa.
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include "armpl.h"
int birm_vdotv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    if (nx <= 262144)
    {
        float sum = 0.0f;
        int idx = 0;

        if (sa == 1 && sb == 1)
        {
            float32x4_t vsum0 = vdupq_n_f32(0.0f);
            float32x4_t vsum1 = vdupq_n_f32(0.0f);
            float32x4_t vsum2 = vdupq_n_f32(0.0f);
            float32x4_t vsum3 = vdupq_n_f32(0.0f);

            for (; idx <= nx - 64; idx += 64)
            {
                __builtin_prefetch(&x1[idx + 64], 0, 0);
                __builtin_prefetch(&x2[idx + 64], 0, 0);

                float32x4_t v1_0 = vld1q_f32(&x1[idx]);
                float32x4_t v2_0 = vld1q_f32(&x2[idx]);
                float32x4_t v1_1 = vld1q_f32(&x1[idx + 4]);
                float32x4_t v2_1 = vld1q_f32(&x2[idx + 4]);
                float32x4_t v1_2 = vld1q_f32(&x1[idx + 8]);
                float32x4_t v2_2 = vld1q_f32(&x2[idx + 8]);
                float32x4_t v1_3 = vld1q_f32(&x1[idx + 12]);
                float32x4_t v2_3 = vld1q_f32(&x2[idx + 12]);

                vsum0 = vfmaq_f32(vsum0, v1_0, v2_0);
                vsum1 = vfmaq_f32(vsum1, v1_1, v2_1);
                vsum2 = vfmaq_f32(vsum2, v1_2, v2_2);
                vsum3 = vfmaq_f32(vsum3, v1_3, v2_3);

                float32x4_t v1_4 = vld1q_f32(&x1[idx + 16]);
                float32x4_t v2_4 = vld1q_f32(&x2[idx + 16]);
                float32x4_t v1_5 = vld1q_f32(&x1[idx + 20]);
                float32x4_t v2_5 = vld1q_f32(&x2[idx + 20]);
                float32x4_t v1_6 = vld1q_f32(&x1[idx + 24]);
                float32x4_t v2_6 = vld1q_f32(&x2[idx + 24]);
                float32x4_t v1_7 = vld1q_f32(&x1[idx + 28]);
                float32x4_t v2_7 = vld1q_f32(&x2[idx + 28]);

                vsum0 = vfmaq_f32(vsum0, v1_4, v2_4);
                vsum1 = vfmaq_f32(vsum1, v1_5, v2_5);
                vsum2 = vfmaq_f32(vsum2, v1_6, v2_6);
                vsum3 = vfmaq_f32(vsum3, v1_7, v2_7);

                float32x4_t v1_8 = vld1q_f32(&x1[idx + 32]);
                float32x4_t v2_8 = vld1q_f32(&x2[idx + 32]);
                float32x4_t v1_9 = vld1q_f32(&x1[idx + 36]);
                float32x4_t v2_9 = vld1q_f32(&x2[idx + 36]);
                float32x4_t v1_10 = vld1q_f32(&x1[idx + 40]);
                float32x4_t v2_10 = vld1q_f32(&x2[idx + 40]);
                float32x4_t v1_11 = vld1q_f32(&x1[idx + 44]);
                float32x4_t v2_11 = vld1q_f32(&x2[idx + 44]);

                vsum0 = vfmaq_f32(vsum0, v1_8, v2_8);
                vsum1 = vfmaq_f32(vsum1, v1_9, v2_9);
                vsum2 = vfmaq_f32(vsum2, v1_10, v2_10);
                vsum3 = vfmaq_f32(vsum3, v1_11, v2_11);

                float32x4_t v1_12 = vld1q_f32(&x1[idx + 48]);
                float32x4_t v2_12 = vld1q_f32(&x2[idx + 48]);
                float32x4_t v1_13 = vld1q_f32(&x1[idx + 52]);
                float32x4_t v2_13 = vld1q_f32(&x2[idx + 52]);
                float32x4_t v1_14 = vld1q_f32(&x1[idx + 56]);
                float32x4_t v2_14 = vld1q_f32(&x2[idx + 56]);
                float32x4_t v1_15 = vld1q_f32(&x1[idx + 60]);
                float32x4_t v2_15 = vld1q_f32(&x2[idx + 60]);

                vsum0 = vfmaq_f32(vsum0, v1_12, v2_12);
                vsum1 = vfmaq_f32(vsum1, v1_13, v2_13);
                vsum2 = vfmaq_f32(vsum2, v1_14, v2_14);
                vsum3 = vfmaq_f32(vsum3, v1_15, v2_15);
            }

            for (; idx <= nx - 32; idx += 32)
            {
                float32x4_t v1_0 = vld1q_f32(&x1[idx]);
                float32x4_t v2_0 = vld1q_f32(&x2[idx]);
                float32x4_t v1_1 = vld1q_f32(&x1[idx + 4]);
                float32x4_t v2_1 = vld1q_f32(&x2[idx + 4]);
                float32x4_t v1_2 = vld1q_f32(&x1[idx + 8]);
                float32x4_t v2_2 = vld1q_f32(&x2[idx + 8]);
                float32x4_t v1_3 = vld1q_f32(&x1[idx + 12]);
                float32x4_t v2_3 = vld1q_f32(&x2[idx + 12]);

                vsum0 = vfmaq_f32(vsum0, v1_0, v2_0);
                vsum1 = vfmaq_f32(vsum1, v1_1, v2_1);
                vsum2 = vfmaq_f32(vsum2, v1_2, v2_2);
                vsum3 = vfmaq_f32(vsum3, v1_3, v2_3);

                float32x4_t v1_4 = vld1q_f32(&x1[idx + 16]);
                float32x4_t v2_4 = vld1q_f32(&x2[idx + 16]);
                float32x4_t v1_5 = vld1q_f32(&x1[idx + 20]);
                float32x4_t v2_5 = vld1q_f32(&x2[idx + 20]);
                float32x4_t v1_6 = vld1q_f32(&x1[idx + 24]);
                float32x4_t v2_6 = vld1q_f32(&x2[idx + 24]);
                float32x4_t v1_7 = vld1q_f32(&x1[idx + 28]);
                float32x4_t v2_7 = vld1q_f32(&x2[idx + 28]);

                vsum0 = vfmaq_f32(vsum0, v1_4, v2_4);
                vsum1 = vfmaq_f32(vsum1, v1_5, v2_5);
                vsum2 = vfmaq_f32(vsum2, v1_6, v2_6);
                vsum3 = vfmaq_f32(vsum3, v1_7, v2_7);
            }

            for (; idx <= nx - 16; idx += 16)
            {
                vsum0 = vfmaq_f32(vsum0, vld1q_f32(&x1[idx]), vld1q_f32(&x2[idx]));
                vsum1 = vfmaq_f32(vsum1, vld1q_f32(&x1[idx + 4]), vld1q_f32(&x2[idx + 4]));
                vsum2 = vfmaq_f32(vsum2, vld1q_f32(&x1[idx + 8]), vld1q_f32(&x2[idx + 8]));
                vsum3 = vfmaq_f32(vsum3, vld1q_f32(&x1[idx + 12]), vld1q_f32(&x2[idx + 12]));
            }

            for (; idx <= nx - 4; idx += 4)
            {
                vsum0 = vfmaq_f32(vsum0, vld1q_f32(&x1[idx]), vld1q_f32(&x2[idx]));
            }

            vsum0 = vaddq_f32(vsum0, vsum1);
            vsum2 = vaddq_f32(vsum2, vsum3);
            vsum0 = vaddq_f32(vsum0, vsum2);

            sum = vaddvq_f32(vsum0);
        }
        else if (sa == 2 && sb == 2)
        {
            const float *p1 = x1;
            const float *p2 = x2;
            long n = nx;
            double dsum = 0.0;

            // 主循环: 每次处理32个逻辑元素 (64个float，因为stride=2)
            if (n >= 32)
            {
                long loops = n / 32;
                n = n % 32;

                __asm__ volatile(
                    // 初始化8个累加器 (v0-v7)
                    "movi   v0.4s, #0\n"
                    "movi   v1.4s, #0\n"
                    "movi   v2.4s, #0\n"
                    "movi   v3.4s, #0\n"
                    "movi   v4.4s, #0\n"
                    "movi   v5.4s, #0\n"
                    "movi   v6.4s, #0\n"
                    "movi   v7.4s, #0\n"

                    ".p2align 5\n"
                    "1:\n"
                    // ===== 预取 =====
                    "prfm   pldl1strm, [%[p1], #512]\n"
                    "prfm   pldl1strm, [%[p2], #512]\n"

                    // ===== Block A: 8个元素 + 指令交错 =====
                    "ld2    {v16.4s, v17.4s}, [%[p1]], #32\n"
                    "ld2    {v18.4s, v19.4s}, [%[p2]], #32\n"
                    "ld2    {v20.4s, v21.4s}, [%[p1]], #32\n"
                    "ld2    {v22.4s, v23.4s}, [%[p2]], #32\n"
                    "fmla   v0.4s, v16.4s, v18.4s\n"
                    "fmla   v1.4s, v20.4s, v22.4s\n"

                    // ===== Block B: 8个元素 =====
                    "ld2    {v24.4s, v25.4s}, [%[p1]], #32\n"
                    "ld2    {v26.4s, v27.4s}, [%[p2]], #32\n"
                    "ld2    {v28.4s, v29.4s}, [%[p1]], #32\n"
                    "ld2    {v30.4s, v31.4s}, [%[p2]], #32\n"
                    "fmla   v2.4s, v24.4s, v26.4s\n"
                    "fmla   v3.4s, v28.4s, v30.4s\n"

                    // ===== Block C: 8个元素 =====
                    "ld2    {v16.4s, v17.4s}, [%[p1]], #32\n"
                    "ld2    {v18.4s, v19.4s}, [%[p2]], #32\n"
                    "ld2    {v20.4s, v21.4s}, [%[p1]], #32\n"
                    "ld2    {v22.4s, v23.4s}, [%[p2]], #32\n"
                    "fmla   v4.4s, v16.4s, v18.4s\n"
                    "fmla   v5.4s, v20.4s, v22.4s\n"

                    // ===== Block D: 8个元素 =====
                    "ld2    {v24.4s, v25.4s}, [%[p1]], #32\n"
                    "ld2    {v26.4s, v27.4s}, [%[p2]], #32\n"
                    "ld2    {v28.4s, v29.4s}, [%[p1]], #32\n"
                    "ld2    {v30.4s, v31.4s}, [%[p2]], #32\n"
                    "fmla   v6.4s, v24.4s, v26.4s\n"
                    "fmla   v7.4s, v28.4s, v30.4s\n"

                    // 循环控制
                    "subs   %[loops], %[loops], #1\n"
                    "bne    1b\n"

                    // ===== 规约8个累加器 =====
                    "fadd   v0.4s, v0.4s, v1.4s\n"
                    "fadd   v2.4s, v2.4s, v3.4s\n"
                    "fadd   v4.4s, v4.4s, v5.4s\n"
                    "fadd   v6.4s, v6.4s, v7.4s\n"
                    "fadd   v0.4s, v0.4s, v2.4s\n"
                    "fadd   v4.4s, v4.4s, v6.4s\n"
                    "fadd   v0.4s, v0.4s, v4.4s\n"

                    // 水平求和
                    "faddp  v0.4s, v0.4s, v0.4s\n"
                    "faddp  s0, v0.2s\n"

                    // 转换为double
                    "fcvt   d0, s0\n"
                    "str    d0, [%[sum]]\n"

                    : [p1] "+r"(p1), [p2] "+r"(p2), [loops] "+r"(loops)
                    : [sum] "r"(&dsum)
                    : "memory", "cc",
                      "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
                      "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
                      "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31");
            }

            // 中等尾部: 处理16个元素
            if (n >= 16)
            {
                float32x4_t vsum0 = vdupq_n_f32(0.0f);
                float32x4_t vsum1 = vdupq_n_f32(0.0f);
                float32x4_t vsum2 = vdupq_n_f32(0.0f);
                float32x4_t vsum3 = vdupq_n_f32(0.0f);

                float32x4x2_t d0 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e0 = vld2q_f32(p2);
                p2 += 8;
                float32x4x2_t d1 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e1 = vld2q_f32(p2);
                p2 += 8;
                float32x4x2_t d2 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e2 = vld2q_f32(p2);
                p2 += 8;
                float32x4x2_t d3 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e3 = vld2q_f32(p2);
                p2 += 8;

                vsum0 = vfmaq_f32(vsum0, d0.val[0], e0.val[0]);
                vsum1 = vfmaq_f32(vsum1, d1.val[0], e1.val[0]);
                vsum2 = vfmaq_f32(vsum2, d2.val[0], e2.val[0]);
                vsum3 = vfmaq_f32(vsum3, d3.val[0], e3.val[0]);

                vsum0 = vaddq_f32(vsum0, vsum1);
                vsum2 = vaddq_f32(vsum2, vsum3);
                vsum0 = vaddq_f32(vsum0, vsum2);
                dsum += (double)vaddvq_f32(vsum0);
                n -= 16;
                idx += 16;
            }

            // 小尾部: 处理8个元素
            if (n >= 8)
            {
                float32x4x2_t d0 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e0 = vld2q_f32(p2);
                p2 += 8;
                float32x4x2_t d1 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e1 = vld2q_f32(p2);
                p2 += 8;

                float32x4_t vsum0 = vmulq_f32(d0.val[0], e0.val[0]);
                vsum0 = vfmaq_f32(vsum0, d1.val[0], e1.val[0]);
                dsum += (double)vaddvq_f32(vsum0);
                n -= 8;
                idx += 8;
            }

            // 微尾部: 处理4个元素
            if (n >= 4)
            {
                float32x4x2_t d0 = vld2q_f32(p1);
                p1 += 8;
                float32x4x2_t e0 = vld2q_f32(p2);
                p2 += 8;
                float32x4_t vsum0 = vmulq_f32(d0.val[0], e0.val[0]);
                dsum += (double)vaddvq_f32(vsum0);
                n -= 4;
                idx += 4;
            }

            // 标量尾部
            for (long i = 0; i < n; i++)
            {
                dsum += (double)p1[i * 2] * (double)p2[i * 2];
            }
            idx += (int)n;

            sum = (float)dsum;
            idx = nx; // 标记所有元素已处理，跳过通用尾部循环
        }

        for (; idx < nx; idx++)
        {
            sum += x1[idx * sa] * x2[idx * sb];
        }

        *y = sum;
        return birmSuccess;
    }
    else
    {
        armpl_int_t len = nx;
        armpl_int_t inca = sa;
        armpl_int_t incb = sb;

        y[0] = cblas_sdot(len, x1, inca, x2, incb);

        return birmSuccess;
    }
}