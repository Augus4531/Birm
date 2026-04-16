#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vmulv_sf(const float *x1, const int sa1, const float *x2, const int sa2, const int nx, float *y, const int sb)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa1 <= 0 || sa2 <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 根据步长预留足够长度的 Block
    vsip_block_f *blk_x1 = vsip_blockbind_f((float *)x1, nx * sa1, VSIP_MEM_NONE);
    vsip_block_f *blk_x2 = vsip_blockbind_f((float *)x2, nx * sa2, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx * sb, VSIP_MEM_NONE);

    // 绑定不同步长 (sa1, sa2, sb) 的 View
    vsip_vview_f *vw_x1 = vsip_vbind_f(blk_x1, 0, sa1, nx);
    vsip_vview_f *vw_x2 = vsip_vbind_f(blk_x2, 0, sa2, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, sb, nx);

    vsip_blockadmit_f(blk_x1, VSIP_TRUE);
    vsip_blockadmit_f(blk_x2, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    vsip_vmul_f(vw_x1, vw_x2, vw_y);

    vsip_vdestroy_f(vw_x1);
    vsip_vdestroy_f(vw_x2);
    vsip_vdestroy_f(vw_y);
    vsip_scalar_f *p = 0;
    vsip_blockrelease_f(blk_x1, VSIP_TRUE);
    vsip_blockrelease_f(blk_x2, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x1);
    vsip_blockdestroy_f(blk_x2);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}