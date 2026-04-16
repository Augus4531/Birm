#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_cvmuls_if(float *x1, const float x2, const int nx)
{
    if (!x1)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x1 = vsip_cblockbind_f(x1, NULL, nx, VSIP_MEM_NONE);
    vsip_cvview_f *vw_x1 = vsip_cvbind_f(blk_x1, 0, 1, nx);

    vsip_cblockadmit_f(blk_x1, VSIP_TRUE);

    // 实数标量乘以复数矢量 (Real Scalar Complex Vector Multiply)
    // 就地操作，将结果直接存回 vw_x1
    vsip_rscvmul_f(x2, vw_x1, vw_x1);

    vsip_cvdestroy_f(vw_x1);

    // 【修改点】：声明两个临时指针，用来满足你当前 VSIP 版本的 release 函数签名
    vsip_scalar_f *rp = NULL;
    vsip_scalar_f *ip = NULL;
    // 传入 &rp 和 &ip 补齐 4 个参数
    vsip_cblockrelease_f(blk_x1, VSIP_TRUE, &rp, &ip);

    vsip_cblockdestroy_f(blk_x1);

    return birmSuccess;
}