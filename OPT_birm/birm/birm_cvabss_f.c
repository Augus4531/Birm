#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <stddef.h>
int birm_cvabss_f(const float *x, const int nx, float *y) {
    // 参数检查
    if (x == NULL || y == NULL) {
        return -1;
    }
    if (nx <= 0) {
        return -2;
    }

    int i = 0;

    // 使用NEON指令集进行向量化优化
    // 每次处理2个复数（4个浮点数：实部1,虚部1,实部2,虚部2）
    for (; i <= nx - 2; i += 2) {
        // 加载4个浮点数：两个复数的实部和虚部
        float32x4_t complex_vec = vld1q_f32(x + 2 * i);

        // 分离实部和虚部
        float32x4_t real_imag = complex_vec;

        // 计算实部的平方和虚部的平方
        float32x4_t square_vec = vmulq_f32(real_imag, real_imag);

        // 提取实部平方和虚部平方
        float32x2_t real_square = vget_low_f32(square_vec);    // 实部1平方, 虚部1平方
        float32x2_t imag_square = vget_high_f32(square_vec);   // 实部2平方, 虚部2平方

        // 对每个复数计算模平方：实部平方 + 虚部平方
        float32x2_t result = vpadd_f32(real_square, imag_square);

        // 存储结果
        vst1_f32(y + i, result);
    }

    // 处理剩余的单个复数
    for (; i < nx; i++) {
        float real = x[2 * i];
        float imag = x[2 * i + 1];
        y[i] = real * real + imag * imag;
    }

    return 0;
}
