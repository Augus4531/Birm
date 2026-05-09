#include "birm_arm_float_User.h"
#include <arm_neon.h>

/**
 * @brief 浮点实数矢量与浮点实数标量的比较，输出较大值 (NEON 优化版)
 * 相当于 y[i] = max(x1[i], x2)
 */
int birm_vthrsh_f(const float *x1, const float x2, const int nx, float *y)
{
    // 1. 参数检查
    if (!x1 || !y) { return birmParamNullError; }
    if (nx <= 0) { return birmParamLengthInvalidError; }

    int i = 0;
    
    // 2. 准备阶段：将标量 x2 广播到向量寄存器中
    // v_threshold = {x2, x2, x2, x2}
    float32x4_t v_threshold = vdupq_n_f32(x2);

    // 计算主循环次数，每次处理 16 个数据
    int blk_cnt = nx >> 4; 

    const float * __restrict p_x1 = x1;
    float * __restrict p_y = y;

    // 3. 主循环：NEON 并行处理 + 循环展开
    while (blk_cnt > 0)
    {
        // Load: 加载 16 个 x1 元素
        float32x4_t va1 = vld1q_f32(p_x1);
        float32x4_t va2 = vld1q_f32(p_x1 + 4);
        float32x4_t va3 = vld1q_f32(p_x1 + 8);
        float32x4_t va4 = vld1q_f32(p_x1 + 12);

        // Max: 与阈值向量进行比较取大值 (硬件直接支持，无 if-else 分支)
        float32x4_t vres1 = vmaxq_f32(va1, v_threshold);
        float32x4_t vres2 = vmaxq_f32(va2, v_threshold);
        float32x4_t vres3 = vmaxq_f32(va3, v_threshold);
        float32x4_t vres4 = vmaxq_f32(va4, v_threshold);

        // Store: 存储结果
        vst1q_f32(p_y, vres1);
        vst1q_f32(p_y + 4, vres2);
        vst1q_f32(p_y + 8, vres3);
        vst1q_f32(p_y + 12, vres4);

        // 指针推进
        p_x1 += 16;
        p_y  += 16;
        blk_cnt--;
        i += 16;
    }

    // 4. 尾部处理：处理剩余的数据 (少于 16 个)
    for (; i < nx; i++)
    {
        // 标量部分也可以使用三目运算符，编译器优化后通常指令较少
        y[i] = (x1[i] > x2) ? x1[i] : x2;
    }

    return birmSuccess;
}
