#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vdotv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 直接调用 cblas_sdot 计算实数点乘
    y[0] = cblas_sdot(nx, x1, 1, x2, 1);

    return birmSuccess;
}