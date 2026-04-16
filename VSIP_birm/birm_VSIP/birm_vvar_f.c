#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vvar_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;
    if (nx == 1)
    {
        *y = 0.0f;
        return birmSuccess;
    }

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);

    // 代数和与平方和
    float xsum = vsip_vsumval_f(vw_x);
    float x2 = vsip_vsumsqval_f(vw_x);

    vsip_vdestroy_f(vw_x);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);

    // 计算均值
    float xmean = xsum / nx;
    float x2mean = x2 / nx;

    // 计算方差
    float out = x2mean - (xmean * xmean);
    if (out < 0.0f)
        out = 0.0f;

    *y = out;
    return birmSuccess;
}