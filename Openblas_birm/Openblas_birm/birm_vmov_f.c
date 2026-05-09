#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vmov_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    cblas_scopy(nx, x, 1, y, 1);

    return birmSuccess;
}