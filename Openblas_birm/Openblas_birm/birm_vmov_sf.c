#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vmov_sf(const float *x, const int sa, const int nx, float *y, const int sb)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // 带步长拷贝
    cblas_scopy(nx, x, sa, y, sb);

    return birmSuccess;
}