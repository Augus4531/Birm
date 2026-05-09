#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_cvaddcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !y || !x2)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先将 x2 拷贝到 y (长度为 2 * nx 个 float)
    memcpy(y, x2, nx * 2 * sizeof(float));

    // y = 1.0 * x1 + y (当作 2*nx 个实数进行按位加法，完全等效于复数加法)
    cblas_saxpy(nx * 2, 1.0f, x1, 1, y, 1);

    return birmSuccess;
}