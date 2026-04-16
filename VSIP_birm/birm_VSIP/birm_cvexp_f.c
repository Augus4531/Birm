#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvexp_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // x 是实数 block，y 是复数 block
    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f(y, NULL, nx, VSIP_MEM_NONE);

    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, 1, nx);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 计算欧拉公式: y = cos(x) + j*sin(x)
    vsip_veuler_f(vw_x, vw_y);

    vsip_vdestroy_f(vw_x);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_blockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}