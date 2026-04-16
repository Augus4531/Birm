#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量填充标量数,带步长
 * @param[in] x - 输入实数标量数
 * @param[in] nx - 输入实数矢量长度
 * @param[in] sb - 输入步长
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_vfill_sf(const float x, const int nx, const int sb, float *y)
{
    if (!y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;
    for (i = 0; i < nx; i++)
    {
        y[i * sb] = x;
    }

    return birmSuccess;
}
