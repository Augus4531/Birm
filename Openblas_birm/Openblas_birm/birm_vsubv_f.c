#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_vsubv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先将 x1 拷贝到 y
    memcpy(y, x1, nx * sizeof(float));

    // 执行 y = -1.0 * x2 + y
    cblas_saxpy(nx, -1.0f, x2, 1, y, 1);

    return birmSuccess;
}