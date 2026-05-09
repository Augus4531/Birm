#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_cvsubcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !y || !x2)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先将 x1 拷贝到 y (长度为 2 * nx 个 float)
    memcpy(y, x1, nx * 2 * sizeof(float));

    // 执行 y = -1.0 * x2 + y
    cblas_saxpy(nx * 2, -1.0f, x2, 1, y, 1);

    return birmSuccess;
}