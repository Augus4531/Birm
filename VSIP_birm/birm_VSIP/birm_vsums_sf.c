#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vsums_sf(const float *x, const int sa, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0)
        return birmParamLengthInvalidError;

    // 按最大跨度绑定内存块
    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx * sa, VSIP_MEM_NONE);
    // 绑定步长为 sa 的视图
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, sa, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);

    // VSIP 专用的平方和算子
    y[0] = vsip_vsumsqval_f(vw_x);

    vsip_vdestroy_f(vw_x);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);

    return birmSuccess;
}