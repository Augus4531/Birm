#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vsums_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);

    // 矢量平方和 (Sum of squares value)
    y[0] = vsip_vsumsqval_f(vw_x);

    vsip_vdestroy_f(vw_x);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);

    return birmSuccess;
}