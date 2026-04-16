#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvmov_sf(const float *x, const int sa, const int nx, float *y, const int sb)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nx * sa, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f(y, NULL, nx * sb, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, sa, nx);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, sb, nx);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 复数矢量拷贝
    vsip_cvcopy_f_f(vw_x, vw_y);

    vsip_cvdestroy_f(vw_x);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}