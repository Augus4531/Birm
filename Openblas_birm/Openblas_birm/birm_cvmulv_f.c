#include "birm_arm_float_User.h"
/**
 * @brief 浮点单精度复数矢量与单精度实数矢量的乘法
 * @param[in] x1 - 输入复数矢量指针x1
 * @param[in] x2 - 输入实数矢量指针x2
 * @param[in] nx - 复数矢量长度nx
 * @param[out] y - 输出矢量指针y
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
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
        y[2 * i] = x1[2 * i] * x2[i];
        y[2 * i + 1] = x1[2 * i + 1] * x2[i];
    }

    return birmSuccess;
}