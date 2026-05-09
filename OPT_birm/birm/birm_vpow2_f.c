
#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <float.h>
#include <stdint.h>
#include "birm_arm_float_User.h"
// 稍慢
// 20260114变快了 0.072
int birm_vpow2_f(const int n, int *y)
{
    if (!y)
        return birmParamNullError;
    if (n <= 0)
        return birmParamLengthInvalidError;

    unsigned int nAbs = (unsigned int)n;
    int k = 0;

    if (nAbs > 1)
    {
        unsigned int tmp = nAbs - 1;
        unsigned int clz_res;
        // ARM64 汇编指令: clz (Count Leading Zeros)
        // %0 是输出 clz_res, %1 是输入 tmp
        __asm__("clz %w0, %w1" : "=r"(clz_res) : "r"(tmp));

        k = 32 - clz_res;
    }

    *y = k;
    return birmSuccess;
}
