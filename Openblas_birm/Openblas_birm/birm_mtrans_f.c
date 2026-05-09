#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_mtrans_f(const float *x, const int nn, const int mm, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nn <= 0 || mm <= 0)
        return birmParamLengthInvalidError;

    // cblas_somatcopy 能够进行 out-of-place 的矩阵转置
    cblas_somatcopy(CblasRowMajor, CblasTrans, nn, mm, 1.0f, x, mm, y, nn);

    return birmSuccess;
}