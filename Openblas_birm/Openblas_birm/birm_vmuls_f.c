#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_vmuls_f(const float *x1, const float x2, const int nx, float *y)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 1. 将 x1 拷贝到 y
    memcpy(y, x1, nx * sizeof(float));

    // 2. 将 y 的所有元素乘以标量 x2: y = x2 * y
    cblas_sscal(nx, x2, y, 1);

    return birmSuccess;
}