#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量填充标量数
 */
int birm_vfill_f(const float x, const int nx, float *y)
{
    if (!y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    for (i = 0; i < nx; i++)
    {
        y[i] = x;
    }

    return birmSuccess;
}
