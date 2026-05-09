#include "birm_arm_float_User.h"
#include <arm_neon.h>

// 辅助函数：stnp (Store Pair Non-Temporal)
// 保持这个函数，它对写性能至关重要
static inline void vst1q_stream_f32_x2(float *addr, float32x4_t v0, float32x4_t v1) {
    asm volatile(
        "stnp %q[val0], %q[val1], [%[ptr]]"
        : 
        : [ptr] "r" (addr), [val0] "w" (v0), [val1] "w" (v1)
        : "memory"
    );
}

// 预取距离：320 Bytes (约 5 个 Cache Line)
#define PREFETCH_DIST 320

int birm_int2float_f(const int* x, const int nx, float* y)
{
    if (!x || !y) return birmParamNullError;
    if (nx <= 0) return birmParamLengthInvalidError;

    int i = 0;
    // 每次处理 32个点 = 128 Bytes
    // 128 Bytes 正好是 2 个 Cache Line，也适合 DDR Burst
    int blkCnt = nx >> 5; 

    const int *pSrc = x;
    float *pDst = y;

    while (blkCnt > 0)
    {
        // --- 1. 极简预取 (消除 35% 的 prfm 开销) ---
        // 关键修改：每次循环仅发射一条预取指令。
        // 不需要预取 pSrc + 16，因为读取了 Cache Line 头，硬件通常会自动拉取整行。
        // 这将把 prfm 的 overhead 从 35% 降低到 5% 以下。
        __builtin_prefetch(pSrc + PREFETCH_DIST, 0, 3);

        // --- 2. 流水线核心 (保持交错) ---
        // 我们保持 Load-Convert-Store 的交错，但逻辑写得更紧凑
        
        // Group 1: Load
        int32x4_t i0 = vld1q_s32(pSrc);
        int32x4_t i1 = vld1q_s32(pSrc + 4);
        
        // Group 1: Convert (Start early)
        float32x4_t f0 = vcvtq_f32_s32(i0);
        float32x4_t f1 = vcvtq_f32_s32(i1);
        
        // Group 2: Load (Insert between Calc & Store to hide latency)
        int32x4_t i2 = vld1q_s32(pSrc + 8);
        int32x4_t i3 = vld1q_s32(pSrc + 12);
        
        // Group 1: Store (Stream output)
        vst1q_stream_f32_x2(pDst, f0, f1);

        // Group 2: Convert
        float32x4_t f2 = vcvtq_f32_s32(i2);
        float32x4_t f3 = vcvtq_f32_s32(i3);

        // Group 3: Load
        int32x4_t i4 = vld1q_s32(pSrc + 16);
        int32x4_t i5 = vld1q_s32(pSrc + 20);

        // Group 2: Store
        vst1q_stream_f32_x2(pDst + 8, f2, f3);

        // Group 3: Convert
        float32x4_t f4 = vcvtq_f32_s32(i4);
        float32x4_t f5 = vcvtq_f32_s32(i5);

        // Group 4: Load
        int32x4_t i6 = vld1q_s32(pSrc + 24);
        int32x4_t i7 = vld1q_s32(pSrc + 28);

        // Group 3: Store
        vst1q_stream_f32_x2(pDst + 16, f4, f5);

        // Group 4: Convert
        float32x4_t f6 = vcvtq_f32_s32(i6);
        float32x4_t f7 = vcvtq_f32_s32(i7);

        // Group 4: Store
        vst1q_stream_f32_x2(pDst + 24, f6, f7);

        // 更新指针
        pSrc += 32;
        pDst += 32;
        blkCnt--;
    }

    // --- 内存屏障 (必须保留) ---
    // 确保 stnp 的写入顺序对后续操作可见
    asm volatile("dmb ishst" ::: "memory");

    // --- 尾部处理 ---
    i = (nx >> 5) << 5;
    for (; i < nx; i++)
    {
        y[i] = (float)x[i];
    }

    return birmSuccess;
}