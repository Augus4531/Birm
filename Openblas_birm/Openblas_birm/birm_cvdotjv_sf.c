#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_cvdotjv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    // cblas_cdotc_sub 默认对第一个向量（x2）取共轭
    cblas_cdotc_sub(nx, x2, sa, x1, sb, y);

    return birmSuccess;
}