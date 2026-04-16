#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cmtrans_sf(const float *x, const int nn, const int mm, const int sa, const int sb, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nn <= 0 || mm <= 0 || sa <= 0 || sb <= 0 || sb > nn || sa > mm)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nn * mm, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f((float *)y, NULL, sa * sb, VSIP_MEM_NONE);

    // x 绑定为全矩阵视图，y 绑定为转置后的子矩阵视图
    vsip_cmview_f *vw_x_full = vsip_cmbind_f(blk_x, 0, mm, nn, 1, mm);
    vsip_cmview_f *vw_y = vsip_cmbind_f(blk_y, 0, sa, sb, 1, sa);

    // 截取视图
    vsip_cmview_f *vw_x_sub = vsip_cmsubview_f(vw_x_full, 0, 0, sa, sb);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    vsip_cmtrans_f(vw_x_sub, vw_y);

    vsip_cmdestroy_f(vw_x_sub);
    vsip_cmdestroy_f(vw_x_full);
    vsip_cmdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}