#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量求绝对值
 * @param[in] x1 - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vabs_f(const float *x, const int nx, float *y)
{

    if (!x || !y)
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
        y[i] = (x[i] >= 0 ? x[i] : -x[i]);
    }

    return birmSuccess;
}