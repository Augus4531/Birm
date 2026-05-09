#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
/**
 * @brief 浮点单精度实数矢量绝对值求和 (NEON 深度优化版)
 */
int birm_vsumm_f(const float *x, const int nx, float *y)
{
    // 1. 参数检查
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 初始化累加器
    float sum_scalar = 0.0f;
    int i = 0;

    // 2. NEON 优化部分
    // 只有当数据量 >= 16 时才进入主循环，利用 4 路循环展开
    if (nx >= 16)
    {
        // 定义 4 个向量累加器，利用指令级并行 (ILP)
        // 这样 CPU 可以并行执行这 4 个变量的加法，而不是等待上一个指令完成
        float32x4_t sum_vec0 = vdupq_n_f32(0.0f);
        float32x4_t sum_vec1 = vdupq_n_f32(0.0f);
        float32x4_t sum_vec2 = vdupq_n_f32(0.0f);
        float32x4_t sum_vec3 = vdupq_n_f32(0.0f);

        // 每次处理 16 个 float
        for (; i <= nx - 16; i += 16)
        {
            // 加载数据 (ARMv8 支持非对齐加载，无需特殊处理)
            float32x4_t v0 = vld1q_f32(x + i);
            float32x4_t v1 = vld1q_f32(x + i + 4);
            float32x4_t v2 = vld1q_f32(x + i + 8);
            float32x4_t v3 = vld1q_f32(x + i + 12);

            // 计算绝对值并累加
            // vabsq_f32: 向量绝对值
            // vaddq_f32: 向量加法
            sum_vec0 = vaddq_f32(sum_vec0, vabsq_f32(v0));
            sum_vec1 = vaddq_f32(sum_vec1, vabsq_f32(v1));
            sum_vec2 = vaddq_f32(sum_vec2, vabsq_f32(v2));
            sum_vec3 = vaddq_f32(sum_vec3, vabsq_f32(v3));

            // 提示：如果数据量极大，可以在此处加入 __builtin_prefetch
        }

        // 将 4 个累加器合并为 1 个
        float32x4_t sum_vec = vaddq_f32(vaddq_f32(sum_vec0, sum_vec1), vaddq_f32(sum_vec2, sum_vec3));

        // 水平求和：将向量中的 4 个 float 相加得到标量
        // vaddvq_f32 是 ARMv8 特有的高效指令
        sum_scalar += vaddvq_f32(sum_vec);
    }

    // 3. 处理剩余的元素 (Tail processing)
    // 处理不足 16 个的剩余部分 (或者 nx 本身就很小的情况)
    for (; i < nx; i++)
    {
        // 使用 fabsf 确保编译器生成最高效的标量绝对值指令
        sum_scalar += fabsf(x[i]);
    }

    // 4. 输出结果
    y[0] = sum_scalar;

    return birmSuccess;
}
