#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vsin_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx, VSIP_MEM_NONE);

    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, 1, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // 计算矢量正弦
    vsip_vsin_f(vw_x, vw_y);

    vsip_vdestroy_f(vw_x);
    vsip_vdestroy_f(vw_y);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}