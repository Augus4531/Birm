#include "birm_arm_float_User.h"

/**
 * @brief 复数矢量与复数标量的乘加
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulcsaddcv_f(const float *x1, const float *x2, const float *x3, const int nx, float *y)
{
    if (!x1 || !x2 || !x3 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    for (i = 0; i < nx; i++)
    {
        y[i * 2] = x1[i * 2] * x2[0] - x1[i * 2 + 1] * x2[1] + x3[i * 2];
        y[i * 2 + 1] = x1[i * 2] * x2[1] + x1[i * 2 + 1] * x2[0] + x3[i * 2 + 1];
    }

    return birmSuccess;
}