#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vreversal_f(const float *x, const int nx, float *y)
{
    if (!y || !x)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx, VSIP_MEM_NONE);

    // 核心：x 的视图起点设在末尾 (nx - 1)，步长设为 -1
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, nx - 1, -1, nx);
    // y 的视图起点为 0，步长为 1
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // 拷贝时，VSIP 会自动处理步长差异，从而实现数组翻转
    vsip_vcopy_f_f(vw_x, vw_y);

    vsip_vdestroy_f(vw_x);
    vsip_vdestroy_f(vw_y);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}