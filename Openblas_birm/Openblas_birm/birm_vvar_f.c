#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vvar_f(const float *x, const int nx, float *y)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;
    if (nx == 1)
    {
        *y = 0.0f;
        return birmSuccess;
    }

    float xsum = 0.0f, xmean = 0.0f, xmean2 = 0.0f;
    float x2 = 0.0f, x2mean = 0.0f;
    int i = 0;

    // 1. 求代数和
    for (i = 0; i < nx; i++)
    {
        xsum += x[i];
    }
    xmean = xsum / nx;
    xmean2 = xmean * xmean;

    // 2. 利用 CBLAS 求平方和
    x2 = cblas_sdot(nx, x, 1, x, 1);
    x2mean = x2 / nx;

    // 3. 计算方差
    float out = x2mean - xmean2;
    // 拦截精度误差导致的微小负数
    if (out < 0.0f)
        out = 0.0f;

    *y = out;
    return birmSuccess;
}