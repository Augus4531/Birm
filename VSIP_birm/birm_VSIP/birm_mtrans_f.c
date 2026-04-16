#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_mtrans_f(const float *x, const int nn, const int mm, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nn <= 0 || mm <= 0)
        return birmParamLengthInvalidError;

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nn * mm, VSIP_MEM_NONE);
    vsip_block_f *blk_y = vsip_blockbind_f(y, nn * mm, VSIP_MEM_NONE);

    vsip_mview_f *vw_x = vsip_mbind_f(blk_x, 0, mm, nn, 1, mm);
    vsip_mview_f *vw_y = vsip_mbind_f(blk_y, 0, nn, mm, 1, nn);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);
    vsip_blockadmit_f(blk_y, VSIP_TRUE);

    // 矩阵转置
    vsip_mtrans_f(vw_x, vw_y);

    vsip_mdestroy_f(vw_x);
    vsip_mdestroy_f(vw_y);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockrelease_f(blk_y, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);
    vsip_blockdestroy_f(blk_y);

    return birmSuccess;
}