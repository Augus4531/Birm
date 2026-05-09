#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <float.h>
#include <stdint.h>
#include <string.h>
#include <arm_neon.h>

//2025-12-3

// 阈值：128K float 数据量 (即 64K int)
// 64K * 4 bytes = 256KB，通常是 L2 Cache 的分界线
#define LARGE_DATA_THRESHOLD 65536 

int birm_vint16tocv_f(const int *x, const int nx, float *y)
{
    if (!x || !y) { return birmParamNullError; }
    if (nx <= 0) { return birmParamLengthInvalidError; }

    const int16_t *src = (const int16_t *)x;
    float *dst = y;
    int total_len = nx * 2;
    int i = 0;

    // ============================================================
    // 策略 A: 针对 > 128K 的大数据量
    // 重点：加入轻量级预取，保持最大吞吐
    // ============================================================
    if (nx >= LARGE_DATA_THRESHOLD)
    {
        // 每次处理 64 个 float
        for (; i <= total_len - 64; i += 64)
        {
            // [Prefetch] 仅预取 Input 数据
            // 提前 256 字节 (4个 cache line)，这是一个经验值
            // PLD: Preload Data
            __builtin_prefetch(src + i + 128, 0, 0); 
            
            // 1. [LOAD] 并行加载 8 个向量
            int16x8_t in0 = vld1q_s16(src + i);
            int16x8_t in1 = vld1q_s16(src + i + 8);
            int16x8_t in2 = vld1q_s16(src + i + 16);
            int16x8_t in3 = vld1q_s16(src + i + 24);
            int16x8_t in4 = vld1q_s16(src + i + 32);
            int16x8_t in5 = vld1q_s16(src + i + 40);
            int16x8_t in6 = vld1q_s16(src + i + 48);
            int16x8_t in7 = vld1q_s16(src + i + 56);

            // 2. [CONVERT] 纯寄存器操作，无依赖
            float32x4_t f0_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in0)));
            float32x4_t f0_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in0)));
            float32x4_t f1_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in1)));
            float32x4_t f1_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in1)));
            float32x4_t f2_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in2)));
            float32x4_t f2_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in2)));
            float32x4_t f3_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in3)));
            float32x4_t f3_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in3)));
            float32x4_t f4_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in4)));
            float32x4_t f4_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in4)));
            float32x4_t f5_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in5)));
            float32x4_t f5_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in5)));
            float32x4_t f6_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in6)));
            float32x4_t f6_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in6)));
            float32x4_t f7_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in7)));
            float32x4_t f7_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in7)));

            // 3. [STORE] 连续突发写入
            vst1q_f32(dst + i,      f0_lo);
            vst1q_f32(dst + i + 4,  f0_hi);
            vst1q_f32(dst + i + 8,  f1_lo);
            vst1q_f32(dst + i + 12, f1_hi);
            vst1q_f32(dst + i + 16, f2_lo);
            vst1q_f32(dst + i + 20, f2_hi);
            vst1q_f32(dst + i + 24, f3_lo);
            vst1q_f32(dst + i + 28, f3_hi);
            vst1q_f32(dst + i + 32, f4_lo);
            vst1q_f32(dst + i + 36, f4_hi);
            vst1q_f32(dst + i + 40, f5_lo);
            vst1q_f32(dst + i + 44, f5_hi);
            vst1q_f32(dst + i + 48, f6_lo);
            vst1q_f32(dst + i + 52, f6_hi);
            vst1q_f32(dst + i + 56, f7_lo);
            vst1q_f32(dst + i + 60, f7_hi);
        }
    }
    // ============================================================
    // 策略 B: 针对 < 128K 的小数据量 (完全保留之前的逻辑)
    // ============================================================
    else 
    {
        // 保持您刚才满意的代码逻辑 (64个展开)
        for (; i <= total_len - 64; i += 64)
        {
            int16x8_t in0 = vld1q_s16(src + i);
            int16x8_t in1 = vld1q_s16(src + i + 8);
            int16x8_t in2 = vld1q_s16(src + i + 16);
            int16x8_t in3 = vld1q_s16(src + i + 24);
            int16x8_t in4 = vld1q_s16(src + i + 32);
            int16x8_t in5 = vld1q_s16(src + i + 40);
            int16x8_t in6 = vld1q_s16(src + i + 48);
            int16x8_t in7 = vld1q_s16(src + i + 56);

            float32x4_t f0_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in0)));
            float32x4_t f0_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in0)));
            float32x4_t f1_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in1)));
            float32x4_t f1_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in1)));
            float32x4_t f2_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in2)));
            float32x4_t f2_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in2)));
            float32x4_t f3_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in3)));
            float32x4_t f3_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in3)));
            float32x4_t f4_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in4)));
            float32x4_t f4_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in4)));
            float32x4_t f5_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in5)));
            float32x4_t f5_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in5)));
            float32x4_t f6_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in6)));
            float32x4_t f6_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in6)));
            float32x4_t f7_lo = vcvtq_f32_s32(vmovl_s16(vget_low_s16(in7)));
            float32x4_t f7_hi = vcvtq_f32_s32(vmovl_s16(vget_high_s16(in7)));

            vst1q_f32(dst + i,      f0_lo);
            vst1q_f32(dst + i + 4,  f0_hi);
            vst1q_f32(dst + i + 8,  f1_lo);
            vst1q_f32(dst + i + 12, f1_hi);
            vst1q_f32(dst + i + 16, f2_lo);
            vst1q_f32(dst + i + 20, f2_hi);
            vst1q_f32(dst + i + 24, f3_lo);
            vst1q_f32(dst + i + 28, f3_hi);
            vst1q_f32(dst + i + 32, f4_lo);
            vst1q_f32(dst + i + 36, f4_hi);
            vst1q_f32(dst + i + 40, f5_lo);
            vst1q_f32(dst + i + 44, f5_hi);
            vst1q_f32(dst + i + 48, f6_lo);
            vst1q_f32(dst + i + 52, f6_hi);
            vst1q_f32(dst + i + 56, f7_lo);
            vst1q_f32(dst + i + 60, f7_hi);
        }
    }

    // 扫尾
    for (; i < total_len; i++) {
        dst[i] = (float)src[i];
    }

    return birmSuccess;
}
