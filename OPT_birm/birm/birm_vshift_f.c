#include "armpl.h"
#include "birm_arm_float_User.h"
#include <string.h> // For memcpy
#include <string.h>
#include <stdlib.h>

int birm_vshift_f(const float *x, int nx, int shift, float *y) {
    if (!x || !y)   return birmParamNullError;
    if (nx <= 0)    return birmParamLengthInvalidError;
    if (nx == 1) {
        y[0] = x[0];
        return birmSuccess;
    }

    // 归一化位移到 [0, nx)
    int k = shift % nx;
    if (k < 0) k += nx;          // k 是向右移位的量
    if (k == 0) {                // 不需移位
        if (x != y) memcpy(y, x, (size_t)nx * sizeof(float));
        return birmSuccess;
    }

    // 处理同址/异址统一路径：复制两段
    // y[0..k-1]   <- x[nx-k .. nx-1]
    // y[k..nx-1]  <- x[0 .. nx-k-1]
    // 如果 x==y，直接用临时缓冲防止覆盖
    float *tmp = NULL;
    const float *src = x;
    if (x == y) {
        tmp = (float*)malloc((size_t)nx * sizeof(float));
        if (!tmp) return birmParamNullError;
        src = tmp;
        memcpy(tmp, x, (size_t)nx * sizeof(float));
    }

    memcpy(y,             src + (nx - k), (size_t)k       * sizeof(float));
    memcpy(y + k,         src,            (size_t)(nx-k)  * sizeof(float));

    if (tmp) free(tmp);
    return birmSuccess;
}

