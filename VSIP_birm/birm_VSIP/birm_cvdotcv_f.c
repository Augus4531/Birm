#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvdotcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x1 = vsip_cblockbind_f((float *)x1, NULL, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_x2 = vsip_cblockbind_f((float *)x2, NULL, nx, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, 1, nx);
    vsip_cvview_f *vw_x2 = vsip_cvbind_f(blk_x2, 0, 1, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);
    vsip_cblockadmit_f(blk_x2, VSIP_TRUE);

    // VSIP 中 cvdot 是无共轭的点乘
    vsip_cscalar_f res = vsip_cvdot_f(vw_x1, vw_x2);
    y[0] = res.r;
    y[1] = res.i;

    vsip_cvdestroy_f(vw_x1);
    vsip_cvdestroy_f(vw_x2);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x1, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_x2, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x1);
    vsip_cblockdestroy_f(blk_x2);

    return birmSuccess;
}