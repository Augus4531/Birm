/**
 * @brief Vector clamp/clip to range - NEON optimized
 *
 * @param x1  Input vector pointer
 * @param x2  Maximum value (upper bound)
 * @param x3  Minimum value (lower bound)
 * @param nx  Vector length
 * @param y   Output vector pointer
 * @return int 0 for success, -1 for error
 *
 * Formula: y[i] = clamp(x1[i], x3, x2)
 *          = min(max(x1[i], x3), x2)
 *          If x1[i] > x2, then y[i] = x2
 *          If x1[i] < x3, then y[i] = x3
 *          Otherwise, y[i] = x1[i]
 *
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>
int birm_vclip_f(const float *x1, const float x2, const float x3, const int nx, float *y)
{
    if (!x1 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }
    if (x2 < x3)
    {
        return birmParamOtherError;
    }

    // Ensure min <= max
    float vmin = (x3 <= x2) ? x3 : x2;
    float vmax = (x3 <= x2) ? x2 : x3;

    // Broadcast min and max to NEON registers
    float32x4_t vmin_vec = vdupq_n_f32(vmin);
    float32x4_t vmax_vec = vdupq_n_f32(vmax);

    int idx = 0;

    // 32-way unrolling: process 32 elements at a time
    for (; idx <= nx - 32; idx += 32)
    {
        __builtin_prefetch(&x1[idx + 64], 0, 0);
        __builtin_prefetch(&y[idx + 64], 1, 0);

        // Load 32 elements (8 vectors)
        float32x4_t v0 = vld1q_f32(&x1[idx]);
        float32x4_t v1 = vld1q_f32(&x1[idx + 4]);
        float32x4_t v2 = vld1q_f32(&x1[idx + 8]);
        float32x4_t v3 = vld1q_f32(&x1[idx + 12]);
        float32x4_t v4 = vld1q_f32(&x1[idx + 16]);
        float32x4_t v5 = vld1q_f32(&x1[idx + 20]);
        float32x4_t v6 = vld1q_f32(&x1[idx + 24]);
        float32x4_t v7 = vld1q_f32(&x1[idx + 28]);

        // Clamp: first max with min, then min with max
        v0 = vmaxq_f32(v0, vmin_vec);
        v0 = vminq_f32(v0, vmax_vec);

        v1 = vmaxq_f32(v1, vmin_vec);
        v1 = vminq_f32(v1, vmax_vec);

        v2 = vmaxq_f32(v2, vmin_vec);
        v2 = vminq_f32(v2, vmax_vec);

        v3 = vmaxq_f32(v3, vmin_vec);
        v3 = vminq_f32(v3, vmax_vec);

        v4 = vmaxq_f32(v4, vmin_vec);
        v4 = vminq_f32(v4, vmax_vec);

        v5 = vmaxq_f32(v5, vmin_vec);
        v5 = vminq_f32(v5, vmax_vec);

        v6 = vmaxq_f32(v6, vmin_vec);
        v6 = vminq_f32(v6, vmax_vec);

        v7 = vmaxq_f32(v7, vmin_vec);
        v7 = vminq_f32(v7, vmax_vec);

        // Store results
        vst1q_f32(&y[idx], v0);
        vst1q_f32(&y[idx + 4], v1);
        vst1q_f32(&y[idx + 8], v2);
        vst1q_f32(&y[idx + 12], v3);
        vst1q_f32(&y[idx + 16], v4);
        vst1q_f32(&y[idx + 20], v5);
        vst1q_f32(&y[idx + 24], v6);
        vst1q_f32(&y[idx + 28], v7);
    }

    // 4-way NEON processing
    for (; idx <= nx - 4; idx += 4)
    {
        float32x4_t v = vld1q_f32(&x1[idx]);
        v = vmaxq_f32(v, vmin_vec);
        v = vminq_f32(v, vmax_vec);
        vst1q_f32(&y[idx], v);
    }

    // Scalar processing for remaining elements
    for (; idx < nx; idx++)
    {
        float val = x1[idx];
        if (val < vmin)
            val = vmin;
        if (val > vmax)
            val = vmax;
        y[idx] = val;
    }

    return birmSuccess;
}