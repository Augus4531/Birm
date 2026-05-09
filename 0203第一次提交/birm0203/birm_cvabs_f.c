#include <arm_neon.h>
#include <stddef.h>
#include <math.h>
#include "birm_arm_float_User.h"

int birm_cvabs_f(const float *x, int nx, float *y) {
    if (x == NULL || y == NULL) return -1;
    if (nx <= 0) return -2;

    int i = 0;

    // 使用NEON处理主要部分 (每次处理4个复数 = 8个float)
    if (nx >= 4) {
        for (; i <= nx - 4; i += 4) {
            // 加载4个复数 (8个float)
            float32x4x2_t complex_data = vld2q_f32(x + i * 2);

            // 获取实部和虚部向量
            float32x4_t real_vec = complex_data.val[0];
            float32x4_t imag_vec = complex_data.val[1];

            // 计算平方
            float32x4_t real_sq = vmulq_f32(real_vec, real_vec);
            float32x4_t imag_sq = vmulq_f32(imag_vec, imag_vec);

            // 计算平方和
            float32x4_t sum_sq = vaddq_f32(real_sq, imag_sq);

            // 计算平方根
            float32x4_t magnitude = vsqrtq_f32(sum_sq);

            // 存储结果
            vst1q_f32(y + i, magnitude);
        }
    }

    // 处理剩余复数
    for (; i < nx; ++i) {
        float real = x[i * 2];
        float imag = x[i * 2 + 1];
        y[i] = sqrtf(real * real + imag * imag);
    }

    return 0;
}

