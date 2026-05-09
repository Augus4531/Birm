#include "birm_arm_float_User.h"
#include <cblas.h>

int birm_vshift_f(const float *x, const int nx, const int shift, float *y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int shiftLen;
    int shift1;

    if (shift > 0) // 右移
    {
        shift1 = shift % nx;
        shiftLen = nx - shift1;
        // 把 x 的前 shiftLen 个拷贝到 y 的 shift1 位置
        cblas_scopy(shiftLen, x, 1, y + shift1, 1);
        // 把 x 剩下的 shift1 个拷贝到 y 的头部
        cblas_scopy(shift1, x + shiftLen, 1, y, 1);
    }
    else if (shift < 0) // 左移
    {
        shift1 = (-shift) % nx;
        shiftLen = nx - shift1;
        // 把 x 的 shift1 之后的拷贝到 y 的头部
        cblas_scopy(shiftLen, x + shift1, 1, y, 1);
        // 把 x 的前 shift1 个拷贝到 y 的尾部
        cblas_scopy(shift1, x, 1, y + shiftLen, 1);
    }
    else // 不移位
    {
        cblas_scopy(nx, x, 1, y, 1);
    }

    return birmSuccess;
}