#include "birm_arm_float_User.h"
#include <arm_neon.h>

// 预取距离：通常设置为 2 到 4 个 Cache Line (1个 Cache Line 通常 64 字节)
// 这里我们一次循环处理 16 个 float (64字节)，等于一个 Cache Line。
// 预取距离设置为 320 字节左右 (即提前预取未来第 5-6 次循环的数据)
#define PREFETCH_DISTANCE 80 

/**
 * @brief 针对大尺寸数组优化的浮点转定点函数
 * 优化策略:
 * 1. 4路循环展开 (4x Unrolling): 每次循环处理 16 个复数点 (32个 float, 128字节)。
 * 2. 软件预取 (__builtin_prefetch): 提前加载内存，掩盖 DRAM 延迟。
 * 3. 减少循环开销。
 */
int birm_cvtovint16_f(const float *x, const int nx, int *y)
{
    if (!x || !y) { return birmParamNullError; }
    if (nx <= 0) { return birmParamLengthInvalidError; }

    int i = 0;
    
    // 每次主循环处理 16 个复数点 (16对 Re/Im)
    // 输入: 32 个 float (128 Bytes) -> 正好 2 个 Cache Line (假设64B)
    // 输出: 16 个 int (64 Bytes)
    int blkCnt = nx >> 4; // nx / 16

    const float *pSrc = x;
    int16_t *pDstShort = (int16_t *)y;

    while (blkCnt > 0)
    {
        // --- 预取策略 ---
        // __builtin_prefetch(addr, rw, locality)
        // rw: 0=read, 1=write
        // locality: 3=high temporal locality (keep in cache)
        // 提前预取未来的数据块
        __builtin_prefetch(pSrc + PREFETCH_DISTANCE, 0, 3);
        __builtin_prefetch(pSrc + PREFETCH_DISTANCE + 16, 0, 3);

        // --- Block 0 (处理前 4 个复数) ---
        float32x4_t f0 = vld1q_f32(pSrc);
        float32x4_t f1 = vld1q_f32(pSrc + 4);
        int32x4_t i32_0 = vcvtq_s32_f32(f0);
        int32x4_t i32_1 = vcvtq_s32_f32(f1);
        int16x4_t n0 = vqmovn_s32(i32_0);
        int16x8_t res0 = vqmovn_high_s32(n0, i32_1);
        vst1q_s16(pDstShort, res0);

        // --- Block 1 (处理接下来的 4 个复数) ---
        float32x4_t f2 = vld1q_f32(pSrc + 8);
        float32x4_t f3 = vld1q_f32(pSrc + 12);
        int32x4_t i32_2 = vcvtq_s32_f32(f2);
        int32x4_t i32_3 = vcvtq_s32_f32(f3);
        int16x4_t n1 = vqmovn_s32(i32_2);
        int16x8_t res1 = vqmovn_high_s32(n1, i32_3);
        vst1q_s16(pDstShort + 8, res1);

        // --- Block 2 ---
        float32x4_t f4 = vld1q_f32(pSrc + 16);
        float32x4_t f5 = vld1q_f32(pSrc + 20);
        int32x4_t i32_4 = vcvtq_s32_f32(f4);
        int32x4_t i32_5 = vcvtq_s32_f32(f5);
        int16x4_t n2 = vqmovn_s32(i32_4);
        int16x8_t res2 = vqmovn_high_s32(n2, i32_5);
        vst1q_s16(pDstShort + 16, res2);

        // --- Block 3 ---
        float32x4_t f6 = vld1q_f32(pSrc + 24);
        float32x4_t f7 = vld1q_f32(pSrc + 28);
        int32x4_t i32_6 = vcvtq_s32_f32(f6);
        int32x4_t i32_7 = vcvtq_s32_f32(f7);
        int16x4_t n3 = vqmovn_s32(i32_6);
        int16x8_t res3 = vqmovn_high_s32(n3, i32_7);
        vst1q_s16(pDstShort + 24, res3);

        // 更新指针
        pSrc += 32;       // 16 个复数 = 32 个 float
        pDstShort += 32;  // 16 个复数 = 32 个 short
        blkCnt--;
    }

    // 处理剩余尾部数据 (同之前版本)
    i = (nx >> 4) << 4; 
    short *yTemp = (short *)y + (i * 2);

    for (; i < nx; i++)
    {
        float re_f = x[2*i];
        float im_f = x[2*i+1];
        short re, im;

        if(re_f < -32768.0f) re = -32768;
        else if(re_f > 32767.0f) re = 32767;
        else re = (short)re_f;

        if(im_f < -32768.0f) im = -32768;
        else if(im_f > 32767.0f) im = 32767;
        else im = (short)im_f;

        yTemp[0] = re;
        yTemp[1] = im;
        yTemp += 2;
    }

    return birmSuccess;
}

