#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vmuls_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb)
{
    if (!x1 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 1. 带步长拷贝: 从 x1(步长sa) 拷贝 nx 个元素到 y(步长sb)
    cblas_scopy(nx, x1, sa, y, sb);

    // 2. 带步长缩放: 将 y(步长sb) 上的元素乘以标量 x2
    cblas_sscal(nx, x2, y, sb);

    return birmSuccess;
}