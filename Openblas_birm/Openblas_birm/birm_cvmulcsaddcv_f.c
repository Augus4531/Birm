#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_cvmulcsaddcv_f(const float *x1, const float *x2, const float *x3, const int nx, float *y)
{
    if (!x1 || !x2 || !x3 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先将 x3 拷贝到 y
    memcpy(y, x3, nx * 2 * sizeof(float));

    // 执行复数 axpy: y = x2 * x1 + y
    cblas_caxpy(nx, x2, x1, 1, y, 1);

    return birmSuccess;
}