#include "birm_arm_float_User.h"
#include <math.h>
/**
 * @brief 浮点单精度复数矢量求模
 * @param[in] x - 输入复数矢量指针
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvabs_f(const float *x, const int nx, float *y)
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
    for (i = 0; i < nx * 2; i += 2)
    {
        y[i / 2] = sqrt(x[i] * x[i] + x[i + 1] * x[i + 1]);
    }

    return birmSuccess;
}