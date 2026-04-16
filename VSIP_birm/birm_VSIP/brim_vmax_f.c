#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vmax_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);

    // 传入一个 vsip_index 地址即可在获取最大值的同时接收索引
    // 这里我们只关心最大值
    vsip_index idx;
    *y = vsip_vmaxval_f(vw_x, &idx);

    vsip_vdestroy_f(vw_x);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);

    return birmSuccess;
}