#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "birm_arm_float_User.h"
/**
 * @brief Scalar implementation of finding minimum value and its index in real vector
 * 
 * @param x  Input real vector pointer
 * @param nx Input vector length (number of real numbers)
 * @param id Output index of minimum value
 * @param y  Output minimum value pointer (single float)
 * @return int 0 for success, -1 for error
 */


int birm_vmini_f(const float *x, const int nx, int *id, float *y)
{
    int i = 0;
    float min_val;
    int min_idx;

    if (!x || !id || !y) { return -1; }
    if (nx <= 0) { return -2; }

    // 初始化全局最小值为第一个元素
    min_val = x[0];
    min_idx = 0;

    // 如果数据量足够大，使用 4 路循环展开优化
    if (nx >= 4) {
        float min1 = x[0]; int idx1 = 0;
        float min2 = x[1]; int idx2 = 1;
        float min3 = x[2]; int idx3 = 2;
        float min4 = x[3]; int idx4 = 3;

        // 主循环：步长为 4
        // 使用 4 个独立变量打破流水线依赖，允许 CPU 并行比较
        for (i = 4; i <= nx - 4; i += 4) {
            if (x[i] < min1)   { min1 = x[i];   idx1 = i; }
            if (x[i+1] < min2) { min2 = x[i+1]; idx2 = i+1; }
            if (x[i+2] < min3) { min3 = x[i+2]; idx3 = i+2; }
            if (x[i+3] < min4) { min4 = x[i+3]; idx4 = i+3; }
        }

        // 归约：将 4 路结果合并
        if (min1 < min_val) { min_val = min1; min_idx = idx1; }
        if (min2 < min_val) { min_val = min2; min_idx = idx2; }
        if (min3 < min_val) { min_val = min3; min_idx = idx3; }
        if (min4 < min_val) { min_val = min4; min_idx = idx4; }
    }

    // 处理剩余的元素 (Tail Loop)
    // 处理 nx 无法被 4 整除的情况，或者 nx < 4 的情况
    for (; i < nx; i++) {
        if (x[i] < min_val) {
            min_val = x[i];
            min_idx = i;
        }
    }

    *y = min_val;
    *id = min_idx;
    return 0;
}
