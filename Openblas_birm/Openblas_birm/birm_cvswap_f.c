#include "birm_arm_float_User.h"

int birm_cvswap_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    float k;
    for (i = 0; i < 2 * nx; i = i + 2)
    {
        k = x[i];
        y[i] = x[i + 1];
        y[i + 1] = k;
    }
    return birmSuccess;
}