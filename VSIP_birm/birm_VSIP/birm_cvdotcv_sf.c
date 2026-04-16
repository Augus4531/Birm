#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvdotcv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 注意要按步进大小计算 block 能跨越的实际长度
    vsip_cblock_f *blk_x1 = vsip_cblockbind_f((float *)x1, NULL, nx * sa, VSIP_MEM_NONE);
    vsip_cblock_f *blk_x2 = vsip_cblockbind_f((float *)x2, NULL, nx * sb, VSIP_MEM_NONE);

    // bind 时传入 stride sa 和 sb
    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, sa, nx);
    vsip_cvview_f *vw_x2 = vsip_cvbind_f(blk_x2, 0, sb, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);
    vsip_cblockadmit_f(blk_x2, VSIP_TRUE);

    vsip_cscalar_f res = vsip_cvdot_f(vw_x1, vw_x2);
    y[0] = res.r;
    y[1] = res.i;

    vsip_cvdestroy_f(vw_x1);
    vsip_cvdestroy_f(vw_x2);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x1, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_x2, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x1);
    vsip_cblockdestroy_f(blk_x2);

    return birmSuccess;
}