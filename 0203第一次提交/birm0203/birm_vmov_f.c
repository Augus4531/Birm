#include <string.h> // 需要包含此头文件
#include "birm_arm_float_User.h"


int birm_vmov_f(const float* x, const int nx, float* y)
{
    if (!x || !y) { return birmParamNullError; }
    if (nx <= 0) { return birmParamLengthInvalidError; }

    // 直接内存块拷贝，编译器通常会将其内联或调用高度优化的汇编实现
    memcpy(y, x, nx * sizeof(float));

    return birmSuccess;
}
