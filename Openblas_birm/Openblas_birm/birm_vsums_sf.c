#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vsums_sf(const float *x, const int sa, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0)
        return birmParamLengthInvalidError;

    // 平方和即为矢量自身带步长的点乘
    y[0] = cblas_sdot(nx, x, sa, x, sa);

    return birmSuccess;
}