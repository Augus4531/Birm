#include "birm_arm_float_User.h"
#include <cblas.h>
#include <string.h>

int birm_vdivs_f(const float *x1, const float x2, const int nx, float *y)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 先拷贝到 y
    memcpy(y, x1, nx * sizeof(float));

    // 计算缩放因子并进行矢量乘法 y = (1.0 / x2) * y
    float inv_x2 = 1.0f / x2;
    cblas_sscal(nx, inv_x2, y, 1);

    return birmSuccess;
}