#include "birm_arm_float_User.h"

/**
 * @brief 浮点实数矢量进行数据范围限制
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入最大值 x2
 * @param[in] x3 - 输入最小值 x3
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vclip_f(const float *x1, const float x2, const float x3, const int nx, float *y)
{
    if (!x1 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }
    if (x2 < x3)
    {
        return birmParamOtherError;
    }

    int i = 0;
    for (i = 0; i < nx; i++)
    {
        if (x1[i] > x2)
        {
            y[i] = x2;
        }
        else if (x1[i] < x3)
        {
            y[i] = x3;
        }
        else
        {
            y[i] = x1[i];
        }
    }

    return birmSuccess;
}