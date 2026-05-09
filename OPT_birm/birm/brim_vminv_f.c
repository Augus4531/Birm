#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vminv_f(const float *x1, const float *x2, int nx, float *y)
{
    // 参数检查
    if (x1 == NULL || x2 == NULL || y == NULL)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 确保内存对齐（64字节对齐，匹配缓存行）
    // 保持与原代码一致的严格对齐检查逻辑
    if (((uintptr_t)x1 & 0x3F) != 0 || ((uintptr_t)x2 & 0x3F) != 0 || ((uintptr_t)y & 0x3F) != 0)
    {
        // 如果未对齐，回退到标量处理
        for (int i = 0; i < nx; i++)
        {
            // 修改：取最小值
            y[i] = (x1[i] < x2[i]) ? x1[i] : x2[i];
        }
        return birmSuccess;
    }

    int i = 0;
    // 主循环：每次处理 8 个 float (2个 128-bit 寄存器)
    for (; i <= nx - 8; i += 8)
    {
        float32x4_t x1_1 = vld1q_f32(x1 + i);
        float32x4_t x1_2 = vld1q_f32(x1 + i + 4);
        float32x4_t x2_1 = vld1q_f32(x2 + i);
        float32x4_t x2_2 = vld1q_f32(x2 + i + 4);

        // 修改：使用 vminq_f32 计算最小值
        float32x4_t result1 = vminq_f32(x1_1, x2_1);
        float32x4_t result2 = vminq_f32(x1_2, x2_2);

        vst1q_f32(y + i, result1);
        vst1q_f32(y + i + 4, result2);
    }

    // 处理剩余标量数据
    for (; i < nx; i++)
    {
        // 修改：取最小值
        y[i] = (x1[i] < x2[i]) ? x1[i] : x2[i];
    }

    return birmSuccess;
}
