#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vdotv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 直接带入步进参数 sa, sb 计算点乘
    y[0] = cblas_sdot(nx, x1, sa, x2, sb);

    return birmSuccess;
}