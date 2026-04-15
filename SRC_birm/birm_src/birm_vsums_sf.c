#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度实数矢量求平方和
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] sa - 输入 x 步长
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsums_sf(const float *x, const int sa, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0)
    {
        return birmParamLengthInvalidError;
    }

    armpl_int_t inc = sa;
    y[0] = cblas_sdot(nx, (float *)x, inc, (float *)x, inc);

    return birmSuccess;
}