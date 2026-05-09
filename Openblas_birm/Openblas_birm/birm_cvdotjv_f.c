#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_cvdotjv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // cblas_cdotc_sub 会自动对第一个参数 (在此是 x2) 取共轭，与原代码逻辑保持一致
    cblas_cdotc_sub(nx, x2, 1, x1, 1, y);

    return birmSuccess;
}