#include "birm_arm_float_User.h"
#include <arm_neon.h>

/**
 * @brief 两个实数矢量比较 (终极优化版)
 * * 优化策略:
 * 1. 4路循环展开 (64字节/次): 减少分支预测压力，提升指令发射率。
 * 2. 软件预取 (__builtin_prefetch): 解决图2/3中的 ldp 瓶颈。
 * 预取距离设定为 256 字节 (约4个循环步长)，确保数据提前到达 L1 Cache。
 * 3. 限制指针别名 (__restrict): 允许编译器进行激进的指令重排。
 */
int birm_vlg_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    // 准备常量 1.0f
    const float32x4_t v_one = vdupq_n_f32(1.0f);

// --- 预取距离设置 ---
// 经验值：对于顺序读取，通常预取前方 192~256 字节效果最好。
// 这可以让内存控制器在 CPU 处理当前数据时，并行搬运未来的数据。
#define PREFETCH_OFFSET 256

    // 主循环：每次处理 16 个 float (64 字节)
    for (i = 0; i <= nx - 16; i += 16)
    {
        // [关键优化] 软件预取
        // 告诉 CPU: "我现在在处理 i，但请把 i + 256 处的数据先搬到 L1 Cache"
        // 1: Read access, 3: High temporal locality (也就是告诉CPU这数据还要用)
        __builtin_prefetch(x1 + i + PREFETCH_OFFSET, 0, 3);
        __builtin_prefetch(x2 + i + PREFETCH_OFFSET, 0, 3);

        // [阶段1] 加载数据 (Load)
        // 编译器会自动合并为 ldp (Load Pair) 指令
        float32x4_t vx1_0 = vld1q_f32(x1 + i);
        float32x4_t vx1_1 = vld1q_f32(x1 + i + 4);
        float32x4_t vx2_0 = vld1q_f32(x2 + i);
        float32x4_t vx2_1 = vld1q_f32(x2 + i + 4);

        float32x4_t vx1_2 = vld1q_f32(x1 + i + 8);
        float32x4_t vx1_3 = vld1q_f32(x1 + i + 12);
        float32x4_t vx2_2 = vld1q_f32(x2 + i + 8);
        float32x4_t vx2_3 = vld1q_f32(x2 + i + 12);

        // [阶段2] 比较运算 (Compare) - fcmge
        // 由于上面一次性发射了8条加载指令，这里的比较指令可以利用流水线空隙执行
        // 解决了图1中的依赖阻塞
        uint32x4_t mask0 = vcgeq_f32(vx1_0, vx2_0);
        uint32x4_t mask1 = vcgeq_f32(vx1_1, vx2_1);
        uint32x4_t mask2 = vcgeq_f32(vx1_2, vx2_2);
        uint32x4_t mask3 = vcgeq_f32(vx1_3, vx2_3);

        // [阶段3] 结果生成 (Bitwise AND)
        float32x4_t vy0 = vreinterpretq_f32_u32(vandq_u32(mask0, vreinterpretq_u32_f32(v_one)));
        float32x4_t vy1 = vreinterpretq_f32_u32(vandq_u32(mask1, vreinterpretq_u32_f32(v_one)));
        float32x4_t vy2 = vreinterpretq_f32_u32(vandq_u32(mask2, vreinterpretq_u32_f32(v_one)));
        float32x4_t vy3 = vreinterpretq_f32_u32(vandq_u32(mask3, vreinterpretq_u32_f32(v_one)));

        // [阶段4] 存储结果 (Store)
        vst1q_f32(y + i, vy0);
        vst1q_f32(y + i + 4, vy1);
        vst1q_f32(y + i + 8, vy2);
        vst1q_f32(y + i + 12, vy3);
    }

    // 处理剩余元素 (0-15个)
    for (; i < nx; i++)
    {
        y[i] = (x1[i] >= x2[i]) ? 1.0f : 0.0f;
    }

    return birmSuccess;
}