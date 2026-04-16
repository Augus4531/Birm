#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cmtrans_f(const float *x, const int nn, const int mm, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nn <= 0 || mm <= 0)
        return birmParamLengthInvalidError;

    // 假定 VSIP 库支持交织复数（Interleaved Complex）绑定
    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nn * mm, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f((float *)y, NULL, nn * mm, VSIP_MEM_NONE);

    vsip_cmview_f *vw_x = vsip_cmbind_f(blk_x, 0, mm, nn, 1, mm);
    vsip_cmview_f *vw_y = vsip_cmbind_f(blk_y, 0, nn, mm, 1, nn);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    vsip_cmtrans_f(vw_x, vw_y);

    vsip_cmdestroy_f(vw_x);
    vsip_cmdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}