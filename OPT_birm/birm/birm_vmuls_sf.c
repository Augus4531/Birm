/**
 * @brief 浮点单精度实数矢量与单精度实数标量的乘法，带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输入y步长
 * @return 成功返回0，错误返回负数，警告返回正数
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vmuls_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    int i = 0;
    int count = nx;
    const float *src = x1;
    float *dst = y;
    float32x4_t vscalar = vdupq_n_f32(x2);

    // =========================================================
    // Case 1: 连续内存访问 (sa=1, sb=1)
    // =========================================================
    if (sa == 1 && sb == 1)
    {
        // 1. 小数据量 (L1/L2 Cache)
        if (nx <= 32000)
        {
            // Alignment Peeling
            while (count > 0 && ((uintptr_t)dst & 0xF))
            {
                *dst++ = *src++ * x2;
                count--;
            }
            // NEON Loop
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
            while (count >= 4)
            {
                vst1q_f32(dst, vmulq_f32(vld1q_f32(src), vscalar));
                src += 4;
                dst += 4;
                count -= 4;
            }
        }
        // 2. 大数据量 (DDR) - STNP 优化
        else
        {
            // Alignment Peeling to 64 bytes
            while (count > 0 && ((uintptr_t)dst & 63))
            {
                *dst++ = *src++ * x2;
                count--;
            }
            // STNP Loop (32 floats / iter)
            while (count >= 32)
            {
                __asm__ volatile(
                    "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
                    "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src]], #64 \n\t"
                    "fmul   v0.4s, v0.4s, %[vval].4s     \n\t"
                    "fmul   v1.4s, v1.4s, %[vval].4s     \n\t"
                    "fmul   v2.4s, v2.4s, %[vval].4s     \n\t"
                    "fmul   v3.4s, v3.4s, %[vval].4s     \n\t"
                    "fmul   v4.4s, v4.4s, %[vval].4s     \n\t"
                    "fmul   v5.4s, v5.4s, %[vval].4s     \n\t"
                    "fmul   v6.4s, v6.4s, %[vval].4s     \n\t"
                    "fmul   v7.4s, v7.4s, %[vval].4s     \n\t"
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
            // Tail Handling
            while (count >= 4)
            {
                vst1q_f32(dst, vmulq_f32(vld1q_f32(src), vscalar));
                src += 4;
                dst += 4;
                count -= 4;
            }
        }
    }
    // =========================================================
    // Case 2: 输入步长为2，输出连续 (sa=2, sb=1)
    // =========================================================
    else if (sa == 2 && sb == 1)
    {
        // 1. 小数据量处理 (L1/L2)
        if (nx <= 32000)
        {
            // 每次处理 8 个输出 (读取 16 个输入)
            while (count >= 8)
            {
                // 使用 ld2 进行解交织
                float32x4x2_t g0 = vld2q_f32(src);     // load 8 floats (4 real, 4 imag)
                float32x4x2_t g1 = vld2q_f32(src + 8); // load next 8 floats

                vst1q_f32(dst, vmulq_f32(g0.val[0], vscalar));
                vst1q_f32(dst + 4, vmulq_f32(g1.val[0], vscalar));

                src += 16; // 步长2，读取了8个有效点，内存前进 16 floats
                dst += 8;
                count -= 8;
            }
        }
        // 2. 大数据量处理 (DDR) - 使用 STNP
        else
        {
            // Alignment Peeling for DST (Important for STNP)
            while (count > 0 && ((uintptr_t)dst & 63))
            {
                *dst++ = *src * x2;
                src += 2;
                count--;
            }

            // 核心循环：处理 16 个输出 (64字节)
            // 需要读取 32 个输入 (128字节)
            while (count >= 16)
            {
                __asm__ volatile(
                    // 1. Load 128 bytes (32 floats) from SRC
                    // 使用 ld1 连续加载，比 ld2 更快
                    "ld1    {v0.4s, v1.4s, v2.4s, v3.4s}, [%[src]], #64 \n\t"
                    "ld1    {v4.4s, v5.4s, v6.4s, v7.4s}, [%[src]], #64 \n\t"

                    // 2. Unzip (解交织) - 提取偶数位置元素
                    // uzp1 v0, v1 -> v0 变为 v0和v1的偶数项组合
                    "uzp1   v0.4s, v0.4s, v1.4s          \n\t" // Result A
                    "uzp1   v1.4s, v2.4s, v3.4s          \n\t" // Result B
                    "uzp1   v2.4s, v4.4s, v5.4s          \n\t" // Result C
                    "uzp1   v3.4s, v6.4s, v7.4s          \n\t" // Result D

                    // 3. Compute
                    "fmul   v0.4s, v0.4s, %[vval].4s     \n\t"
                    "fmul   v1.4s, v1.4s, %[vval].4s     \n\t"
                    "fmul   v2.4s, v2.4s, %[vval].4s     \n\t"
                    "fmul   v3.4s, v3.4s, %[vval].4s     \n\t"

                    // 4. Store using STNP (Bypass Cache RFO for DST)
                    "stnp   q0, q1, [%[dst]]             \n\t"
                    "stnp   q2, q3, [%[dst], #32]        \n\t"

                    "add    %[dst], %[dst], #64          \n\t"

                    : [src] "+r"(src), [dst] "+r"(dst)
                    : [vval] "w"(vscalar)
                    : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");

                count -= 16;
            }
        }

        // Tail
        while (count >= 4)
        {
            float32x4x2_t g = vld2q_f32(src);
            vst1q_f32(dst, vmulq_f32(g.val[0], vscalar));
            src += 8;
            dst += 4;
            count -= 4;
        }
    }

    // =========================================================
    // Case 3: 输入输出步长均为 2 (sa=2, sb=2)
    // =========================================================
    else if (sa == 2 && sb == 2)
    {
        if (nx >= 262144)
        {
            float32x4_t v_x2 = vdupq_n_f32(x2);

            // 8路循环展开
            for (; i <= nx - 8; i += 8)
            {
                __builtin_prefetch(&x1[(i + 16) * 2], 0, 0);

                // 高效加载：利用 vld2q 一次读取连续内存
                float32x4x2_t d1 = vld2q_f32(&x1[i * 2]); // 加载 8 个 float
                float32x4x2_t d2 = vld2q_f32(&x1[(i + 4) * 2]);

                // 向量计算 (由减法改为乘法)
                float32x4_t res1 = vmulq_f32(d1.val[0], v_x2);
                float32x4_t res2 = vmulq_f32(d2.val[0], v_x2);

                // 离散存储：手动解包写入，跳过间隔
                // 处理第一组 res1
                vst1q_lane_f32(&y[(i + 0) * 2], res1, 0);
                vst1q_lane_f32(&y[(i + 1) * 2], res1, 1);
                vst1q_lane_f32(&y[(i + 2) * 2], res1, 2);
                vst1q_lane_f32(&y[(i + 3) * 2], res1, 3);

                // 处理第二组 res2
                vst1q_lane_f32(&y[(i + 4) * 2], res2, 0);
                vst1q_lane_f32(&y[(i + 5) * 2], res2, 1);
                vst1q_lane_f32(&y[(i + 6) * 2], res2, 2);
                vst1q_lane_f32(&y[(i + 7) * 2], res2, 3);
            }
        }
        else
        {
            // 预计算索引偏移，避免乘法指令
            int sa_2 = sa * 2;
            int sa_3 = sa * 3;
            int sb_2 = sb * 2;
            int sb_3 = sb * 3;

            int step_in = sa * 4;
            int step_out = sb * 4;

            int idx_in = 0;
            int idx_out = 0;

            // 4路标量展开
            // 这里的 idx_in/idx_out 是相对于当前 src/dst 指针的偏移
            // 所以我们需要先更新 src/dst 指针到 i 的位置
            src += i * 2; // sa=2
            dst += i * 2; // sb=2

            int limit = nx - i;
            int j = 0;

            for (; j <= limit - 4; j += 4)
            {
                float v0 = src[idx_in];
                float v1 = src[idx_in + sa];
                float v2 = src[idx_in + sa_2];
                float v3 = src[idx_in + sa_3];

                dst[idx_out] = v0 * x2; // 改为乘法
                dst[idx_out + sb] = v1 * x2;
                dst[idx_out + sb_2] = v2 * x2;
                dst[idx_out + sb_3] = v3 * x2;

                idx_in += step_in;
                idx_out += step_out;
            }
            // 更新全局索引 i
            i += j;
        }

        // 更新 count 以供最后的通用尾部处理
        count = nx - i;
        src = x1 + i * sa;
        dst = y + i * sb;
    }

    // =========================================================
    // Case 3: 通用步长 (General Stride) - 多指针标量并行
    // =========================================================
    else
    {
        const float *src_ptr0 = x1 + i * sa;
        const float *src_ptr1 = src_ptr0 + sa;
        const float *src_ptr2 = src_ptr1 + sa;
        const float *src_ptr3 = src_ptr2 + sa;
        float *dst_ptr0 = y + i * sb;
        float *dst_ptr1 = dst_ptr0 + sb;
        float *dst_ptr2 = dst_ptr1 + sb;
        float *dst_ptr3 = dst_ptr2 + sb;
        int step_in = sa * 4;
        int step_out = sb * 4;

        for (; i <= nx - 4; i += 4)
        {
            dst_ptr0[0] = src_ptr0[0] * x2;
            dst_ptr1[0] = src_ptr1[0] * x2;
            dst_ptr2[0] = src_ptr2[0] * x2;
            dst_ptr3[0] = src_ptr3[0] * x2;

            src_ptr0 += step_in;
            src_ptr1 += step_in;
            src_ptr2 += step_in;
            src_ptr3 += step_in;
            dst_ptr0 += step_out;
            dst_ptr1 += step_out;
            dst_ptr2 += step_out;
            dst_ptr3 += step_out;
        }
        count = nx - i;
    }

    // Scalar Tail
    while (count-- > 0)
    {
        *dst++ = *src * x2;
        src += sa;
        dst += (sb - 1); // dst already ++, so add sb-1
    }

    return birmSuccess;
}