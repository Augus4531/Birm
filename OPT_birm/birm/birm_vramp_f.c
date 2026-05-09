#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "birm_arm_float_User.h"

int birm_vramp_f(const float x1, const float x2, const int nx, float *y)
{
    if (y == NULL)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    // 使用更小的块大小减少累积误差
    const int BLOCK_SIZE = 256; // 减小块大小

    // 使用双精度进行关键计算
    double x1_double = (double)x1;
    double x2_double = (double)x2;

    int idx = 0;

    // 分块计算，每个块独立计算起始值
    for (; idx <= nx - BLOCK_SIZE; idx += BLOCK_SIZE)
    {
        // 使用双精度计算起始值，避免累积误差
        double block_start = x1_double + x2_double * idx;

        // 使用NEON优化块内计算，但保持高精度
        float32x4_t vresult;
        float32x4_t vstep = vdupq_n_f32(4.0f * x2);

        // 使用双精度计算前4个值，然后转换为单精度
        for (int j = 0; j < 4; j++)
        {
            double precise_value = block_start + x2_double * j;
            vresult[j] = (float)precise_value;
        }

        for (int i = 0; i < BLOCK_SIZE; i += 4)
        {
            vst1q_f32(&y[idx + i], vresult);
            vresult = vaddq_f32(vresult, vstep);
        }
    }

    // 处理剩余元素 - 使用双精度避免累积误差
    for (; idx < nx; idx++)
    {
        double precise_value = x1_double + x2_double * idx;
        y[idx] = (float)precise_value;
    }

    return birmSuccess;
}
