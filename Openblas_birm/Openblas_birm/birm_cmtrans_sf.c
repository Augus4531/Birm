#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_cmtrans_sf(const float *x, const int nn, const int mm, const int sa, const int sb, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nn <= 0 || mm <= 0 || sa <= 0 || sb <= 0 || sb > nn || sa > mm)
        return birmParamLengthInvalidError;

    float alpha[2] = {1.0f, 0.0f};

    // 转置范围为 sa * sb。
    // 原矩阵列跨度为 mm，目标矩阵（截取转置后）列跨度为 sa
    cblas_comatcopy(CblasRowMajor, CblasTrans, sa, sb, alpha, x, mm, y, sa);

    return birmSuccess;
}