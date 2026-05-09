#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_cvmuls_if(float *x1, const float x2, const int nx)
{
    if (!x1)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // x1 = x2 * x1，其中 x2 是实数标量，x1 是复数向量，步长为1
    cblas_csscal(nx, x2, x1, 1);

    return birmSuccess;
}