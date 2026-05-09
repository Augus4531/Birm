#include <arm_neon.h>
#include "birm_arm_float_User.h"

/**
 * 采用 64路循环展开 + 流水线指令重排
 */
int birm_ssubv_f(const float x1, const float *x2, const int nx, float *y)
{
    if (!x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int n_loop = nx;

    // 1. 将标量 x1 广播到 v30 寄存器
    // v30 将作为被减数 (Minuend)
    asm volatile(
        "dup v30.4s, %w[val]"
        :
        : [val] "r"(x1)
        : "v30");

    const float *src = x2;
    float *dst = y;

    // ---------------------------------------------------------
    // 主循环：一次处理 64 个 float (256 字节)
    // ---------------------------------------------------------
    while (n_loop >= 64)
    {
        asm volatile(
            "ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[in]], #64 \n\t"
            "ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[in]], #64 \n\t"
            "ld1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%[in]], #64 \n\t"
            "ld1 {v12.4s, v13.4s, v14.4s, v15.4s}, [%[in]], #64 \n\t"

            // === Calc 阶段 (注意：x1 是被减数) ===
            "fsub v0.4s, v30.4s, v0.4s \n\t"
            "fsub v1.4s, v30.4s, v1.4s \n\t"
            "fsub v2.4s, v30.4s, v2.4s \n\t"
            "fsub v3.4s, v30.4s, v3.4s \n\t"

            "fsub v4.4s, v30.4s, v4.4s \n\t"
            "fsub v5.4s, v30.4s, v5.4s \n\t"
            "fsub v6.4s, v30.4s, v6.4s \n\t"
            "fsub v7.4s, v30.4s, v7.4s \n\t"

            // === Store/Calc 交叉 (掩盖写延迟) ===
            "st1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[out]], #64 \n\t"

            "fsub v8.4s,  v30.4s, v8.4s \n\t"
            "fsub v9.4s,  v30.4s, v9.4s \n\t"
            "fsub v10.4s, v30.4s, v10.4s \n\t"
            "fsub v11.4s, v30.4s, v11.4s \n\t"

            "st1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[out]], #64 \n\t"

            "fsub v12.4s, v30.4s, v12.4s \n\t"
            "fsub v13.4s, v30.4s, v13.4s \n\t"
            "fsub v14.4s, v30.4s, v14.4s \n\t"
            "fsub v15.4s, v30.4s, v15.4s \n\t"

            "st1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%[out]], #64 \n\t"
            "st1 {v12.4s, v13.4s, v14.4s, v15.4s}, [%[out]], #64 \n\t"

            : [in] "+r"(src), [out] "+r"(dst)
            :
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15", "v30");
        n_loop -= 64;
    }

    // 2. 处理剩余块 (4对齐)
    while (n_loop >= 4)
    {
        asm volatile(
            "ld1 {v0.4s}, [%[in]], #16 \n\t"
            "fsub v0.4s, v30.4s, v0.4s \n\t" // Scalar - Vector
            "st1 {v0.4s}, [%[out]], #16 \n\t"
            : [in] "+r"(src), [out] "+r"(dst)
            :
            : "memory", "v0");
        n_loop -= 4;
    }

    // 3. 处理尾部残余
    while (n_loop > 0)
    {
        *dst++ = x1 - *src++;
        n_loop--;
    }

    return birmSuccess;
}
