#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_cvmulcs_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先拷贝到目标内存 y，因为 cblas_cscal 是原地操作
    memcpy(y, x1, nx * 2 * sizeof(float));

    // alpha 为复数标量 x2，对 y 进行缩放：y = x2 * y
    cblas_cscal(nx, x2, y, 1);

    return birmSuccess;
}