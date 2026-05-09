#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>

#define PREFETCH_DISTANCE 64 // 预取距离，可视情况调整

/**
 * @brief 转浮点实数化为整数 (ARMv8 NEON 优化版)
 * 优化点:
 * 1. 使用 vcvtmq_s32_f32 指令替代 floor() 函数。
 * 该指令直接执行 "Round towards Minus Infinity" (向下取整)，速度极快。
 * 2. 消除 float->double 的隐式转换。
 * 3. 4路循环展开 + 预取。
 */
int birm_vfloat2int_f(const float *x, const int nx, int *y)
{
    if (!x || !y) { return birmParamNullError; }
    if (nx <= 0) { return birmParamLengthInvalidError; }

    int i = 0;
    int blkCnt = nx >> 4; // nx / 16

    const float *pSrc = x;
    int *pDst = y;

    while (blkCnt > 0)
    {
        // --- 预取未来数据 ---
        __builtin_prefetch(pSrc + PREFETCH_DISTANCE, 0, 3);
        
        // --- 加载 16 个 float (4个向量) ---
        float32x4_t f0 = vld1q_f32(pSrc);
        float32x4_t f1 = vld1q_f32(pSrc + 4);
        float32x4_t f2 = vld1q_f32(pSrc + 8);
        float32x4_t f3 = vld1q_f32(pSrc + 12);

        // --- 核心转换 ---
        // vcvtmq_s32_f32: Convert to signed integer, rounding towards Minus infinity
        // 这完全等价于 (int)floor(f)
        int32x4_t i0 = vcvtmq_s32_f32(f0);
        int32x4_t i1 = vcvtmq_s32_f32(f1);
        int32x4_t i2 = vcvtmq_s32_f32(f2);
        int32x4_t i32_3 = vcvtmq_s32_f32(f3); // 命名避免冲突

        // --- 存储结果 ---
        vst1q_s32(pDst, i0);
        vst1q_s32(pDst + 4, i1);
        vst1q_s32(pDst + 8, i2);
        vst1q_s32(pDst + 12, i32_3);

        // --- 更新指针 ---
        pSrc += 16;
        pDst += 16;
        blkCnt--;
    }

    // --- 处理剩余数据 (Tail) ---
    // 计算剩余部分的起始索引
    i = (nx >> 4) << 4;

    for (; i < nx; i++)
    {
        // 优化细节：使用 floorf 而不是 floor
        // floor 是 double 精度，floorf 是 float 精度，避免隐式转换
        y[i] = (int)floorf(x[i]);
    }

    return birmSuccess;
}