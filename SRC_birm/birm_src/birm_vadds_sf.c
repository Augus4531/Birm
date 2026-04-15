#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的加法，带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长， 如果sa为负数，则先将x1指针偏移到末尾，然后再计算
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y不可以同源
 * @param[in] sb - 输入y步长，如果sb为负数，则先将y指针偏移到末尾，然后再计算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vadds_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb)
{
    if (!x1 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    for (i = 0; i < nx; i++)
    {
        y[i * sb] = x1[i * sa] + x2;
    }

    return birmSuccess;
}