#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vclip_f(const float *x1, const float x2, const float x3, const int nx, float *y)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;
    if (x2 < x3)
        return birmParamOtherError; // x2 为最大值, x3 为最小值

    vsip_block_f *blk_x1 = vsip_blockbind_f((float *)x1, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx, VSIP_MEM_NONE);

    vsip_vview_f *vw_x1 = vsip_vbind_f(blk_x1, 0, 1, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, 1, nx);

    vsip_blockadmit_f(blk_x1, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // vsip_vclip_f (输入, 下限阈值, 上限阈值, 低于下限替换值, 高于上限替换值, 输出)
    vsip_vclip_f(vw_x1, x3, x2, x3, x2, vw_y);

    vsip_vdestroy_f(vw_x1);
    vsip_vdestroy_f(vw_y);
    vsip_scalar_f *p = 0;
    vsip_blockrelease_f(blk_x1, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x1);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}