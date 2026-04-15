
#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "birm_arm_float_User.h"

int birm_vsum_f(const float *x, const int nx, float *y) {
    // 1. 参数安全校验
    if (x == NULL || y == NULL) 
        return -1;
    if (nx <= 0)
        return -2;
    
    // 2. 调试模式下的对齐检查
    #ifndef NDEBUG
    if((((uintptr_t)x) & 0xF) != 0) fprintf(stderr, "x not 16-byte aligned\n");
    if((((uintptr_t)y) & 0xF) != 0) fprintf(stderr, "y not 16-byte aligned\n");
    #endif
    
    // 3. 正式对齐声明
    const float* aligned_x = (const float*)__builtin_assume_aligned(x, 16);
    float sum = 0.0f;
    int idx = 0;

    // 使用多个NEON向量来减少依赖链，提高性能
    float32x4_t sum_vec1 = vdupq_n_f32(0.0f);
    float32x4_t sum_vec2 = vdupq_n_f32(0.0f);
    float32x4_t sum_vec3 = vdupq_n_f32(0.0f);
    float32x4_t sum_vec4 = vdupq_n_f32(0.0f);
    
    const int block_size = 16;  // 每次处理16个元素

    // 主循环：每次处理16个元素
    for (; idx <= nx - block_size; idx += block_size) {
        // 预取下一批数据
        __builtin_prefetch(aligned_x + idx + 32, 0, 0);
        
        // 加载4组数据，每组4个浮点数
        float32x4_t data1 = vld1q_f32(aligned_x + idx);
        float32x4_t data2 = vld1q_f32(aligned_x + idx + 4);
        float32x4_t data3 = vld1q_f32(aligned_x + idx + 8);
        float32x4_t data4 = vld1q_f32(aligned_x + idx + 12);
        
        // 分别累加到4个累加器
        sum_vec1 = vaddq_f32(sum_vec1, data1);
        sum_vec2 = vaddq_f32(sum_vec2, data2);
        sum_vec3 = vaddq_f32(sum_vec3, data3);
        sum_vec4 = vaddq_f32(sum_vec4, data4);
    }
    
    // 合并4个累加器
    sum_vec1 = vaddq_f32(sum_vec1, sum_vec2);
    sum_vec3 = vaddq_f32(sum_vec3, sum_vec4);
    sum_vec1 = vaddq_f32(sum_vec1, sum_vec3);
    
    // 将NEON向量的4个元素相加得到部分和
    sum = vaddvq_f32(sum_vec1);
    
    // 处理剩余元素（不足16个）
    for (; idx < nx; idx++) {
        sum += aligned_x[idx];
    }
    
    *y = sum;
    return 0;
}
