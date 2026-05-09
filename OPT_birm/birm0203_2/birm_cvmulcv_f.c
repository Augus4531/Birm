#include "birm_arm_float_User.h"
#include <arm_neon.h>

/**
 * @brief 复数矢量点乘 (Element-wise Complex Multiplication) 优化版
 * 公式:
 * Re(y) = Re(x1)*Re(x2) - Im(x1)*Im(x2)
 * Im(y) = Re(x1)*Im(x2) + Im(x1)*Re(x2)
 *
 * 优化策略:
 * 1. 使用 NEON SIMD 并行计算。
 * 2. 利用 vmulq (乘法), vmlsq (乘减), vmlaq (乘加) 指令。
 * 3. 使用 ld2/st2 指令自动分离/交织实部和虚部，避免手动 shuffle。
 */
int birm_cvmulcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !y || !x2)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    // 每次循环处理 4 个复数 (即 8 个 float)
    int loop_end = nx & ~3; // nx - (nx % 4)

    for (; i < loop_end; i += 4)
    {
        // 1. 加载并解交织 (De-interleave Load)
        // vld2q_f32 会把内存中的 [r0, i0, r1, i1...] 自动拆分加载到两个寄存器
        // x1_vec.val[0] 存放实部 (re1)，x1_vec.val[1] 存放虚部 (im1)
        float32x4x2_t vx1 = vld2q_f32(x1 + i * 2);
        float32x4x2_t vx2 = vld2q_f32(x2 + i * 2);

        float32x4_t re1 = vx1.val[0];
        float32x4_t im1 = vx1.val[1];
        float32x4_t re2 = vx2.val[0];
        float32x4_t im2 = vx2.val[1];

        // 2. 计算实部: Re(y) = Re(x1)*Re(x2) - Im(x1)*Im(x2)
        // vmulq_f32: re1 * re2
        // vmlsq_f32: (re1 * re2) - (im1 * im2) -> 乘减指令
        float32x4_t re_res = vmulq_f32(re1, re2);
        re_res = vmlsq_f32(re_res, im1, im2);

        // 3. 计算虚部: Im(y) = Re(x1)*Im(x2) + Im(x1)*Re(x2)
        // vmulq_f32: re1 * im2
        // vmlaq_f32: (re1 * im2) + (im1 * re2) -> 乘加指令
        float32x4_t im_res = vmulq_f32(re1, im2);
        im_res = vmlaq_f32(im_res, im1, re2);

        // 4. 交织并存储 (Interleave Store)
        // vst2q_f32 会把两个寄存器的内容交织写回内存 [r0, i0, r1, i1...]
        float32x4x2_t vy;
        vy.val[0] = re_res;
        vy.val[1] = im_res;
        vst2q_f32(y + i * 2, vy);
    }

    // 处理剩余数据
    for (; i < nx; i++)
    {
        float r1 = x1[i * 2];
        float i1 = x1[i * 2 + 1];
        float r2 = x2[i * 2];
        float i2 = x2[i * 2 + 1];

        y[i * 2] = r1 * r2 - i1 * i2;
        y[i * 2 + 1] = r1 * i2 + i1 * r2;
    }

    return birmSuccess;
}
