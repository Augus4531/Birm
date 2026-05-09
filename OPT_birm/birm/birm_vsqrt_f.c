#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

int birm_vsqrt_f(const float *x, const int nx, float *y)
{
    // 参数检查
    if (x == NULL || y == NULL)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 确保内存对齐（16字节对齐可提升性能）
    if (((uintptr_t)x % 16 != 0) || ((uintptr_t)y % 16 != 0))
    {
        // 非对齐内存处理，回退到标量方式检查负数
        for (int i = 0; i < nx; i++)
        {
            if (x[i] < 0.0f)
                return -3;
        }

        // 处理非对齐部分
        int i = 0;
        while (i < nx && ((uintptr_t)(x + i) % 16 != 0))
        {
            y[i] = sqrtf(x[i]);
            i++;
        }

        // 主NEON循环（对齐部分）
        for (; i <= nx - 8; i += 8)
        { // 循环展开：一次处理8个元素
            float32x4_t vec_x1 = vld1q_f32(x + i);
            float32x4_t vec_x2 = vld1q_f32(x + i + 4);

            float32x4_t vec_sqrt1 = vsqrtq_f32(vec_x1);
            float32x4_t vec_sqrt2 = vsqrtq_f32(vec_x2);

            vst1q_f32(y + i, vec_sqrt1);
            vst1q_f32(y + i + 4, vec_sqrt2);
        }

        // 处理剩余元素
        for (; i < nx; i++)
        {
            y[i] = sqrtf(x[i]);
        }
    }
    else
    {
        // 内存已对齐的优化路径
        int i = 0;

        // 主NEON循环：一次处理8个元素（循环展开）
        for (; i <= nx - 8; i += 8)
        {
            // 使用连续加载指令减少指令依赖性[6](@ref)
            float32x4_t vec_x1 = vld1q_f32(x + i);
            float32x4_t vec_x2 = vld1q_f32(x + i + 4);

            // 并行计算两个向量的平方根
            float32x4_t vec_sqrt1 = vsqrtq_f32(vec_x1);
            float32x4_t vec_sqrt2 = vsqrtq_f32(vec_x2);

            vst1q_f32(y + i, vec_sqrt1);
            vst1q_f32(y + i + 4, vec_sqrt2);
        }

        // 处理剩余元素（少于8个）
        for (; i < nx; i++)
        {
            y[i] = sqrtf(x[i]);
        }
    }

    return birmSuccess;
}
