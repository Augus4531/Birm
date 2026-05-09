#include <arm_neon.h>
#include "birm_arm_float_User.h"

int birm_vreversal_f(const float * __restrict x, const int nx, float * __restrict y)
{
    if (!y || !x) return birmParamNullError;
    if (nx <= 0) return birmParamLengthInvalidError;

    int nxHalf = nx >> 1;
    int odd = nx & 1;

    // 处理奇数情况的中心点
    if (odd) {
        y[nxHalf] = x[nxHalf];
    }

    // 定义 TBL 映射表：将 16 字节完全反转
    // 映射关系：[12,13,14,15, 8,9,10,11, 4,5,6,7, 0,1,2,3]
    static const uint8_t rev_mask[16] = {12,13,14,15, 8,9,10,11, 4,5,6,7, 0,1,2,3};
    uint8x16_t mask = vld1q_u8(rev_mask);

    int i = 0;
    // 每次处理 4 个 float
    for (; i <= nxHalf - 4; i += 4) {
        // --- 处理 Group 1: 从末尾反转到开头 ---
        float32x4_t v_back = vld1q_f32(x + nx - i - 4);
        // 使用汇编直接调用 tbl
        float32x4_t res_front;
        __asm__ ("tbl %0.16b, {%1.16b}, %2.16b" 
                 : "=w"(res_front) 
                 : "w"(v_back), "w"(mask));
        vst1q_f32(y + i, res_front);

        // --- 处理 Group 2: 从中间反转到后部 ---
        float32x4_t v_mid = vld1q_f32(x + nxHalf - i - 4);
        float32x4_t res_back;
        __asm__ ("tbl %0.16b, {%1.16b}, %2.16b" 
                 : "=w"(res_back) 
                 : "w"(v_mid), "w"(mask));
        vst1q_f32(y + nxHalf + odd + i, res_back);
    }

    // 标量残余处理
    for (; i < nxHalf; ++i) {
        y[i] = x[nx - i - 1];
        y[i + nxHalf + odd] = x[nxHalf - 1 - i];
    }

    return birmSuccess;
}

