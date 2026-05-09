#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_cmtrans_f(const float *x, const int nn, const int mm, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nn <= 0 || mm <= 0)
        return birmParamLengthInvalidError;

    // alpha 为复数 1.0 + 0.0i，表示只转置不缩放
    float alpha[2] = {1.0f, 0.0f};

    // 输入为行主序，执行转置，行数nn，列数mm
    // 输入列跨度为 mm，输出列跨度转置后为 nn
    cblas_comatcopy(CblasRowMajor, CblasTrans, nn, mm, alpha, x, mm, y, nn);

    return birmSuccess;
}