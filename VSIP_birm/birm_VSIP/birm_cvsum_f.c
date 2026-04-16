#include "birm_arm_float_User.h"
#include <vsip.h>

/**
 * @brief 浮点单精度复数矢量求和
 */
int birm_cvsum_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nx, VSIP_MEM_NONE);
    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, 1, nx);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);

    // 【核心修改】：因为该版本 VSIP 缺少 vsip_cvsumval_f
    // 我们先求复数均值，然后再乘以数据长度 nx，达到等效求和的目的
    vsip_cscalar_f mean_val = vsip_cvmeanval_f(vw_x);

    // 将均值乘以点数，得到总和，分别赋值给输出的实部和虚部
    y[0] = mean_val.r * nx;
    y[1] = mean_val.i * nx;

    vsip_cvdestroy_f(vw_x);

    // 适配飞腾 VSIP 库特有的 4 参数 release
    vsip_scalar_f *rp = 0;
    vsip_scalar_f *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);

    vsip_cblockdestroy_f(blk_x);

    return birmSuccess;
}