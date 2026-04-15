#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量与实数矢量的乘法,带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmulv_sf(const float *x1, const int sa1, const float *x2, const int sa2, const int nx, float *y, const int sb)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa1 <= 0 || sa2 <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    for (i = 0; i < nx; i++)
    {
        y[i * sb] = x1[i * sa1] * x2[i * sa2];
    }

    return birmSuccess;
}
