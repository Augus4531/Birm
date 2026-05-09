/*
 * birm_cvdotcv_sf_opt.c
 * Optimized for Phytium S5000C (ARMv8/ASIMD)
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

/**
 * @brief 复数向量点积 (Complex Vector Dot Product) - 飞腾S5000C优化版
 * * 公式: sum += x1[i] * x2[i]
 * 复数乘法: (a + bi)(c + di) = (ac - bd) + i(ad + bc)
 * * 优化策略:
 * 1. Restrict 指针: 允许编译器激进重排加载指令。
 * 2. Stride-1: 使用 vld2 解交织加载，4路展开。
 * 3. Stride-2: 使用 vld1 加载 + vuzp1q_f64 过滤间隔，避免 vld4 的带宽浪费。
 */
int birm_cvdotcv_sf(const float *__restrict x1, const int sa, const float *__restrict x2, const int sb, const int nx, float *__restrict y)
{
    // --- 参数检查 ---
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // --- 累加器初始化 ---
    float sum_real = 0.0f;
    float sum_imag = 0.0f;
    int idx = 0;

    // 使用4组累加器来掩盖 FMA 指令延迟 (Latency Hiding)
    float32x4_t vacc_r0 = vdupq_n_f32(0.0f), vacc_i0 = vdupq_n_f32(0.0f);
    float32x4_t vacc_r1 = vdupq_n_f32(0.0f), vacc_i1 = vdupq_n_f32(0.0f);
    float32x4_t vacc_r2 = vdupq_n_f32(0.0f), vacc_i2 = vdupq_n_f32(0.0f);
    float32x4_t vacc_r3 = vdupq_n_f32(0.0f), vacc_i3 = vdupq_n_f32(0.0f);

    // ==========================================================
    // 路径 A: 连续内存 (sa == 1 && sb == 1)
    // ==========================================================
    if (sa == 1 && sb == 1)
    {
        const float *ptr_x1 = x1;
        const float *ptr_x2 = x2;

        // 每次循环处理 16 个复数 (4 vectors * 4 floats)
        for (; idx <= nx - 16; idx += 16)
        {
            // vld2 将实部和虚部自动分流到 val[0] 和 val[1]
            float32x4x2_t d1_0 = vld2q_f32(ptr_x1);
            float32x4x2_t d2_0 = vld2q_f32(ptr_x2);
            float32x4x2_t d1_1 = vld2q_f32(ptr_x1 + 8);
            float32x4x2_t d2_1 = vld2q_f32(ptr_x2 + 8);
            float32x4x2_t d1_2 = vld2q_f32(ptr_x1 + 16);
            float32x4x2_t d2_2 = vld2q_f32(ptr_x2 + 16);
            float32x4x2_t d1_3 = vld2q_f32(ptr_x1 + 24);
            float32x4x2_t d2_3 = vld2q_f32(ptr_x2 + 24);

            // Group 0
            vacc_r0 = vfmaq_f32(vacc_r0, d1_0.val[0], d2_0.val[0]); // Real += a*c
            vacc_r0 = vfmsq_f32(vacc_r0, d1_0.val[1], d2_0.val[1]); // Real -= b*d
            vacc_i0 = vfmaq_f32(vacc_i0, d1_0.val[0], d2_0.val[1]); // Imag += a*d
            vacc_i0 = vfmaq_f32(vacc_i0, d1_0.val[1], d2_0.val[0]); // Imag += b*c

            // Group 1
            vacc_r1 = vfmaq_f32(vacc_r1, d1_1.val[0], d2_1.val[0]);
            vacc_r1 = vfmsq_f32(vacc_r1, d1_1.val[1], d2_1.val[1]);
            vacc_i1 = vfmaq_f32(vacc_i1, d1_1.val[0], d2_1.val[1]);
            vacc_i1 = vfmaq_f32(vacc_i1, d1_1.val[1], d2_1.val[0]);

            // Group 2
            vacc_r2 = vfmaq_f32(vacc_r2, d1_2.val[0], d2_2.val[0]);
            vacc_r2 = vfmsq_f32(vacc_r2, d1_2.val[1], d2_2.val[1]);
            vacc_i2 = vfmaq_f32(vacc_i2, d1_2.val[0], d2_2.val[1]);
            vacc_i2 = vfmaq_f32(vacc_i2, d1_2.val[1], d2_2.val[0]);

            // Group 3
            vacc_r3 = vfmaq_f32(vacc_r3, d1_3.val[0], d2_3.val[0]);
            vacc_r3 = vfmsq_f32(vacc_r3, d1_3.val[1], d2_3.val[1]);
            vacc_i3 = vfmaq_f32(vacc_i3, d1_3.val[0], d2_3.val[1]);
            vacc_i3 = vfmaq_f32(vacc_i3, d1_3.val[1], d2_3.val[0]);

            ptr_x1 += 32; // 16 complex * 2 floats = 32 floats
            ptr_x2 += 32;
        }
    }
    // ==========================================================
    // 路径 B: 间隔为2 (sa == 2 && sb == 2)
    // ==========================================================
    else if (sa == 2 && sb == 2)
    {
        const float *ptr_x1 = x1;
        const float *ptr_x2 = x2;

        // 每次循环处理 8 个有效复数 (Unroll 2x vectors of valid data)
        // 实际上在内存中跨越了 16 个复数的位置 (stride 2)
        // 内存消耗: 16 complex * 2 floats = 32 floats (128 bytes)
        for (; idx <= nx - 8; idx += 8)
        {
            // --- Step 1: 加载原始数据 (包含间隔) ---
            // 需要8个有效复数，数据分布在16个复数空间里
            // 每次 vld1q 加载2个复数空间 (其中1个有效，1个无效)
            // 需要加载 8 次 vld1q 来覆盖范围

            // Block 0 (Indices 0, 2) from memory offsets 0, 1, 2, 3
            float32x4_t x1_raw0 = vld1q_f32(ptr_x1 + 0);
            float32x4_t x1_raw1 = vld1q_f32(ptr_x1 + 4);
            float32x4_t x2_raw0 = vld1q_f32(ptr_x2 + 0);
            float32x4_t x2_raw1 = vld1q_f32(ptr_x2 + 4);

            // Block 1 (Indices 4, 6) from memory offsets 4, 5, 6, 7
            float32x4_t x1_raw2 = vld1q_f32(ptr_x1 + 8);
            float32x4_t x1_raw3 = vld1q_f32(ptr_x1 + 12);
            float32x4_t x2_raw2 = vld1q_f32(ptr_x2 + 8);
            float32x4_t x2_raw3 = vld1q_f32(ptr_x2 + 12);

            // Block 2 (Indices 8, 10)
            float32x4_t x1_raw4 = vld1q_f32(ptr_x1 + 16);
            float32x4_t x1_raw5 = vld1q_f32(ptr_x1 + 20);
            float32x4_t x2_raw4 = vld1q_f32(ptr_x2 + 16);
            float32x4_t x2_raw5 = vld1q_f32(ptr_x2 + 20);

            // Block 3 (Indices 12, 14)
            float32x4_t x1_raw6 = vld1q_f32(ptr_x1 + 24);
            float32x4_t x1_raw7 = vld1q_f32(ptr_x1 + 28);
            float32x4_t x2_raw6 = vld1q_f32(ptr_x2 + 24);
            float32x4_t x2_raw7 = vld1q_f32(ptr_x2 + 28);

            // --- Step 2: 过滤间隔 (Filter) ---
            // 将 vector 视为 float64x2，提取偶数项 (index 0)，丢弃奇数项 (stride gap)

            // Valid Indices 0, 2, 4, 6 (Packed)
            float32x4_t x1_v02 = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x1_raw0), vreinterpretq_f64_f32(x1_raw1)));
            float32x4_t x1_v46 = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x1_raw2), vreinterpretq_f64_f32(x1_raw3)));
            float32x4_t x2_v02 = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x2_raw0), vreinterpretq_f64_f32(x2_raw1)));
            float32x4_t x2_v46 = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x2_raw2), vreinterpretq_f64_f32(x2_raw3)));

            // Valid Indices 8, 10, 12, 14 (Packed)
            float32x4_t x1_v8A = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x1_raw4), vreinterpretq_f64_f32(x1_raw5)));
            float32x4_t x1_vCE = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x1_raw6), vreinterpretq_f64_f32(x1_raw7)));
            float32x4_t x2_v8A = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x2_raw4), vreinterpretq_f64_f32(x2_raw5)));
            float32x4_t x2_vCE = vreinterpretq_f32_f64(vuzp1q_f64(vreinterpretq_f64_f32(x2_raw6), vreinterpretq_f64_f32(x2_raw7)));

            // --- Step 3: 实虚部分离 (De-interleave) ---
            float32x4_t x1_re_0 = vuzp1q_f32(x1_v02, x1_v46); // Real parts of 0,2,4,6
            float32x4_t x1_im_0 = vuzp2q_f32(x1_v02, x1_v46); // Imag parts of 0,2,4,6
            float32x4_t x2_re_0 = vuzp1q_f32(x2_v02, x2_v46);
            float32x4_t x2_im_0 = vuzp2q_f32(x2_v02, x2_v46);

            float32x4_t x1_re_1 = vuzp1q_f32(x1_v8A, x1_vCE); // Real parts of 8,10,12,14
            float32x4_t x1_im_1 = vuzp2q_f32(x1_v8A, x1_vCE);
            float32x4_t x2_re_1 = vuzp1q_f32(x2_v8A, x2_vCE);
            float32x4_t x2_im_1 = vuzp2q_f32(x2_v8A, x2_vCE);

            // --- Step 4: 向量计算 ---
            // Chunk 0
            vacc_r0 = vfmaq_f32(vacc_r0, x1_re_0, x2_re_0);
            vacc_r0 = vfmsq_f32(vacc_r0, x1_im_0, x2_im_0);
            vacc_i0 = vfmaq_f32(vacc_i0, x1_re_0, x2_im_0);
            vacc_i0 = vfmaq_f32(vacc_i0, x1_im_0, x2_re_0);

            // Chunk 1
            vacc_r1 = vfmaq_f32(vacc_r1, x1_re_1, x2_re_1);
            vacc_r1 = vfmsq_f32(vacc_r1, x1_im_1, x2_im_1);
            vacc_i1 = vfmaq_f32(vacc_i1, x1_re_1, x2_im_1);
            vacc_i1 = vfmaq_f32(vacc_i1, x1_im_1, x2_re_1);

            ptr_x1 += 32; // 8 valid complex * stride 2 * 2 floats = 32 floats
            ptr_x2 += 32;
        }
    }

    // --- 向量归约 (Horizontal Reduction) ---
    // 只有当使用了向量路径时才进行归约
    if ((sa == 1 && sb == 1) || (sa == 2 && sb == 2))
    {
        // 汇总是 4 个 accumulator 的结果 (如果是 Stride 2 路径只用到了 r0,r1，r2,r3为0，不影响结果)
        vacc_r0 = vaddq_f32(vaddq_f32(vacc_r0, vacc_r1), vaddq_f32(vacc_r2, vacc_r3));
        vacc_i0 = vaddq_f32(vaddq_f32(vacc_i0, vacc_i1), vaddq_f32(vacc_i2, vacc_i3));

        // ARMv8 专用指令 vaddvq，比 vget_lane 循环加更高效
        sum_real += vaddvq_f32(vacc_r0);
        sum_imag += vaddvq_f32(vacc_i0);
    }

    // ==========================================================
    // 处理剩余元素 / 通用路径
    // ==========================================================
    for (; idx < nx; idx++)
    {
        // 计算指针偏移，这里使用乘法是因为剩余次数少，且sa/sb可能不规则
        // 编译器通常会优化这里的循环步进
        float a = x1[2 * (idx * sa)];
        float b = x1[2 * (idx * sa) + 1];
        float c = x2[2 * (idx * sb)];
        float d = x2[2 * (idx * sb) + 1];

        sum_real += (a * c - b * d);
        sum_imag += (a * d + b * c);
    }

    y[0] = sum_real;
    y[1] = sum_imag;

    return birmSuccess;
}