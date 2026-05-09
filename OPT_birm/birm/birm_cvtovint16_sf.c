#include "birm_arm_float_User.h"
#include <arm_neon.h>

/**
 * @brief 浮点复数打包转化为2个16bit构成的32bit整形数 (带步长优化版)
 * 优化策略:
 * 1. 快速路径检测: 当 sa=1, sb=1 时，回退到纯连续内存的高速 NEON 模式。
 * 2. 向量化计算: 即使数据不连续，依然一次加载4个复数点到寄存器进行并行饱和运算。
 * 3. 减少访存次数: 输出时直接以 int (32bit) 为单位写入，代替两次 short (16bit) 写入。
 * 4. 软件预取: 针对大步长导致的 Cache Miss 进行预取。
 */
int birm_cvtovint16_sf(const float *x, const int sa, const int nx, int *y, const int sb)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    if (sa == 1 && sb == 1)
    {
        int i = 0;
        int blkCnt = nx >> 2; // nx / 4
        const float *pSrc = x;
        int16_t *pDstShort = (int16_t *)y;

        while (blkCnt > 0)
        {
            float32x4_t f0 = vld1q_f32(pSrc);
            float32x4_t f1 = vld1q_f32(pSrc + 4);
            int32x4_t i32_0 = vcvtq_s32_f32(f0);
            int32x4_t i32_1 = vcvtq_s32_f32(f1);
            int16x4_t n0 = vqmovn_s32(i32_0);
            int16x8_t res = vqmovn_high_s32(n0, i32_1);
            vst1q_s16(pDstShort, res);
            pSrc += 8;
            pDstShort += 8;
            blkCnt--;
        }
        // 处理剩余部分
        for (i = (nx >> 2) << 2; i < nx; i++)
        {
            float re_f = x[2 * i];
            float im_f = x[2 * i + 1];
            short re = (re_f < -32768.0f) ? -32768 : ((re_f > 32767.0f) ? 32767 : (short)re_f);
            short im = (im_f < -32768.0f) ? -32768 : ((im_f > 32767.0f) ? 32767 : (short)im_f);
            ((short *)y)[2 * i] = re;
            ((short *)y)[2 * i + 1] = im;
        }
        return birmSuccess;
    }

    // --- 策略2: Strided Path (带步长处理) ---
    // 无法直接 vld1q 连续加载，需要手动 Gather/Scatter
    int i;
    int blkCnt = nx >> 2; // 每次处理4个复数点

    int in_step = 2 * sa;

    const float *pSrc = x;
    int *pDst = y;

    while (blkCnt > 0)
    {
        // [预取] 如果步长很大，提前预取下一次迭代的数据
        __builtin_prefetch(pSrc + in_step * 8, 0, 0);

        // --- Step A: Gather (收集数据) ---
        // 我们需要凑齐4个复数 (8个float) 放入 NEON 寄存器
        // 分别加载 4 个不连续的 (Re, Im) 对
        // vld1_f32 加载 2 个 float (64-bit)，正好是一个复数对
        float32x2_t pair0 = vld1_f32(pSrc);
        float32x2_t pair1 = vld1_f32(pSrc + in_step);
        float32x2_t pair2 = vld1_f32(pSrc + in_step * 2);
        float32x2_t pair3 = vld1_f32(pSrc + in_step * 3);

        // 组合成两个 128-bit 向量
        // f_vec0 = [Re0, Im0, Re1, Im1]
        // f_vec1 = [Re2, Im2, Re3, Im3]
        float32x4_t f_vec0 = vcombine_f32(pair0, pair1);
        float32x4_t f_vec1 = vcombine_f32(pair2, pair3);

        // 这里的计算与无步长版本完全一致，极快
        int32x4_t i32_0 = vcvtq_s32_f32(f_vec0);
        int32x4_t i32_1 = vcvtq_s32_f32(f_vec1);

        // 饱和窄化：32bit -> 16bit
        int16x4_t n0 = vqmovn_s32(i32_0);
        // res 包含: [Re0, Im0, Re1, Im1, Re2, Im2, Re3, Im3] (全16位)
        int16x8_t res = vqmovn_high_s32(n0, i32_1);

        // 我们需要把 res 里的数据拆开写到 y, y+sb, y+2sb...
        int32x4_t res_packaged = vreinterpretq_s32_s16(res);

        // 直接写 int 类型，避免两次 short 写操作，利用 CPU 写合并
        pDst[0] = vgetq_lane_s32(res_packaged, 0);
        pDst[sb] = vgetq_lane_s32(res_packaged, 1);
        pDst[sb * 2] = vgetq_lane_s32(res_packaged, 2);
        pDst[sb * 3] = vgetq_lane_s32(res_packaged, 3);

        // 更新指针
        pSrc += in_step * 4;
        pDst += sb * 4;
        blkCnt--;
    }

    // --- 处理尾部 (Tail) ---
    // 剩下的不足4个点，退化为标量处理
    // 重新计算当前的索引 i
    i = (nx >> 2) << 2;
    // 注意：这里的 pDst 和 pSrc 已经在 while 循环中更新过了，

    for (; i < nx; i++)
    {
        float val_re = x[2 * i * sa];
        float val_im = x[2 * i * sa + 1];

        short re, im;

        if (val_re < -32768.0f)
            re = -32768;
        else if (val_re > 32767.0f)
            re = 32767;
        else
            re = (short)val_re;

        if (val_im < -32768.0f)
            im = -32768;
        else if (val_im > 32767.0f)
            im = 32767;
        else
            im = (short)val_im;

        // 利用 int 指针写入一对 short
        // 假设小端序 (ARM 默认): Low=Re, High=Im
        int32_t packed = (uint16_t)re | ((uint32_t)(uint16_t)im << 16);
        y[i * sb] = packed;
    }

    return birmSuccess;
}
