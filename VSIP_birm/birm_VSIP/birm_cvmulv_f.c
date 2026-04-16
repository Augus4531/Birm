#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvmulv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // x1, y 是复数 block；x2 是实数 block
    vsip_cblock_f *blk_x1 = vsip_cblockbind_f((float *)x1, NULL, nx, VSIP_MEM_NONE);
    vsip_block_f *blk_x2 = vsip_blockbind_f((float *)x2, nx, VSIP_MEM_NONE);
    vsip_cblock_f *blk_y = vsip_cblockbind_f((float *)y, NULL, nx, VSIP_MEM_NONE);

    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, 1, nx);
    vsip_vview_f *vw_x2 = vsip_vbind_f(blk_x2, 0, 1, nx);
    vsip_cvview_f *vw_y = vsip_cvbind_f(blk_y, 0, 1, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);
    vsip_blockadmit_f(blk_x2, VSIP_TRUE);
    vsip_cblockadmit_f(blk_y, VSIP_TRUE);

    // 执行实数向量乘以复数向量：y = x2 * x1
    vsip_rcvmul_f(vw_x2, vw_x1, vw_y);

    vsip_cvdestroy_f(vw_x1);
    vsip_vdestroy_f(vw_x2);
    vsip_cvdestroy_f(vw_y);
    vsip_scalar_f *rp = 0, *ip = 0;
    vsip_cblockrelease_f(blk_x1, VSIP_TRUE, &rp, &ip);
    vsip_blockrelease_f(blk_x2, VSIP_TRUE);
    vsip_cblockrelease_f(blk_y, VSIP_TRUE, &rp, &ip);
    vsip_cblockdestroy_f(blk_x1);
    vsip_blockdestroy_f(blk_x2);
    vsip_cblockdestroy_f(blk_y);

    return birmSuccess;
}