#include <arm_neon.h>
#include "birm_arm_float_User.h"

int birm_vsubs_f(const float* x1, const float x2, const int nx, float* y)
{
    if (!x1 || !y) return birmParamNullError;
    if (nx <= 0) return birmParamLengthInvalidError;

    int n_loop = nx;
    
    // 预加载标量 x2 到 v30
    asm volatile (
        "dup v30.4s, %w[val]" 
        : 
        : [val] "r" (x2) 
        : "v30"
    );

    const float *src = x1;
    float *dst = y;

    // ---------------------------------------------------------
    // 主循环：一次处理 64 个 float (256 字节)
    // 策略：批量加载 -> 批量计算 -> 批量存储
    // ---------------------------------------------------------
    while (n_loop >= 64) {
        asm volatile (
            // === 阶段 1: 疯狂加载 (利用内存带宽) ===
            // 预取下一轮数据 (距离当前约 256 字节)
            "prfm pldl1keep, [%[in], #256] \n\t"

            // 加载 Group A (v0-v3)
            "ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[in]], #64 \n\t"
            // 加载 Group B (v4-v7)
            "ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[in]], #64 \n\t"
            // 加载 Group C (v8-v11) - 此时 Group A 的数据应该已经这就绪了
            "ld1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%[in]], #64 \n\t"
            // 加载 Group D (v12-v15)
            "ld1 {v12.4s, v13.4s, v14.4s, v15.4s}, [%[in]], #64 \n\t"

            // === 阶段 2: 计算 (此时 v0-v7 肯定已经在寄存器中了，无延迟) ===
            "fsub v0.4s, v0.4s, v30.4s \n\t"
            "fsub v1.4s, v1.4s, v30.4s \n\t"
            "fsub v2.4s, v2.4s, v30.4s \n\t"
            "fsub v3.4s, v3.4s, v30.4s \n\t"
            
            "fsub v4.4s, v4.4s, v30.4s \n\t"
            "fsub v5.4s, v5.4s, v30.4s \n\t" // 插入计算 Group B
            "fsub v6.4s, v6.4s, v30.4s \n\t"
            "fsub v7.4s, v7.4s, v30.4s \n\t"

            // === 阶段 3: 交叉 存储与计算 (掩盖写回延迟) ===
            // 存储 Group A
            "st1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[out]], #64 \n\t"
            
            // 计算 Group C (v8-v11)
            "fsub v8.4s, v8.4s, v30.4s \n\t"
            "fsub v9.4s, v9.4s, v30.4s \n\t"
            "fsub v10.4s, v10.4s, v30.4s \n\t"
            "fsub v11.4s, v11.4s, v30.4s \n\t"

            // 存储 Group B
            "st1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[out]], #64 \n\t"

            // 计算 Group D (v12-v15)
            "fsub v12.4s, v12.4s, v30.4s \n\t"
            "fsub v13.4s, v13.4s, v30.4s \n\t"
            "fsub v14.4s, v14.4s, v30.4s \n\t"
            "fsub v15.4s, v15.4s, v30.4s \n\t"

            // 存储 Group C
            "st1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%[out]], #64 \n\t"
            // 存储 Group D
            "st1 {v12.4s, v13.4s, v14.4s, v15.4s}, [%[out]], #64 \n\t"

            : [in] "+r" (src), [out] "+r" (dst)
            : 
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", 
              "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15", "v30"
        );
        n_loop -= 64;
    }

    // 处理剩余块 (标准循环处理剩余部分)
    while (n_loop >= 4) {
        asm volatile (
            "ld1 {v0.4s}, [%[in]], #16 \n\t"
            "fsub v0.4s, v0.4s, v30.4s \n\t"
            "st1 {v0.4s}, [%[out]], #16 \n\t"
            : [in] "+r" (src), [out] "+r" (dst)
            : 
            : "memory", "v0"
        );
        n_loop -= 4;
    }

    // 处理尾部
    while (n_loop > 0) {
        *dst++ = *src++ - x2;
        n_loop--;
    }

    return birmSuccess;
}