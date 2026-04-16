#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvdotjv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x1 = vsip_cblockbind_f((float *)x1, NULL, nx * sa, VSIP_MEM_NONE);
    vsip_cblock_f *blk_x2 = vsip_cblockbind_f((float *)x2, NULL, nx * sb, VSIP_MEM_NONE);

    // 带步长绑定视图
    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, sa, nx);
    vsip_cvview_f *vw_x2 = vsip_cvbind_f(blk_x2, 0, sb, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);
    vsip_cblockadmit_f(blk_x2, VSIP_TRUE);

    // vsip_cvjdot_f 会对第一个参数取共轭
    vsip_cscalar_f res = vsip_cvjdot_f(vw_x2, vw_x1);
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