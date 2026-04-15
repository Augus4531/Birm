#include <arm_neon.h>
#include <stddef.h>
#include "birm_arm_float_User.h"

int birm_vmin_f(const float *x, int nx, float *y) {
    // 参数检查
    if (x == NULL || y == NULL) {
        return -1;
    }

    if (nx <= 0) {
        return -2;
    }

    // 如果只有一个元素，直接返回该元素
    if (nx == 1) {
        *y = x[0];
        return 0;
    }

    int i = 0;
    float min_val = x[0];

    // 使用NEON处理主要部分 (每次处理4个元素)
    if (nx >= 4) {
        // 初始化最小值为第一个向量的最小值
        float32x4_t min_vec = vld1q_f32(x);
        i = 4;

        // 处理剩余的完整向量
        for (; i <= nx - 4; i += 4) {
            float32x4_t vec = vld1q_f32(x + i);
            min_vec = vminq_f32(min_vec, vec);
        }

        // 提取向量中的最小值
        float32x2_t min2 = vpmin_f32(vget_low_f32(min_vec), vget_high_f32(min_vec));
        min2 = vpmin_f32(min2, min2);
        min_val = vget_lane_f32(min2, 0);
    }

    // 处理剩余的元素
    for (; i < nx; ++i) {
        if (x[i] < min_val) {
            min_val = x[i];
        }
    }

    *y = min_val;
    return 0;
}
