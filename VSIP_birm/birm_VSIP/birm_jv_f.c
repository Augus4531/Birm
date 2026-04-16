#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_jv_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f(y, NULL, nx, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, 1, nx);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, nx);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 计算复数共轭
    vsip_cvconj_f(vw_x, vw_y);

    vsip_cvdestroy_f(vw_x);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}