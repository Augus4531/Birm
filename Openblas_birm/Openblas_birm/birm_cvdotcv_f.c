#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_cvdotcv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 使用 _sub 后缀函数直接将结果写入输出指针 y
    cblas_cdotu_sub(nx, x1, 1, x2, 1, y);

    return birmSuccess;
}