#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_vaddv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先将 x2 拷贝到 y
    memcpy(y, x2, nx * sizeof(float));
    // 然后执行 y = 1.0 * x1 + y
    cblas_saxpy(nx, 1.0f, x1, 1, y, 1);

    return birmSuccess;
}