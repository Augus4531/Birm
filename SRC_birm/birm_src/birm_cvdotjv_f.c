#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度复数矢量与复数矢量共轭的点乘
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数矢量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvdotjv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    armpl_int_t inc = 1;
    armpl_singlecomplex_t *_y = (armpl_singlecomplex_t *)y;
    _y[0] = cdotc_(&nx, (armpl_singlecomplex_t *)x2, &inc, (armpl_singlecomplex_t *)x1, &inc);

    return birmSuccess;
}
