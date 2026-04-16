#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vsubs_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    vsip_block_f *blk_x1 = vsip_blockbind_f((float *)x1, nx * sa, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx * sb, VSIP_MEM_NONE);

    vsip_vview_f *vw_x1 = vsip_vbind_f(blk_x1, 0, sa, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, sb, nx);

    vsip_blockadmit_f(blk_x1, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // 带步长操作: y = (-x2) + x1
    vsip_svadd_f(-x2, vw_x1, vw_y);

    vsip_vdestroy_f(vw_x1);
    vsip_vdestroy_f(vw_y);
    vsip_scalar_f *p = 0;
    vsip_blockrelease_f(blk_x1, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x1);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}