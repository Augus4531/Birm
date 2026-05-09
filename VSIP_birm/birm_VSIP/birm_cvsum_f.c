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

    // 绑定交织格式的复数块并创建复数视图
    vsip_cblock_f *blk_x = vsip_cblockbind_f((float *)x, NULL, nx, VSIP_MEM_NONE);
    vsip_cvview_f *vw_x = vsip_cvbind_f(blk_x, 0, 1, nx);

    vsip_cblockadmit_f(blk_x, VSIP_TRUE);

    // 【核心修改】：彻底规避存在 Bug 的 vsip_cvmeanval_f
    // 使用标准 VSIPL 接口，将复数视图拆分为实部和虚部的实数视图
    vsip_vview_f *vw_r = vsip_vrealview_f(vw_x);
    vsip_vview_f *vw_i = vsip_vimagview_f(vw_x);

    // 分别对实部和虚部使用基础的单精度实数求和
    // vsip_vsumval_f 是极度基础的函数，在任何厂家的库中都经过了严格测试和优化
    y[0] = vsip_vsumval_f(vw_r);
    y[1] = vsip_vsumval_f(vw_i);

    // 销毁实部和虚部视图
    vsip_vdestroy_f(vw_r);
    vsip_vdestroy_f(vw_i);

    // 销毁复数视图
    vsip_cvdestroy_f(vw_x);

    // 适配飞腾 VSIP 库特有的 4 参数 release
    vsip_scalar_f *rp = 0;
    vsip_scalar_f *ip = 0;
    vsip_cblockrelease_f(blk_x, VSIP_TRUE, &rp, &ip);

    vsip_cblockdestroy_f(blk_x);

    return birmSuccess;
}