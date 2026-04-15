#include "birm_arm_float_User.h"
#include <arm_neon.h> // 必须引入 ARM NEON 头文件

int birm_vmaxv_f(const float *x1, const float *x2, int nx, float *y) {
    // 参数检查
    if (x1 == NULL || x2 == NULL || y == NULL) return -1;
    if (nx <= 0) return -2;
    
    // 确保内存对齐（64字节对齐，匹配缓存行）
    if (((uintptr_t)x1 & 0x3F) != 0 || ((uintptr_t)x2 & 0x3F) != 0 || ((uintptr_t)y & 0x3F) != 0) {
        // 如果未对齐，回退到标量处理
        for (int i = 0; i < nx; i++) {
            y[i] = (x1[i] > x2[i]) ? x1[i] : x2[i];
        }
        return 0;
    }
    
    int i = 0;
    for (; i <= nx - 8; i += 8) {
        float32x4_t x1_1 = vld1q_f32(x1 + i);
        float32x4_t x1_2 = vld1q_f32(x1 + i + 4);
        float32x4_t x2_1 = vld1q_f32(x2 + i);
        float32x4_t x2_2 = vld1q_f32(x2 + i + 4);
        
        float32x4_t result1 = vmaxq_f32(x1_1, x2_1);
        float32x4_t result2 = vmaxq_f32(x1_2, x2_2);
        
        vst1q_f32(y + i, result1);
        vst1q_f32(y + i + 4, result2);
    }
    // 处理剩余标量数据
    for (; i < nx; i++) {
        y[i] = (x1[i] > x2[i]) ? x1[i] : x2[i];
    }
    
    return 0;
}

