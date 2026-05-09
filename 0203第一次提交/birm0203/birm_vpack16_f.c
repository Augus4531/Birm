
#include <arm_neon.h>
#include <stdint.h>
#include "birm_arm_float_User.h"

//2025-12-2
//目前最快
int birm_vpack16_f(const float *x, const int nx, int *y)
{
    if (!x || !y) return birmParamNullError;
    if (nx <= 0) return birmParamLengthInvalidError;

    const float *src = x;
    int16_t *dst = (int16_t *)y;
    int n = nx;

    // ---------------------------------------------------------
    // V8 优化：在 V7 指令精简的基础上，加回软件预取
    // 预取距离：384 字节 (约 96 个 float)
    // ---------------------------------------------------------
    while (n >= 32)
    {
        // 预取策略：提前预取 3 个循环步长之后的数据
        // 32 floats * 4 bytes * 3 steps = 384 bytes
        __builtin_prefetch(src + 96);

        // --- Block 1: Load & Convert ---
        float32x4_t f0 = vld1q_f32(src);
        float32x4_t f1 = vld1q_f32(src + 4);
        float32x4_t f2 = vld1q_f32(src + 8);
        float32x4_t f3 = vld1q_f32(src + 12);

        int32x4_t n0 = vcvtq_s32_f32(f0);
        int32x4_t n1 = vcvtq_s32_f32(f1);
        int32x4_t n2 = vcvtq_s32_f32(f2);
        int32x4_t n3 = vcvtq_s32_f32(f3);

        // --- Block 1: Pack & Store ---
        int16x8_t p01 = vuzp1q_s16(vreinterpretq_s16_s32(n0), vreinterpretq_s16_s32(n1));
        int16x8_t p23 = vuzp1q_s16(vreinterpretq_s16_s32(n2), vreinterpretq_s16_s32(n3));

        vst1q_s16(dst, p01);
        vst1q_s16(dst + 8, p23);

        // --- Block 2: Load & Convert ---
        float32x4_t f4 = vld1q_f32(src + 16);
        float32x4_t f5 = vld1q_f32(src + 20);
        float32x4_t f6 = vld1q_f32(src + 24);
        float32x4_t f7 = vld1q_f32(src + 28);

        int32x4_t n4 = vcvtq_s32_f32(f4);
        int32x4_t n5 = vcvtq_s32_f32(f5);
        int32x4_t n6 = vcvtq_s32_f32(f6);
        int32x4_t n7 = vcvtq_s32_f32(f7);

        // --- Block 2: Pack & Store ---
        int16x8_t p45 = vuzp1q_s16(vreinterpretq_s16_s32(n4), vreinterpretq_s16_s32(n5));
        int16x8_t p67 = vuzp1q_s16(vreinterpretq_s16_s32(n6), vreinterpretq_s16_s32(n7));

        vst1q_s16(dst + 16, p45);
        vst1q_s16(dst + 24, p67);

        src += 32;
        dst += 32;
        n -= 32;
    }

    // 尾部处理 (保持不变)
    while (n >= 4)
    {
        float32x4_t f = vld1q_f32(src);
        int32x4_t i_vec = vcvtq_s32_f32(f);
        int16x4_t s = vmovn_s32(i_vec);
        vst1_s16(dst, s);
        src += 4;
        dst += 4;
        n -= 4;
    }

    int remainder_start_idx = nx - n;
    int *y_int_ptr = y + (remainder_start_idx / 2); 
    
    if (n >= 2) {
        int val = ((int)(src[0]) & 0xffff) | (((int)(src[1]) & 0xffff) << 16);
        *y_int_ptr++ = val;
        src += 2;
        n -= 2;
    }
    
    if (n > 0) {
        int val = (int)(src[0]) & 0xffff;
        *y_int_ptr = val;
    }

    return birmSuccess;
}
