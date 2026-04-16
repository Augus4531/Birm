#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vaddv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_block_f *blk_x1 = vsip_blockbind_f((float *)x1, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_x2 = vsip_blockbind_f((float *)x2, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx, VSIP_MEM_NONE);

    vsip_vview_f *vw_x1 = vsip_vbind_f(blk_x1, 0, 1, nx);
    vsip_vview_f *vw_x2 = vsip_vbind_f(blk_x2, 0, 1, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, 1, nx);

    vsip_blockadmit_f(blk_x1, VSIP_TRUE);
    vsip_blockadmit_f(blk_x2, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    vsip_vadd_f(vw_x1, vw_x2, vw_y);

    vsip_vdestroy_f(vw_x1);
    vsip_vdestroy_f(vw_x2);
    vsip_vdestroy_f(vw_y);
    vsip_scalar_f *p = 0;
    vsip_blockrelease_f(blk_x1, VSIP_TRUE);
    vsip_blockrelease_f(blk_x2, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x1);
    vsip_blockdestroy_f(blk_x2);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}