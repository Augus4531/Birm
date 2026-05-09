#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// 辅助宏：使用FMA计算 a + b * c
#if defined(__aarch64__)
#define VMLA(a, b, c) vfmaq_f32(a, b, c)
#define VMLS(a, b, c) vfmsq_f32(a, b, c) // a - b * c
#else
#define VMLA(a, b, c) vmlaq_f32(a, b, c)
#define VMLS(a, b, c) vmlsq_f32(a, b, c)
#endif

// 测试通过
int birm_cvdivcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (x1 == NULL || x2 == NULL || y == NULL)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    int idx = 0;
    const float epsilon = 1e-20f;
    const float32x4_t vepsilon = vdupq_n_f32(epsilon);
    const float32x4_t vtwo = vdupq_n_f32(2.0f);

    // 优化策略：
    // 1. 将循环展开从8个复数减少为4个复数。
    //    这减少了寄存器压力，允许编译器更好地进行指令重排，通常在L1/L2边界大小(如8K)时表现更稳健。
    // 2. 使用 FMA (Fused Multiply-Add) 指令减少指令延迟。

    for (; idx <= nx - 4; idx += 4)
    {
        // 加载 x1 (4个复数, 8个float)
        float32x4x2_t vx1 = vld2q_f32(x1 + idx * 2);
        // 加载 x2 (4个复数, 8个float)
        float32x4x2_t vx2 = vld2q_f32(x2 + idx * 2);

        // --- 1. 计算分母: |x2|^2 = real^2 + imag^2 ---
        // 使用乘法开始
        float32x4_t denom = vmulq_f32(vx2.val[0], vx2.val[0]);
        // 使用 FMA 累加虚部平方: denom = denom + (imag * imag)
        denom = VMLA(denom, vx2.val[1], vx2.val[1]);

        // 避免除零
        denom = vmaxq_f32(denom, vepsilon);

        // --- 2. 计算分母倒数 (Reciprocal) ---
        // 估算值
        float32x4_t recip = vrecpeq_f32(denom);
        // Newton-Raphson 迭代: x_{i+1} = x_i * (2 - d * x_i)
        // 优化写法: 2 - (d * x_i) 可以用 VMLS 实现 -> VMLS(vtwo, denom, recip)
        recip = vmulq_f32(recip, VMLS(vtwo, denom, recip));

        // --- 3. 计算分子 ---
        // 实部: x1_r * x2_r + x1_i * x2_i
        float32x4_t num_real = vmulq_f32(vx1.val[0], vx2.val[0]);
        num_real = VMLA(num_real, vx1.val[1], vx2.val[1]);

        // 虚部: x1_i * x2_r - x1_r * x2_i
        float32x4_t num_imag = vmulq_f32(vx1.val[1], vx2.val[0]);
        num_imag = VMLS(num_imag, vx1.val[0], vx2.val[1]); // num_imag - (x1_r * x2_i)

        // --- 4. 最终乘法: 分子 * (1/分母) ---
        float32x4_t res_real = vmulq_f32(num_real, recip);
        float32x4_t res_imag = vmulq_f32(num_imag, recip);

        // --- 5. 存储 ---
        float32x4x2_t vy = {{res_real, res_imag}};
        vst2q_f32(y + idx * 2, vy);
    }

    // 尾部处理 (Scalar loop)
    for (; idx < nx; idx++)
    {
        float x1_r = x1[idx * 2];
        float x1_i = x1[idx * 2 + 1];
        float x2_r = x2[idx * 2];
        float x2_i = x2[idx * 2 + 1];

        float d = x2_r * x2_r + x2_i * x2_i;
        if (d < epsilon)
            d = epsilon;
        float inv_d = 1.0f / d;

        y[idx * 2] = (x1_r * x2_r + x1_i * x2_i) * inv_d;
        y[idx * 2 + 1] = (x1_i * x2_r - x1_r * x2_i) * inv_d;
    }

    return birmSuccess;
}
