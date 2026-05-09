/**
 * @brief Vector fill with scalar (with stride) - NEON optimized
 * @note Final Corrected Logic: 'nx' is INPUT length.
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
int birm_vfill_sf(const float x1, const int nx, const int sb, float *y)
{
    if (!y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int idx = 0;

    if (sb == 1)
    {
        float32x4_t vx = vdupq_n_f32(x1);

        for (; idx <= nx - 32; idx += 32)
        {
            __builtin_prefetch(&y[idx + 64], 1, 0);

            vst1q_f32(&y[idx], vx);
            vst1q_f32(&y[idx + 4], vx);
            vst1q_f32(&y[idx + 8], vx);
            vst1q_f32(&y[idx + 12], vx);
            vst1q_f32(&y[idx + 16], vx);
            vst1q_f32(&y[idx + 20], vx);
            vst1q_f32(&y[idx + 24], vx);
            vst1q_f32(&y[idx + 28], vx);
        }

        for (; idx <= nx - 4; idx += 4)
        {
            vst1q_f32(&y[idx], vx);
        }
    }
    else if (sb == 2)
    {
        // vst2q 延迟太高，使用激进标量展开更快
        for (; idx <= nx - 16; idx += 16)
        {
            y[(idx + 0) * 2] = x1;
            y[(idx + 1) * 2] = x1;
            y[(idx + 2) * 2] = x1;
            y[(idx + 3) * 2] = x1;
            y[(idx + 4) * 2] = x1;
            y[(idx + 5) * 2] = x1;
            y[(idx + 6) * 2] = x1;
            y[(idx + 7) * 2] = x1;
            y[(idx + 8) * 2] = x1;
            y[(idx + 9) * 2] = x1;
            y[(idx + 10) * 2] = x1;
            y[(idx + 11) * 2] = x1;
            y[(idx + 12) * 2] = x1;
            y[(idx + 13) * 2] = x1;
            y[(idx + 14) * 2] = x1;
            y[(idx + 15) * 2] = x1;
        }

        for (; idx <= nx - 4; idx += 4)
        {
            y[(idx + 0) * 2] = x1;
            y[(idx + 1) * 2] = x1;
            y[(idx + 2) * 2] = x1;
            y[(idx + 3) * 2] = x1;
        }
    }

    for (; idx < nx; idx++)
    {
        y[idx * sb] = x1;
    }

    return birmSuccess;
}