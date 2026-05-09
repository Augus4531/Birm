/**
 * @brief 浮点单精度实数矢量与单精度实数标量的加法，带步长
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

int birm_vadds_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb)
{
    if (!x1 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    const float *src = x1;
    float *dst = y;
    int count = nx;

    // ---------------------------------------------------------
    // Case 1: Stride = 1 - 连续内存
    // ---------------------------------------------------------
    if (sa == 1 && sb == 1)
    {
        // Broadcast scalar x2 to a vector register
        float32x4_t vscalar = vdupq_n_f32(x2);

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
                "tbz    %[n], #3, 2f                \n\t" // Check bit 3 (8 elements)
                "ld1    {v0.4s, v1.4s}, [%[src]], #32 \n\t"
                "fadd   v0.4s, v0.4s, %[vval].4s    \n\t"
                "fadd   v1.4s, v1.4s, %[vval].4s    \n\t"
                "st1    {v0.4s, v1.4s}, [%[dst]], #32 \n\t"
                "2:                                 \n\t"

                "tbz    %[n], #2, 3f                \n\t" // Check bit 2 (4 elements)
                "ld1    {v0.4s}, [%[src]], #16      \n\t"
                "fadd   v0.4s, v0.4s, %[vval].4s    \n\t"
                "st1    {v0.4s}, [%[dst]], #16      \n\t"
                "3:                                 \n\t"

                "tbz    %[n], #1, 4f                \n\t" // Check bit 1 (2 elements)
                "ld1    {v0.2s}, [%[src]], #8       \n\t"
                "fadd   v0.2s, v0.2s, %[vval].2s    \n\t"
                "st1    {v0.2s}, [%[dst]], #8       \n\t"
                "4:                                 \n\t"

                "tbz    %[n], #0, 5f                \n\t" // Check bit 0 (1 element)
                "ld1    {v0.s}[0], [%[src]]         \n\t"
                "fadd   s0, s0, %s[vval]            \n\t"
                "st1    {v0.s}[0], [%[dst]]         \n\t"
                "5:                                 \n\t"

                : [src] "+r"(src), [dst] "+r"(dst)
                : [n] "r"(count), [vval] "w"(vscalar)
                : "cc", "memory", "v0", "v1");

            // 指针已更新，count 清零
            count = 0;
        }
    }
    // ---------------------------------------------------------
    // Case 2: Stride = 2 Optimization (利用 vld2 解交织)
    // ---------------------------------------------------------
    else if (sa == 2 && sb == 2)
    {
        if (nx >= 262144)
        {
            float32x4_t v_x2 = vdupq_n_f32(x2);
            int i = 0;

            // 使用 Lane Store 避免破坏目标地址的间隔数据
            // 8路循环展开
            for (; i <= nx - 8; i += 8)
            {
                __builtin_prefetch(&x1[(i + 16) * 2], 0, 0);

                float32x4x2_t d1 = vld2q_f32(&x1[i * 2]);
                float32x4x2_t d2 = vld2q_f32(&x1[(i + 4) * 2]);

                float32x4_t res1 = vaddq_f32(d1.val[0], v_x2);
                float32x4_t res2 = vaddq_f32(d2.val[0], v_x2);

                // 离散存储 (Lane Store)
                vst1q_lane_f32(&y[(i + 0) * 2], res1, 0);
                vst1q_lane_f32(&y[(i + 1) * 2], res1, 1);
                vst1q_lane_f32(&y[(i + 2) * 2], res1, 2);
                vst1q_lane_f32(&y[(i + 3) * 2], res1, 3);

                vst1q_lane_f32(&y[(i + 4) * 2], res2, 0);
                vst1q_lane_f32(&y[(i + 5) * 2], res2, 1);
                vst1q_lane_f32(&y[(i + 6) * 2], res2, 2);
                vst1q_lane_f32(&y[(i + 7) * 2], res2, 3);
            }

            // 更新指针和计数，准备进入标量尾部处理
            src += i * 2;
            dst += i * 2;
            count -= i;
        }

        // --- Stride=2 标量 (Pointer Optimized) ---
        // 优化：使用指针加法代替乘法索引
        int step_src = 2 * 4; // float * 2 in bytes = 8 bytes
        int step_dst = 2 * 4;

        // 手动将指针转为 float* 操作，利用 float 指针+1 等于字节+4 的特性
        // 这里直接操作 src/dst 指针，每次 +2 即可
        while (count >= 4)
        {
            float v0 = src[0];
            float v1 = src[2];
            float v2 = src[4];
            float v3 = src[6];

            dst[0] = v0 + x2;
            dst[2] = v1 + x2;
            dst[4] = v2 + x2;
            dst[6] = v3 + x2;

            src += 8; // 4个元素 * 步长2
            dst += 8;
            count -= 4;
        }
    }
    // ---------------------------------------------------------
    // Case 3: Generic Path (Arbitrary Stride)
    // ---------------------------------------------------------
    else
    {
        long stride_src = sa * 4; // 转换为字节步长
        long stride_dst = sb * 4; // 转换为字节步长

        // Unroll 4x for generic path
        while (count >= 4)
        {
            __asm__ volatile(
                // --- 加载阶段 ---
                "ldr    s0, [%[src]]                \n\t" // Load x[0]
                "ldr    s1, [%[src], %[sa]]         \n\t" // Load x[1*sa]
                "add    x0, %[src], %[sa]           \n\t" // tmp = src + sa (计算地址)
                "ldr    s2, [x0, %[sa]]             \n\t" // Load x[2*sa]
                "add    x0, x0, %[sa]               \n\t" // tmp = src + 2*sa
                "ldr    s3, [x0, %[sa]]             \n\t" // Load x[3*sa]

                // --- 计算阶段 ---
                "fadd   s0, s0, %s[val]             \n\t"
                "fadd   s1, s1, %s[val]             \n\t"
                "fadd   s2, s2, %s[val]             \n\t"
                "fadd   s3, s3, %s[val]             \n\t"

                // --- 存储阶段 ---
                "str    s0, [%[dst]]                \n\t" // Store y[0]
                "str    s1, [%[dst], %[sb]]         \n\t" // Store y[1*sb]

                "add    x0, %[dst], %[sb]           \n\t" // tmp = dst + sb (仅计算地址，不要 [])

                "str    s2, [x0, %[sb]]             \n\t" // Store y[2*sb]
                "add    x0, x0, %[sb]               \n\t" // tmp = dst + 2*sb
                "str    s3, [x0, %[sb]]             \n\t" // Store y[3*sb]

                // --- 更新主指针 ---
                // 每次循环处理4个元素，所以指针前进 4 * stride
                // lsl #2 相当于乘以4
                "add    %[src], %[src], %[sa], lsl #2 \n\t"
                "add    %[dst], %[dst], %[sb], lsl #2 \n\t"

                : [src] "+r"(src), [dst] "+r"(dst)
                : [sa] "r"(stride_src), [sb] "r"(stride_dst), [val] "w"(x2)
                : "memory", "cc", "s0", "s1", "s2", "s3", "x0"); // 声明破坏了 x0 寄存器
            count -= 4;
        }
    }

    // ---------------------------------------------------------
    // Common Tail Loop (For all cases)
    // ---------------------------------------------------------
    // 处理剩余元素 (count < 4 or count < 16 etc.)
    while (count > 0)
    {
        *dst = *src + x2;
        src += sa;
        dst += sb;
        count--;
    }

    return birmSuccess;
}