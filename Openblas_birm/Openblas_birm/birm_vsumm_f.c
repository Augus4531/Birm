#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vsumm_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // sasum: sum of absolute values
    y[0] = cblas_sasum(nx, x, 1);

    return birmSuccess;
}