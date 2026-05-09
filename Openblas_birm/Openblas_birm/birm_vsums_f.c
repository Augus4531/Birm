#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vsums_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 平方和 = x 与 x 自身的点乘
    y[0] = cblas_sdot(nx, x, 1, x, 1);

    return birmSuccess;
}