#include "birm_arm_float_User.h"
#include "armpl.h"
/**
 * @brief 复数矢量和实数标量的乘法
 * @param[in] x1 - 输入输出复数矢量指针 x1(输入输出同一个指针)
 * @param[in] x2 - 输入复数标量     x2
 * @param[in] nx - 输入复数矢量长度
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvmuls_if(float *x1, const float x2, const int nx)
{
    if (!x1)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    armpl_int_t inc = 1;

    cblas_csscal((armpl_int_t)nx, x2, x1, inc);

    return birmSuccess;
}