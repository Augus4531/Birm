#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <stddef.h>
#include <math.h>

int birm_vabs_f(const float *x, const int nx, float *y)
{
    // 参数检查
    if (x == NULL || y == NULL)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    // 预取距离设置：
    // 我们每次循环处理 16 个 float (64 字节)。
    // 飞腾/ARM64 的 Cache Line 通常为 64 字节。
    // 预取距离通常设置为 256 字节到 512 字节（即 64 到 128 个 float）。
    // 这里设置为 64 个 float (256 字节)，即提前 4 轮循环预取。
    const int prefetch_offset = 64;

    // --- 主循环：每次处理 16 个浮点数 ---
    for (; i <= nx - 16; i += 16)
    {
        // 优化2: 软件预取 (Software Prefetch)
        // 参数说明:
        // addr: 预取地址
        // rw: 0=读 (Read), 1=写 (Write)
        // locality: 3=高时间局部性 (保留在各级 Cache), 0=低局部性 (用完即丢)
        __builtin_prefetch(x + i + prefetch_offset, 0, 3);

        // 如果是写入操作（Write Allocate），也可以尝试预取 y，但在流式写入中通常效果不如读预取明显
        // __builtin_prefetch(y + i + prefetch_offset, 1, 0);

        float32x4_t vec0 = vld1q_f32(x + i);
        float32x4_t vec1 = vld1q_f32(x + i + 4);
        float32x4_t vec2 = vld1q_f32(x + i + 8);
        float32x4_t vec3 = vld1q_f32(x + i + 12);

        vec0 = vabsq_f32(vec0);
        vec1 = vabsq_f32(vec1);
        vec2 = vabsq_f32(vec2);
        vec3 = vabsq_f32(vec3);

        vst1q_f32(y + i, vec0);
        vst1q_f32(y + i + 4, vec1);
        vst1q_f32(y + i + 8, vec2);
        vst1q_f32(y + i + 12, vec3);
    }

    // --- 处理剩余 8 个浮点数 ---
    if (i <= nx - 8)
    {
        float32x4_t vec0 = vld1q_f32(x + i);
        float32x4_t vec1 = vld1q_f32(x + i + 4);

        vec0 = vabsq_f32(vec0);
        vec1 = vabsq_f32(vec1);

        vst1q_f32(y + i, vec0);
        vst1q_f32(y + i + 4, vec1);
        i += 8;
    }

    // --- 处理剩余 4 个浮点数 ---
    if (i <= nx - 4)
    {
        float32x4_t vec = vld1q_f32(x + i);
        vec = vabsq_f32(vec);
        vst1q_f32(y + i, vec);
        i += 4;
    }

    // --- 处理剩余 1-3 个浮点数 ---
    // 优化3: 移除 Union Hack，直接使用库函数
    // 编译器会自动将其编译为 FABS 标量指令，避免寄存器跨域搬运
    for (; i < nx; i++)
    {
        y[i] = fabsf(x[i]);
    }

    return birmSuccess;
}
