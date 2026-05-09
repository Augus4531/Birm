#include <arm_neon.h>
#include <stddef.h>
#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量最大值
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @return 成功返回0，错误返回负数
 */
int birm_vmax_f(const float *x, int nx, float *y) {
    // 1. 参数检查
    if (x == NULL || y == NULL) {
        return -1; // birmParamNullError
    }

    if (nx <= 0) {
        return -2; // birmParamLengthInvalidError
    }

    // 2. 如果只有一个元素，直接返回
    if (nx == 1) {
        *y = x[0];
        return 0; // birmSuccess
    }

    int i = 0;
    float max_val = x[0]; // 初始化为第一个元素

    // 3. 使用 NEON 处理主要部分 (每次处理4个元素)
    if (nx >= 4) {
        // 初始化最大值向量为数组前4个元素
        float32x4_t max_vec = vld1q_f32(x);
        i = 4;

        // 循环处理剩余的完整向量
        for (; i <= nx - 4; i += 4) {
            float32x4_t vec = vld1q_f32(x + i);
            // [修改点1] vminq_f32 -> vmaxq_f32 (取向量最大值)
            max_vec = vmaxq_f32(max_vec, vec);
        }

        // 4. 归约：提取向量中的最大值
        // 将 4 个元素归约到 2 个: max(v[0], v[1]), max(v[2], v[3])
        // [修改点2] vpmin_f32 -> vpmax_f32 (成对取最大值)
        float32x2_t max2 = vpmax_f32(vget_low_f32(max_vec), vget_high_f32(max_vec));
        
        // 将 2 个元素归约到 1 个: max(prev[0], prev[1])
        max2 = vpmax_f32(max2, max2);
        
        // 提取结果
        max_val = vget_lane_f32(max2, 0);
    }

    // 5. 处理剩余的尾部元素 (标量部分)
    for (; i < nx; ++i) {
        // [修改点3] < 改为 >
        if (x[i] > max_val) {
            max_val = x[i];
        }
    }

    *y = max_val;
    return 0; // birmSuccess
}
