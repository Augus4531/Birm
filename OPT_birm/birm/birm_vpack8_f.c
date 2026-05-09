
#include <arm_neon.h>
#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数完成32bit数据压缩为8bit (ARM64 NEON Pipeline Interleaved)
 * 策略:
 * 1. 64路循环展开
 * 2. Load/Compute/Store 完全交错 (Ping-Pong)
 * 3. 利用寄存器堆掩盖 Cache Miss 和 Store Buffer 延迟
 */
int birm_vpack8_f(const float *x, const int nx, int *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    const float *__restrict src = x;
    int8_t *__restrict dst = (int8_t *)y;

    int n_loop = nx >> 6; // nx / 64
    int rem = nx & 63;    // nx % 64

    // =========================================================
    // 核心循环：64路交错处理
    // 逻辑顺序：Load A -> Load B -> Convert A -> Load C -> Convert B -> Store A ...
    // =========================================================
    for (int i = 0; i < n_loop; i++)
    {
        // --- 1. Load Block 0 (0-15) ---
        float32x4_t v0 = vld1q_f32(src);
        float32x4_t v1 = vld1q_f32(src + 4);
        float32x4_t v2 = vld1q_f32(src + 8);
        float32x4_t v3 = vld1q_f32(src + 12);

        // --- 2. Load Block 1 (16-31) ---
        // 紧接着加载，不给计算指令插队机会，利用内存并行性
        float32x4_t v4 = vld1q_f32(src + 16);
        float32x4_t v5 = vld1q_f32(src + 20);
        float32x4_t v6 = vld1q_f32(src + 24);
        float32x4_t v7 = vld1q_f32(src + 28);

        // --- 3. Convert Block 0 (计算掩盖 Block 1 的 Load 延迟) ---
        int16x8_t vn0 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v0)), vmovn_s32(vcvtq_s32_f32(v1)));
        int16x8_t vn1 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v2)), vmovn_s32(vcvtq_s32_f32(v3)));
        int8x16_t res0 = vcombine_s8(vmovn_s16(vn0), vmovn_s16(vn1));

        // --- 4. Load Block 2 (32-47) ---
        // 再次发起加载，利用 Block 0 的计算时间窗口
        float32x4_t v8 = vld1q_f32(src + 32);
        float32x4_t v9 = vld1q_f32(src + 36);
        float32x4_t v10 = vld1q_f32(src + 40);
        float32x4_t v11 = vld1q_f32(src + 44);

        // --- 5. Store Block 0 (此时 Block 0 结果早已准备好) ---
        vst1q_s8(dst, res0);

        // --- 6. Convert Block 1 (计算掩盖 Block 2 的 Load 延迟) ---
        int16x8_t vn2 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v4)), vmovn_s32(vcvtq_s32_f32(v5)));
        int16x8_t vn3 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v6)), vmovn_s32(vcvtq_s32_f32(v7)));
        int8x16_t res1 = vcombine_s8(vmovn_s16(vn2), vmovn_s16(vn3));

        // --- 7. Load Block 3 (48-63) ---
        float32x4_t v12 = vld1q_f32(src + 48);
        float32x4_t v13 = vld1q_f32(src + 52);
        float32x4_t v14 = vld1q_f32(src + 56);
        float32x4_t v15 = vld1q_f32(src + 60);

        // --- 8. Store Block 1 ---
        vst1q_s8(dst + 16, res1);

        // --- 9. Convert Block 2 ---
        int16x8_t vn4 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v8)), vmovn_s32(vcvtq_s32_f32(v9)));
        int16x8_t vn5 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v10)), vmovn_s32(vcvtq_s32_f32(v11)));
        int8x16_t res2 = vcombine_s8(vmovn_s16(vn4), vmovn_s16(vn5));

        // --- 10. Store Block 2 ---
        vst1q_s8(dst + 32, res2);

        // --- 11. Convert Block 3 ---
        int16x8_t vn6 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v12)), vmovn_s32(vcvtq_s32_f32(v13)));
        int16x8_t vn7 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v14)), vmovn_s32(vcvtq_s32_f32(v15)));
        int8x16_t res3 = vcombine_s8(vmovn_s16(vn6), vmovn_s16(vn7));

        // --- 12. Store Block 3 ---
        vst1q_s8(dst + 48, res3);

        // 更新指针
        src += 64;
        dst += 64;
    }

    // =========================================================
    // 剩余部分处理 (保持 V5 的高效逻辑)
    // =========================================================
    while (rem >= 16)
    {
        float32x4_t v0 = vld1q_f32(src);
        float32x4_t v1 = vld1q_f32(src + 4);
        float32x4_t v2 = vld1q_f32(src + 8);
        float32x4_t v3 = vld1q_f32(src + 12);

        int16x8_t vn0 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v0)), vmovn_s32(vcvtq_s32_f32(v1)));
        int16x8_t vn1 = vcombine_s16(vmovn_s32(vcvtq_s32_f32(v2)), vmovn_s32(vcvtq_s32_f32(v3)));
        int8x16_t res = vcombine_s8(vmovn_s16(vn0), vmovn_s16(vn1));

        vst1q_s8(dst, res);
        src += 16;
        dst += 16;
        rem -= 16;
    }

    while (rem >= 4)
    {
        int val = ((int)(src[0]) & 0xff) |
                  (((int)(src[1]) & 0xff) << 8) |
                  (((int)(src[2]) & 0xff) << 16) |
                  (((int)(src[3]) & 0xff) << 24);
        *(int *)dst = val;
        src += 4;
        dst += 4;
        rem -= 4;
    }

    if (rem > 0)
    {
        int val = 0;
        if (rem >= 1)
            val |= ((int)(src[0]) & 0xff);
        if (rem >= 2)
            val |= ((int)(src[1]) & 0xff) << 8;
        if (rem == 3)
            val |= ((int)(src[2]) & 0xff) << 16;
        *(int *)dst = val;
    }

    return birmSuccess;
}
