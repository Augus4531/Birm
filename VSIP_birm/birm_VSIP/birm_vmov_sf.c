#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vmov_sf(const float *x, const int sa, const int nx, float *y, const int sb)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 按最大跨度绑定内存块
    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx * sa, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nx * sb, VSIP_MEM_NONE);

    // 绑定指定步长的视图
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, sa, nx);
    vsip_vview_f *vw_y = vsip_vbind_f(blk_y, 0, sb, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // 矢量带步长拷贝
    vsip_vcopy_f_f(vw_x, vw_y);

    vsip_vdestroy_f(vw_x);
    vsip_vdestroy_f(vw_y);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}