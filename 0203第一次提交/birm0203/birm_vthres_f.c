#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vthres_f(const float *__restrict__ x1, const float x2, const int nx, float *__restrict__ y)
{
    if (!x1 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    // 创建包含阈值的向量 [x2, x2, x2, x2]
    float32x4_t v_thresh = vdupq_n_f32(x2);

    // 主循环：每次处理 4 个 float
    for (; i <= nx - 4; i += 4)
    {
        // 1. 加载数据
        float32x4_t v_x = vld1q_f32(&x1[i]);

        // 2. 比较生成掩码 (Result = (v_x > v_thresh) ? 0xFFFFFFFF : 0)
        uint32x4_t v_mask = vcgtq_f32(v_x, v_thresh);

        // 3. 位与操作实现过滤 (利用 reinterpret_cast 将 float 当作 bits 处理)
        // 0xFF... & x = x;  0x00... & x = 0.0f
        float32x4_t v_y = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(v_x), v_mask));

        // 4. 存储结果
        vst1q_f32(&y[i], v_y);
    }

    // 处理剩余的尾部元素 (标量处理)
    for (; i < nx; i++)
    {
        y[i] = (x1[i] > x2) ? x1[i] : 0.0f;
    }

    return birmSuccess;
}