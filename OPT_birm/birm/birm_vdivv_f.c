#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <stddef.h>

int birm_vdivv_f(const float *x1, const float *x2, const int nx, float *y)
{
    // 参数检查
    if (x1 == NULL || x2 == NULL || y == NULL)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    const int main_block_size = 16; // 主循环每次处理16个元素

    // 1. 使用更快的“倒数近似+牛顿迭代”方法替代直接除法 [6](@ref)
    for (; i <= nx - main_block_size; i += main_block_size)
    {
        // 一次处理4组，每组4个浮点数，共16个
        for (int j = 0; j < main_block_size; j += 4)
        {
            float32x4_t vec_x1 = vld1q_f32(x1 + i + j);
            float32x4_t vec_x2 = vld1q_f32(x2 + i + j);

            // 使用倒数近似指令 [6](@ref)
            float32x4_t recip = vrecpeq_f32(vec_x2);
            // 牛顿迭代一次，提高精度 [6](@ref)
            recip = vmulq_f32(vrecpsq_f32(vec_x2, recip), recip);
            // 将被除数与除数的倒数相乘，得到除法结果
            float32x4_t vec_result = vmulq_f32(vec_x1, recip);

            vst1q_f32(y + i + j, vec_result);
        }
    }

    // 2. 使用减计数到零的循环处理剩余元素 [3,5]
    int remaining = nx - i;
    while (remaining >= 4)
    {
        float32x4_t vec_x1 = vld1q_f32(x1 + i);
        float32x4_t vec_x2 = vld1q_f32(x2 + i);

        float32x4_t recip = vrecpeq_f32(vec_x2);
        recip = vmulq_f32(vrecpsq_f32(vec_x2, recip), recip);
        float32x4_t vec_result = vmulq_f32(vec_x1, recip);

        vst1q_f32(y + i, vec_result);
        i += 4;
        remaining -= 4;
    }

    // 3. 处理最后少于4个的元素
    for (; i < nx; i++)
    {
        y[i] = x1[i] / x2[i];
    }

    return birmSuccess;
}