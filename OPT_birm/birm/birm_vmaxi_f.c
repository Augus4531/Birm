#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "birm_arm_float_User.h"
/**
 * @brief 标量实现 - 查找浮点数组最大值及其索引
 *
 * @param x 输入数组指针
 * @param nx 数组长度
 * @param id 最大值索引输出指针
 * @param y 最大值输出指针
 * @return int 0成功，-1错误
 */

int birm_vmaxi_f(const float *x, const int nx, int *id, float *y)
{
    int i = 0;
    float max_val;
    int max_idx;

    // 参数检查
    if (!x || !id || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 初始化全局最大值为第一个元素
    max_val = x[0];
    max_idx = 0;

    // 如果数据量 >= 4，进入并行优化路径
    if (nx >= 4)
    {
        // 初始化4路并行变量
        float max1 = x[0];
        int idx1 = 0;
        float max2 = x[1];
        int idx2 = 1;
        float max3 = x[2];
        int idx3 = 2;
        float max4 = x[3];
        int idx4 = 3;

        // 4路循环展开，步长为4
        // 修改点：将判断条件由 < 改为 >
        for (i = 4; i <= nx - 4; i += 4)
        {
            if (x[i] > max1)
            {
                max1 = x[i];
                idx1 = i;
            }
            if (x[i + 1] > max2)
            {
                max2 = x[i + 1];
                idx2 = i + 1;
            }
            if (x[i + 2] > max3)
            {
                max3 = x[i + 2];
                idx3 = i + 2;
            }
            if (x[i + 3] > max4)
            {
                max4 = x[i + 3];
                idx4 = i + 3;
            }
        }

        // 归约：将4路局部最大值合并到全局最大值
        if (max1 > max_val)
        {
            max_val = max1;
            max_idx = idx1;
        }
        if (max2 > max_val)
        {
            max_val = max2;
            max_idx = idx2;
        }
        if (max3 > max_val)
        {
            max_val = max3;
            max_idx = idx3;
        }
        if (max4 > max_val)
        {
            max_val = max4;
            max_idx = idx4;
        }
    }

    // 处理剩余的尾部元素 (Tail Loop)
    for (; i < nx; i++)
    {
        if (x[i] > max_val)
        {
            max_val = x[i];
            max_idx = i;
        }
    }

    *y = max_val;
    *id = max_idx;
    return birmSuccess;
}
