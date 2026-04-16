#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vramp_f(const float x1, const float x2, const int nx, float *y)
{
    if (!y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    vsip_block_f *blk_y = vsip_blockbind_f(y, nx, VSIP_MEM_NONE);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, 1, nx);

    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // VSIP 的 vramp_f 直接生成 y[i] = x1 + i * x2
    vsip_vramp_f(x1, x2, vw_y);

    vsip_vdestroy_f(vw_y);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}