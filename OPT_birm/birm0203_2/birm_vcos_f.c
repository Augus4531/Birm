/**
 * @file birm_vcos_f.c
 * @brief Real vector cosine with ultra-high precision implementation
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>

/**
 * @brief High-precision NEON vectorized cosine using double precision
 */
static inline float32x4_t vcos_f32_double_precision(float32x4_t x_f32)
{
    // Convert to double precision for calculation
    float32x2_t x_low = vget_low_f32(x_f32);
    float32x2_t x_high = vget_high_f32(x_f32);

    float64x2_t x_d_low = vcvt_f64_f32(x_low);
    float64x2_t x_d_high = vcvt_f64_f32(x_high);

    // High precision constants in double
    const float64x2_t c_2pi_d = vdupq_n_f64(6.283185307179586476925286766559);
    const float64x2_t c_pi_d = vdupq_n_f64(3.141592653589793238462643383279);
    const float64x2_t c_pi_2_d = vdupq_n_f64(1.570796326794896619231321691639);
    const float64x2_t c_inv_2pi_d = vdupq_n_f64(0.15915494309189533576888376337251);

    // Process low part
    float64x2_t k_low = vmulq_f64(x_d_low, c_inv_2pi_d);
    k_low = vrndnq_f64(k_low);
    x_d_low = vmlsq_f64(x_d_low, k_low, c_2pi_d);

    uint64x2_t gt_pi_2_low = vcgtq_f64(x_d_low, c_pi_2_d);
    uint64x2_t lt_npi_2_low = vcltq_f64(x_d_low, vnegq_f64(c_pi_2_d));
    x_d_low = vbslq_f64(gt_pi_2_low, vsubq_f64(x_d_low, c_pi_d), x_d_low);
    x_d_low = vbslq_f64(lt_npi_2_low, vaddq_f64(x_d_low, c_pi_d), x_d_low);

    float64x2_t x2_low = vmulq_f64(x_d_low, x_d_low);

    // High precision polynomial coefficients (double)
    const float64x2_t d0 = vdupq_n_f64(1.0);
    const float64x2_t d1 = vdupq_n_f64(-0.5);
    const float64x2_t d2 = vdupq_n_f64(0.04166666666666666666);
    const float64x2_t d3 = vdupq_n_f64(-0.001388888888888888888);
    const float64x2_t d4 = vdupq_n_f64(2.48015873015873015873e-5);
    const float64x2_t d5 = vdupq_n_f64(-2.75573192239858906525e-7);
    const float64x2_t d6 = vdupq_n_f64(2.08767569878680989792e-9);
    const float64x2_t d7 = vdupq_n_f64(-1.14707455977297247138e-11);
    const float64x2_t d8 = vdupq_n_f64(4.77947733238738529743e-14);
    const float64x2_t d9 = vdupq_n_f64(-1.56192069685862264622e-16);

    float64x2_t result_low = d9;
    result_low = vmlaq_f64(d8, result_low, x2_low);
    result_low = vmlaq_f64(d7, result_low, x2_low);
    result_low = vmlaq_f64(d6, result_low, x2_low);
    result_low = vmlaq_f64(d5, result_low, x2_low);
    result_low = vmlaq_f64(d4, result_low, x2_low);
    result_low = vmlaq_f64(d3, result_low, x2_low);
    result_low = vmlaq_f64(d2, result_low, x2_low);
    result_low = vmlaq_f64(d1, result_low, x2_low);
    result_low = vmlaq_f64(d0, result_low, x2_low);

    uint64x2_t need_negate_low = vorrq_u64(gt_pi_2_low, lt_npi_2_low);
    result_low = vbslq_f64(need_negate_low, vnegq_f64(result_low), result_low);

    // Process high part
    float64x2_t k_high = vmulq_f64(x_d_high, c_inv_2pi_d);
    k_high = vrndnq_f64(k_high);
    x_d_high = vmlsq_f64(x_d_high, k_high, c_2pi_d);

    uint64x2_t gt_pi_2_high = vcgtq_f64(x_d_high, c_pi_2_d);
    uint64x2_t lt_npi_2_high = vcltq_f64(x_d_high, vnegq_f64(c_pi_2_d));
    x_d_high = vbslq_f64(gt_pi_2_high, vsubq_f64(x_d_high, c_pi_d), x_d_high);
    x_d_high = vbslq_f64(lt_npi_2_high, vaddq_f64(x_d_high, c_pi_d), x_d_high);

    float64x2_t x2_high = vmulq_f64(x_d_high, x_d_high);

    float64x2_t result_high = d9;
    result_high = vmlaq_f64(d8, result_high, x2_high);
    result_high = vmlaq_f64(d7, result_high, x2_high);
    result_high = vmlaq_f64(d6, result_high, x2_high);
    result_high = vmlaq_f64(d5, result_high, x2_high);
    result_high = vmlaq_f64(d4, result_high, x2_high);
    result_high = vmlaq_f64(d3, result_high, x2_high);
    result_high = vmlaq_f64(d2, result_high, x2_high);
    result_high = vmlaq_f64(d1, result_high, x2_high);
    result_high = vmlaq_f64(d0, result_high, x2_high);

    uint64x2_t need_negate_high = vorrq_u64(gt_pi_2_high, lt_npi_2_high);
    result_high = vbslq_f64(need_negate_high, vnegq_f64(result_high), result_high);

    // Convert back to float
    float32x2_t result_f32_low = vcvt_f32_f64(result_low);
    float32x2_t result_f32_high = vcvt_f32_f64(result_high);

    return vcombine_f32(result_f32_low, result_f32_high);
}

int birm_vcos_f(const float *__restrict x, const int nx, float *__restrict y)
{
    if (!x || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    // Process 16 elements at a time with double precision intermediate calculations
    for (; i <= nx - 16; i += 16)
    {
        __builtin_prefetch(&x[i + 16], 0, 3);
        __builtin_prefetch(&y[i + 16], 1, 3);

        float32x4_t v0 = vld1q_f32(&x[i]);
        float32x4_t v1 = vld1q_f32(&x[i + 4]);
        float32x4_t v2 = vld1q_f32(&x[i + 8]);
        float32x4_t v3 = vld1q_f32(&x[i + 12]);

        float32x4_t r0 = vcos_f32_double_precision(v0);
        float32x4_t r1 = vcos_f32_double_precision(v1);
        float32x4_t r2 = vcos_f32_double_precision(v2);
        float32x4_t r3 = vcos_f32_double_precision(v3);

        vst1q_f32(&y[i], r0);
        vst1q_f32(&y[i + 4], r1);
        vst1q_f32(&y[i + 8], r2);
        vst1q_f32(&y[i + 12], r3);
    }

    // Process 4 elements at a time
    for (; i <= nx - 4; i += 4)
    {
        float32x4_t v = vld1q_f32(&x[i]);
        float32x4_t r = vcos_f32_double_precision(v);
        vst1q_f32(&y[i], r);
    }

    // Handle remaining elements with standard library
    for (; i < nx; i++)
    {
        y[i] = cosf(x[i]);
    }

    return birmSuccess;
}
