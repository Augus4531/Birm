#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvaddcs_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !y || !x2)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // x2 是一个复数标量，提取出来
    vsip_cscalar_f scalar;
    scalar.r = x2[0];
    scalar.i = x2[1];

    vsip_cblock_f *blk_x1 = vsip_cblockbind_f((float *)x1, NULL, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f((float *)y, NULL, nx, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, 1, nx);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 复数标量 + 复数矢量 = 输出复数矢量
    vsip_csvadd_f(scalar, vw_x1, vw_y);

    vsip_cvdestroy_f(vw_x1);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x1, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x1);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}