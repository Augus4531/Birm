#include <math.h>
#include <stddef.h>
#include "birm_arm_float_User.h"

// 包含ARM性能数学库头文件
#include "amath.h"

// 使用ARM性能库的高精度向量化对数函数
int birm_v10log_f(const float *x, const int nx, float *y)
{
    // 参数检查
    if (x == NULL || y == NULL)
    {
        return birmParamNullError; // 输入指针x或输出指针y为空
    }

    if (nx <= 0)
    {
        return birmParamLengthInvalidError; // 输入长度nx不大于0
    }

    int i = 0;

// 如果支持Neon向量化，使用向量化版本
#if defined(__ARM_NEON) || defined(__aarch64__)
    // 处理向量化部分（每次处理4个元素）
    const int vectorizable_elements = nx & ~3;

    for (; i < vectorizable_elements; i += 4)
    {
        // 加载4个float到Neon寄存器
        float32x4_t vec = vld1q_f32(x + i);

        // 使用ARM性能库的向量化log10函数
        float32x4_t result = armpl_vlog10q_f32(vec);

        // 存储结果
        vst1q_f32(y + i, result);
    }
#endif

    // 处理剩余元素（使用标量函数）
    for (; i < nx; i++)
    {
        // 对每个元素计算以10为底的对数
        if (x[i] > 0)
        {
            y[i] = log10f(x[i]); // 使用标准库函数
        }
        else
        {
            // 处理非正数输入（根据需求返回特定值或错误）
            y[i] = -INFINITY; // 对于非正数返回负无穷
        }
    }

    return birmSuccess; // 正常返回
}
