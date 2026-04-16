#include "birm_arm_float_User.h"
#include <vsip.h>

/**
 * @brief 复数矢量FFT变换 (VSIP实现)
 * 注意：外层测试程序需要用 vsip_ccfftop_create_f() 创建句柄传入 ffthandle
 */
int birm_fft_f(const float *x, const int fftN, void *ffthandle, float *y)
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

    // 绑定数据到 VSIP 的复数内存块和视图
    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, fftN, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f((float *)y, NULL, fftN, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, 1, fftN);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, fftN);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 执行 FFT（假设外部初始化 vsip_plan 时方向设为了 VSIP_FFT_FWD）
    vsip_ccfftop_f(vsip_plan, vw_x, vw_y);

    // 释放和解绑
    vsip_cvdestroy_f(vw_x);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}