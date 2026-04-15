#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 求方差
 * @param[in] x - 输入数据指针
 * @param[in] nx - 一维数据长度
 * @param[out] y - 方差值
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vvar_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }
    if (nx == 1)
    {
        *y = 0;
        return birmSuccess;
    }

    armpl_int_t incx = 1;
    float xsum = 0.0, xmean = 0.0, xmean2 = 0.0, x2 = 0.0, x2mean = 0.0, out = 0.0;
    int i = 0;

    for (i = 0; i < nx; i++)
    {
        xsum = xsum + x[i];
    }
    xmean = xsum / nx;
    xmean2 = xmean * xmean;
    x2 = sdot_((armpl_int_t *)&nx, x, &incx, x, &incx);
    x2mean = x2 / nx;
    out = x2mean - xmean2;
    *y = out;

    return birmSuccess;
}