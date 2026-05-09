/**
 * @brief Complex vector * complex scalar + complex vector
 *
 * 极致优化版本 - 针对飞腾S5000C:
 * - 16路展开主循环
 * - 内联汇编最优指令调度
 * - 流式预取减少cache污染
 * - 消除块处理函数调用开销
 *
 * Formula: y[i] = x1[i] * x2 + x3[i]
 * 其中 (a+bi)*(c+di) + (e+fi) = (ac-bd+e) + (ad+bc+f)i
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_cvmulcsaddcv_f(const float *__restrict__ x1,
                        const float *__restrict__ x2,
                        const float *__restrict__ x3,
                        const int nx,
                        float *__restrict__ y)
{
    if (!x1 || !x2 || !x3 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    const float *p1 = x1;
    const float *p3 = x3;
    float *dst = y;
    long n = nx;

    // 加载复数标量到向量寄存器
    float c = x2[0];
    float d = x2[1];

    // ========== 主循环: 16路展开，每次处理16个复数 ==========
    if (n >= 16)
    {
        long loops = n / 16;
        n = n % 16;

        __asm__ volatile(
            // 广播 c 和 d 到向量寄存器
            "dup    v28.4s, %w[c]\n"
            "dup    v29.4s, %w[d]\n"

            ".p2align 5\n"
            "1:\n"
            // ===== 预取：更远距离 =====
            "prfm   pldl1strm, [%[p1], #768]\n"
            "prfm   pldl1strm, [%[p3], #768]\n"

            // ===== Block A: 加载+计算完全交错 =====
            "ld2    {v0.4s, v1.4s}, [%[p1]], #32\n"
            "ld2    {v4.4s, v5.4s}, [%[p3]], #32\n"
            "fmul   v16.4s, v0.4s, v28.4s\n" // a*c (开始计算)
            "fmul   v17.4s, v0.4s, v29.4s\n" // a*d

            "ld2    {v2.4s, v3.4s}, [%[p1]], #32\n"
            "ld2    {v6.4s, v7.4s}, [%[p3]], #32\n"
            "fmls   v16.4s, v1.4s, v29.4s\n" // a*c - b*d
            "fmla   v17.4s, v1.4s, v28.4s\n" // a*d + b*c

            "fmul   v18.4s, v2.4s, v28.4s\n"
            "fmul   v19.4s, v2.4s, v29.4s\n"
            "fadd   v16.4s, v16.4s, v4.4s\n" // + e
            "fadd   v17.4s, v17.4s, v5.4s\n" // + f

            "fmls   v18.4s, v3.4s, v29.4s\n"
            "fmla   v19.4s, v3.4s, v28.4s\n"

            // ===== Block B: 加载+计算交错 =====
            "ld2    {v0.4s, v1.4s}, [%[p1]], #32\n"
            "ld2    {v4.4s, v5.4s}, [%[p3]], #32\n"
            "fadd   v18.4s, v18.4s, v6.4s\n"
            "fadd   v19.4s, v19.4s, v7.4s\n"

            "fmul   v20.4s, v0.4s, v28.4s\n"
            "fmul   v21.4s, v0.4s, v29.4s\n"

            // 存储 Block A (与 Block B 计算并行)
            "st2    {v16.4s, v17.4s}, [%[dst]], #32\n"

            "ld2    {v2.4s, v3.4s}, [%[p1]], #32\n"
            "ld2    {v6.4s, v7.4s}, [%[p3]], #32\n"
            "fmls   v20.4s, v1.4s, v29.4s\n"
            "fmla   v21.4s, v1.4s, v28.4s\n"

            "st2    {v18.4s, v19.4s}, [%[dst]], #32\n"

            "fmul   v22.4s, v2.4s, v28.4s\n"
            "fmul   v23.4s, v2.4s, v29.4s\n"
            "fadd   v20.4s, v20.4s, v4.4s\n"
            "fadd   v21.4s, v21.4s, v5.4s\n"

            "fmls   v22.4s, v3.4s, v29.4s\n"
            "fmla   v23.4s, v3.4s, v28.4s\n"
            "fadd   v22.4s, v22.4s, v6.4s\n"
            "fadd   v23.4s, v23.4s, v7.4s\n"

            // 存储 Block B
            "st2    {v20.4s, v21.4s}, [%[dst]], #32\n"
            "st2    {v22.4s, v23.4s}, [%[dst]], #32\n"

            // 循环控制
            "subs   %[loops], %[loops], #1\n"
            "bne    1b\n"

            : [p1] "+r"(p1), [p3] "+r"(p3), [dst] "+r"(dst), [loops] "+r"(loops)
            : [c] "r"(c), [d] "r"(d)
            : "memory", "cc",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
              "v28", "v29");
    }

    // ========== 中尾部: 8个复数 ==========
    if (n >= 8)
    {
        float32x4_t vc = vdupq_n_f32(c);
        float32x4_t vd = vdupq_n_f32(d);

        float32x4x2_t x1_0 = vld2q_f32(p1);
        float32x4x2_t x1_1 = vld2q_f32(p1 + 8);
        float32x4x2_t x3_0 = vld2q_f32(p3);
        float32x4x2_t x3_1 = vld2q_f32(p3 + 8);

        float32x4x2_t y0, y1;
        y0.val[0] = vaddq_f32(vfmsq_f32(vmulq_f32(x1_0.val[0], vc), x1_0.val[1], vd), x3_0.val[0]);
        y0.val[1] = vaddq_f32(vfmaq_f32(vmulq_f32(x1_0.val[0], vd), x1_0.val[1], vc), x3_0.val[1]);
        y1.val[0] = vaddq_f32(vfmsq_f32(vmulq_f32(x1_1.val[0], vc), x1_1.val[1], vd), x3_1.val[0]);
        y1.val[1] = vaddq_f32(vfmaq_f32(vmulq_f32(x1_1.val[0], vd), x1_1.val[1], vc), x3_1.val[1]);

        vst2q_f32(dst, y0);
        vst2q_f32(dst + 8, y1);

        p1 += 16;
        p3 += 16;
        dst += 16;
        n -= 8;
    }

    // ========== 小尾部: 4个复数 ==========
    if (n >= 4)
    {
        float32x4_t vc = vdupq_n_f32(c);
        float32x4_t vd = vdupq_n_f32(d);

        float32x4x2_t x1_0 = vld2q_f32(p1);
        float32x4x2_t x3_0 = vld2q_f32(p3);

        float32x4x2_t y0;
        y0.val[0] = vaddq_f32(vfmsq_f32(vmulq_f32(x1_0.val[0], vc), x1_0.val[1], vd), x3_0.val[0]);
        y0.val[1] = vaddq_f32(vfmaq_f32(vmulq_f32(x1_0.val[0], vd), x1_0.val[1], vc), x3_0.val[1]);

        vst2q_f32(dst, y0);

        p1 += 8;
        p3 += 8;
        dst += 8;
        n -= 4;
    }

    // ========== 标量尾部 ==========
    while (n > 0)
    {
        float a = *p1++;
        float b = *p1++;
        float e = *p3++;
        float f = *p3++;
        *dst++ = (a * c - b * d) + e;
        *dst++ = (a * d + b * c) + f;
        n--;
    }

    return birmSuccess;
}