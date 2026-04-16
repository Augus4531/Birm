#include "birm_arm_float_User.h"
#include <vsip.h>

/**
 * @brief 复数矢量iFFT变换 (VSIP实现)
 * 注意：外层程序在创建 VSIP FFT 句柄时，需将方向设为 VSIP_FFT_INV，scale设为 1.0/fftN
 */
int birm_ifft_f(const float *x, const int fftN, void *ffthandle, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (fftN <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 将句柄强转为 VSIP 的 FFT 对象
    vsip_fft_f *vsip_plan = (vsip_fft_f *)ffthandle;

    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, fftN, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f((float *)y, NULL, fftN, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, 1, fftN);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, fftN);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 执行 iFFT 操作，如果句柄创建时包含了 1/fftN 的 scale，这里会自动完成缩放
    vsip_ccfftop_f(vsip_plan, vw_x, vw_y);

    vsip_cvdestroy_f(vw_x);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}