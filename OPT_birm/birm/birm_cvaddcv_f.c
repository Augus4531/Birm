#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "birm_arm_float_User.h"
// 20251218 best

// 512K以后的数据有较大优化，小数据量效率恶化，所以分界线改为256K
//  阈值设定：
//  对于 S5000C 这种架构，通常 L1 Cache (32KB-64KB) 能装下的数据不需要预取。
//  nx = 2048 -> 4096 floats -> 16KB 数据。

#define PREFETCH_THRESHOLD 262144

int birm_cvaddcv_f(const float *__restrict x1,
                   const float *__restrict x2,
                   const int nx,
                   float *__restrict y)
{
    if (!x1 || !y || !x2)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    const int total_floats = nx * 2;
    int i = 0;

    // ==========================================
    // 场景 A: 小数据量 (nx < 262144)
    // 策略：简单、紧凑、无预取、单路 NEON
    // 目标：最小化启动开销和指令数
    // ==========================================
    if (nx <= 262144)
    {
        const int neon_loop = total_floats & ~7; // 8 floats align

        // 2路展开 (2x128bit)，轻量级
        for (; i < neon_loop; i += 8)
        {
            float32x4_t va0 = vld1q_f32(x1 + i);
            float32x4_t vb0 = vld1q_f32(x2 + i);
            float32x4_t va1 = vld1q_f32(x1 + i + 4);
            float32x4_t vb1 = vld1q_f32(x2 + i + 4);

            float32x4_t res0 = vaddq_f32(va0, vb0);
            float32x4_t res1 = vaddq_f32(va1, vb1);

            vst1q_f32(y + i, res0);
            vst1q_f32(y + i + 4, res1);
        }

        // 处理尾部
        for (; i < total_floats; i++)
        {
            y[i] = x1[i] + x2[i];
        }
        return birmSuccess;
    }

    // ==========================================
    // 场景 B: 大数据量 (nx >= 262144)
    // 策略：4路展开 + 预取
    // 目标：隐藏内存延迟，最大化吞吐
    // ==========================================

    const int unroll_factor = 16; // 4路展开
    const int main_loop_limit = total_floats & ~(unroll_factor - 1);

    for (; i < main_loop_limit; i += unroll_factor)
    {
        // 仅在大数据量时开启预取
        __builtin_prefetch(&x1[i + 64], 0, 0);
        __builtin_prefetch(&x2[i + 64], 0, 0);

        float32x4_t va0 = vld1q_f32(x1 + i);
        float32x4_t vb0 = vld1q_f32(x2 + i);
        float32x4_t va1 = vld1q_f32(x1 + i + 4);
        float32x4_t vb1 = vld1q_f32(x2 + i + 4);
        float32x4_t va2 = vld1q_f32(x1 + i + 8);
        float32x4_t vb2 = vld1q_f32(x2 + i + 8);
        float32x4_t va3 = vld1q_f32(x1 + i + 12);
        float32x4_t vb3 = vld1q_f32(x2 + i + 12);

        float32x4_t res0 = vaddq_f32(va0, vb0);
        float32x4_t res1 = vaddq_f32(va1, vb1);
        float32x4_t res2 = vaddq_f32(va2, vb2);
        float32x4_t res3 = vaddq_f32(va3, vb3);

        vst1q_f32(y + i, res0);
        vst1q_f32(y + i + 4, res1);
        vst1q_f32(y + i + 8, res2);
        vst1q_f32(y + i + 12, res3);
    }

    // 处理剩余向量
    const int neon_loop_limit = total_floats & ~3;
    for (; i < neon_loop_limit; i += 4)
    {
        float32x4_t vec1 = vld1q_f32(x1 + i);
        float32x4_t vec2 = vld1q_f32(x2 + i);
        float32x4_t result = vaddq_f32(vec1, vec2);
        vst1q_f32(y + i, result);
    }

    // 处理剩余标量
    for (; i < total_floats; i++)
    {
        y[i] = x1[i] + x2[i];
    }

    return birmSuccess;
}
