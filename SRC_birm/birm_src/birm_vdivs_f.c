#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度实数矢量与实数标量的除法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vdivs_f(const float *x1, const float x2, const int nx, float *y)
{
    if (!x1 || !y)
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
        y[i] = x1[i] / x2;
    }
    return birmSuccess;
}