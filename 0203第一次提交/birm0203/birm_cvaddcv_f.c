
#include "birm_arm_float_User.h"
#include "armpl.h"
#include <arm_neon.h> // 引入 NEON 指令集头文件
//20251209 小数据量较快的
/**
 * @brief 复数矢量和复数矢量的加法 (优化版)
 * 优化策略：
 * 1. 移除 cblas_swaxpby 的函数调用开销。
 * 2. 移除 alpha=1.0 时的冗余浮点乘法运算。
 * 3. 使用 ARM NEON SIMD 指令集进行 128位 并行计算 (一次处理2个复数/4个float)。
 */
int birm_cvaddcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    // 参数检查
    if (!x1 || !y || !x2) { return birmParamNullError; }
    if (nx <= 0) { return birmParamLengthInvalidError; }

    // 复数长度转为 float 长度
    int len = nx * 2;
    int i = 0;

    // -------------------------------------------------------
    // ARM NEON SIMD 优化部分
    // -------------------------------------------------------
    // 每次循环处理 4 个 float (即 2 个复数)
    // 剩余不足 4 个的数据将在后续处理
    int loop_end = len & ~3; // 等同于 len - (len % 4)

    for (; i < loop_end; i += 4) {
        // 加载数据到 NEON 寄存器 (128-bit)
        float32x4_t va = vld1q_f32(x1 + i);
        float32x4_t vb = vld1q_f32(x2 + i);
        
        // 执行矢量加法: vy = vx1 + vx2
        float32x4_t vy = vaddq_f32(va, vb);
        
        // 将结果存回内存
        vst1q_f32(y + i, vy);
    }

    // -------------------------------------------------------
    // 处理剩余的尾部数据 (0-3 个 float)
    // -------------------------------------------------------
    for (; i < len; i++) {
        y[i] = x1[i] + x2[i];
    }

    return birmSuccess;
}
