#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vdotv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 绑定时考虑步长最大跨度
    vsip_block_f *blk_x1 = vsip_blockbind_f((float *)x1, nx * sa, VSIP_MEM_NONE);
    vsip_block_f *blk_x2 = vsip_blockbind_f((float *)x2, nx * sb, VSIP_MEM_NONE);

    // 绑定视图，传入指定的步长 sa 和 sb
    vsip_vview_f *vw_x1 = vsip_vbind_f(blk_x1, 0, sa, nx);
    vsip_vview_f *vw_x2 = vsip_vbind_f(blk_x2, 0, sb, nx);

    vsip_blockadmit_f(blk_x1, VSIP_TRUE);
    vsip_blockadmit_f(blk_x2, VSIP_TRUE);

    // 计算点乘
    y[0] = vsip_vdot_f(vw_x1, vw_x2);

    vsip_vdestroy_f(vw_x1);
    vsip_vdestroy_f(vw_x2);
    vsip_scalar_f *p = 0;
    vsip_blockrelease_f(blk_x1, VSIP_TRUE);
    vsip_blockrelease_f(blk_x2, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x1);
    vsip_blockdestroy_f(blk_x2);

    return birmSuccess;
}