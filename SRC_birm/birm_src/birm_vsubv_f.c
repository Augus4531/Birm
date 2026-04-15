#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度实数矢量与实数矢量的减法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsubv_f(const float *x1, const float *x2, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    armpl_int_t len = nx;
    float alpha = 1.0;
    float beta = -1.0;
    armpl_int_t inc = 1;

    cblas_swaxpby(len, alpha, x1, inc, beta, x2, inc, y, inc);

    return birmSuccess;
}