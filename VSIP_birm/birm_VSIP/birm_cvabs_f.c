#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvabs_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // x 为复数 block，y 为实数 block
    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, 1, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, 1, nx);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // 复数求模赋值给实数矢量
    vsip_cvmag_f(vw_x, vw_y);

    vsip_cvdestroy_f(vw_x);
    vsip_vdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_cblockdestroy_f(blk_x);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}