#include <math.h>
#include <stddef.h>
#include "birm_arm_float_User.h"

// 包含ARM性能数学库头文件
#include "amath.h"

// 使用ARM性能库的高精度向量化正弦函数
int birm_vsin_f(const float *x, const int nx, float *y) {
    // 参数检查
    if (x == NULL || y == NULL) {
        return -1; // 输入指针x或输出指针y为空
    }
    
    if (nx <= 0) {
        return -2; // 输入长度nx不大于0
    }
    
    int i = 0;
    
    // 如果支持Neon向量化，使用向量化版本
    #if defined(__ARM_NEON) || defined(__aarch64__)
    // 处理向量化部分（每次处理4个元素）
    const int vectorizable_elements = nx & ~3;
    
    for (; i < vectorizable_elements; i += 4) {
        // 加载4个float到Neon寄存器
        float32x4_t vec = vld1q_f32(x + i);
        
        // 使用ARM性能库的高精度向量正弦函数
        float32x4_t result = armpl_vsinq_f32_u10(vec);
        
        // 存储结果
        vst1q_f32(y + i, result);
    }
    #endif
    
    // 处理剩余元素（使用标量函数）
    for (; i < nx; i++) {
        y[i] = armpl_sin_f32(x[i]); // 使用ARM性能库的标量正弦函数
    }
    
    return 0; // 正常返回
}
