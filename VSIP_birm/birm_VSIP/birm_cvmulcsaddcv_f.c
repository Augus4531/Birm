#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvmulcsaddcv_f(const float *x1, const float *x2, const float *x3, const int nx, float *y)
{
    if (!x1 || !x2 || !x3 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_cscalar_f scalar;
    scalar.r = x2[0];
    scalar.i = x2[1];

    vsip_cblock_f *blk_x1 = vsip_cblockbind_f((float *)x1, NULL, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_x3 = vsip_cblockbind_f((float *)x3, NULL, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f(y, NULL, nx, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, 1, nx);
    vsip_cvview_f *vw_x3 = vsip_cvbind_f(blk_x3, 0, 1, nx);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);
    vsip_cblockadmit_f(blk_x3, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 第一步：y = scalar * x1
    vsip_csvmul_f(scalar, vw_x1, vw_y);

    // 第二步：y = y + x3
    vsip_cvadd_f(vw_y, vw_x3, vw_y);

    vsip_cvdestroy_f(vw_x1);
    vsip_cvdestroy_f(vw_x3);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x1, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_x3, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x1);
    vsip_cblockdestroy_f(blk_x3);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}