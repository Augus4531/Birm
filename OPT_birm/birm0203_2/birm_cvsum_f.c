#include <arm_neon.h>
#include <stddef.h>
#include <math.h>
#include "birm_arm_float_User.h"

int birm_cvsum_f(const float *x, int nx, float *y) {
    // 参数检查
    if (x == NULL || y == NULL) {
        return -1;
    }
    
    if (nx <= 0) {
        return -2;
    }
    
    // 初始化累加器
    float sum_real = 0.0f;
    float sum_imag = 0.0f;
    
    int i = 0;
    
    // 使用NEON处理主要部分 (每次处理4个复数 = 8个float)
    if (nx >= 4) {
        // 初始化NEON累加器为0
        float32x4_t sum_real_vec = vdupq_n_f32(0.0f);
        float32x4_t sum_imag_vec = vdupq_n_f32(0.0f);
        
        for (; i <= nx - 4; i += 4) {
            // 加载4个复数 (8个float)
            // 使用交错加载: 实部和虚部分离到不同的寄存器
            float32x4x2_t complex_data = vld2q_f32(x + i * 2);
            
            // 累加实部和虚部
            sum_real_vec = vaddq_f32(sum_real_vec, complex_data.val[0]);
            sum_imag_vec = vaddq_f32(sum_imag_vec, complex_data.val[1]);
        }
        
        // 将向量累加器归约为标量
        // 水平相加: 将4个元素相加为1个
        float32x4_t sum_real_temp = vaddq_f32(sum_real_vec, 
                                            vcombine_f32(vget_high_f32(sum_real_vec), 
                                                       vget_low_f32(sum_real_vec)));
        sum_real_temp = vaddq_f32(sum_real_temp, 
                                vrev64q_f32(sum_real_temp));
        sum_real = vgetq_lane_f32(sum_real_temp, 0);
        
        float32x4_t sum_imag_temp = vaddq_f32(sum_imag_vec, 
                                            vcombine_f32(vget_high_f32(sum_imag_vec), 
                                                       vget_low_f32(sum_imag_vec)));
        sum_imag_temp = vaddq_f32(sum_imag_temp, 
                                vrev64q_f32(sum_imag_temp));
        sum_imag = vgetq_lane_f32(sum_imag_temp, 0);
    }
    
    // 处理剩余的复数 (不足4个)
    for (; i < nx; ++i) {
        // 计算数组索引 (每个复数占2个float)
        int data_idx = i * 2;
        sum_real += x[data_idx];      // 实部
        sum_imag += x[data_idx + 1];  // 虚部
    }
    
    // 存储结果
    y[0] = sum_real;
    y[1] = sum_imag;
    
    return 0;
}
