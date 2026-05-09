#include "birm_arm_float_User.h"

int birm_jv_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    };
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    for (int i = 0; i < nx * 2; i += 2)
    {
        y[i] = x[i];
        y[i + 1] = -x[i + 1];
    }
    return birmSuccess;
}