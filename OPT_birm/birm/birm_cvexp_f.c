/**
 * @file birm_cvexp_f.c
 * @brief Real vector complex exponential with ultra-high precision implementation
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>

/**
 * @brief High-precision NEON vectorized cosine using double precision
 */
static inline float32x4_t vcos_f32_double_precision(float32x4_t x_f32)
{
    float32x2_t x_low = vget_low_f32(x_f32);
    float32x2_t x_high = vget_high_f32(x_f32);

    float64x2_t x_d_low = vcvt_f64_f32(x_low);
    float64x2_t x_d_high = vcvt_f64_f32(x_high);

    const float64x2_t c_2pi_d = vdupq_n_f64(6.283185307179586476925286766559);
    const float64x2_t c_pi_d = vdupq_n_f64(3.141592653589793238462643383279);
    const float64x2_t c_pi_2_d = vdupq_n_f64(1.570796326794896619231321691639);
    const float64x2_t c_inv_2pi_d = vdupq_n_f64(0.15915494309189533576888376337251);

    float64x2_t k_low = vmulq_f64(x_d_low, c_inv_2pi_d);
    k_low = vrndnq_f64(k_low);
    x_d_low = vmlsq_f64(x_d_low, k_low, c_2pi_d);

    uint64x2_t gt_pi_2_low = vcgtq_f64(x_d_low, c_pi_2_d);
    uint64x2_t lt_npi_2_low = vcltq_f64(x_d_low, vnegq_f64(c_pi_2_d));
    x_d_low = vbslq_f64(gt_pi_2_low, vsubq_f64(x_d_low, c_pi_d), x_d_low);
    x_d_low = vbslq_f64(lt_npi_2_low, vaddq_f64(x_d_low, c_pi_d), x_d_low);

    float64x2_t x2_low = vmulq_f64(x_d_low, x_d_low);

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

    float32x2_t result_f32_low = vcvt_f32_f64(result_low);
    float32x2_t result_f32_high = vcvt_f32_f64(result_high);

    return vcombine_f32(result_f32_low, result_f32_high);
}

/**
 * @brief High-precision NEON vectorized sine using double precision
 */
static inline float32x4_t vsin_f32_double_precision(float32x4_t x_f32)
{
    float32x2_t x_low = vget_low_f32(x_f32);
    float32x2_t x_high = vget_high_f32(x_f32);

    float64x2_t x_d_low = vcvt_f64_f32(x_low);
    float64x2_t x_d_high = vcvt_f64_f32(x_high);

    const float64x2_t c_2pi_d = vdupq_n_f64(6.283185307179586476925286766559);
    const float64x2_t c_pi_d = vdupq_n_f64(3.141592653589793238462643383279);
    const float64x2_t c_pi_2_d = vdupq_n_f64(1.570796326794896619231321691639);
    const float64x2_t c_inv_2pi_d = vdupq_n_f64(0.15915494309189533576888376337251);

    float64x2_t k_low = vmulq_f64(x_d_low, c_inv_2pi_d);
    k_low = vrndnq_f64(k_low);
    x_d_low = vmlsq_f64(x_d_low, k_low, c_2pi_d);

    uint64x2_t gt_pi_2_low = vcgtq_f64(x_d_low, c_pi_2_d);
    uint64x2_t lt_npi_2_low = vcltq_f64(x_d_low, vnegq_f64(c_pi_2_d));

    float64x2_t x_adj_low = x_d_low;
    x_adj_low = vbslq_f64(gt_pi_2_low, vsubq_f64(c_pi_d, x_d_low), x_adj_low);
    x_adj_low = vbslq_f64(lt_npi_2_low, vnegq_f64(vaddq_f64(c_pi_d, x_d_low)), x_adj_low);

    float64x2_t x2_low = vmulq_f64(x_adj_low, x_adj_low);

    const float64x2_t s1 = vdupq_n_f64(1.0);
    const float64x2_t s3 = vdupq_n_f64(-0.16666666666666666666);
    const float64x2_t s5 = vdupq_n_f64(0.00833333333333333333);
    const float64x2_t s7 = vdupq_n_f64(-0.000198412698412698413);
    const float64x2_t s9 = vdupq_n_f64(2.75573192239858906525e-6);
    const float64x2_t s11 = vdupq_n_f64(-2.50521083854417187751e-8);
    const float64x2_t s13 = vdupq_n_f64(1.60590438368216145994e-10);
    const float64x2_t s15 = vdupq_n_f64(-7.64716373181981647590e-13);

    float64x2_t result_low = s15;
    result_low = vmlaq_f64(s13, result_low, x2_low);
    result_low = vmlaq_f64(s11, result_low, x2_low);
    result_low = vmlaq_f64(s9, result_low, x2_low);
    result_low = vmlaq_f64(s7, result_low, x2_low);
    result_low = vmlaq_f64(s5, result_low, x2_low);
    result_low = vmlaq_f64(s3, result_low, x2_low);
    result_low = vmlaq_f64(s1, result_low, x2_low);
    result_low = vmulq_f64(result_low, x_adj_low);

    float64x2_t k_high = vmulq_f64(x_d_high, c_inv_2pi_d);
    k_high = vrndnq_f64(k_high);
    x_d_high = vmlsq_f64(x_d_high, k_high, c_2pi_d);

    uint64x2_t gt_pi_2_high = vcgtq_f64(x_d_high, c_pi_2_d);
    uint64x2_t lt_npi_2_high = vcltq_f64(x_d_high, vnegq_f64(c_pi_2_d));

    float64x2_t x_adj_high = x_d_high;
    x_adj_high = vbslq_f64(gt_pi_2_high, vsubq_f64(c_pi_d, x_d_high), x_adj_high);
    x_adj_high = vbslq_f64(lt_npi_2_high, vnegq_f64(vaddq_f64(c_pi_d, x_d_high)), x_adj_high);

    float64x2_t x2_high = vmulq_f64(x_adj_high, x_adj_high);

    float64x2_t result_high = s15;
    result_high = vmlaq_f64(s13, result_high, x2_high);
    result_high = vmlaq_f64(s11, result_high, x2_high);
    result_high = vmlaq_f64(s9, result_high, x2_high);
    result_high = vmlaq_f64(s7, result_high, x2_high);
    result_high = vmlaq_f64(s5, result_high, x2_high);
    result_high = vmlaq_f64(s3, result_high, x2_high);
    result_high = vmlaq_f64(s1, result_high, x2_high);
    result_high = vmulq_f64(result_high, x_adj_high);

    float32x2_t result_f32_low = vcvt_f32_f64(result_low);
    float32x2_t result_f32_high = vcvt_f32_f64(result_high);

    return vcombine_f32(result_f32_low, result_f32_high);
}

int birm_cvexp_f(const float *__restrict x, const int nx, float *__restrict y)
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

    // Process 16 elements at a time (produces 32 complex output values)
    for (; i <= nx - 16; i += 16)
    {
        __builtin_prefetch(&x[i + 16], 0, 3);
        __builtin_prefetch(&y[2 * (i + 16)], 1, 3);

        float32x4_t x0 = vld1q_f32(&x[i]);
        float32x4_t x1 = vld1q_f32(&x[i + 4]);
        float32x4_t x2 = vld1q_f32(&x[i + 8]);
        float32x4_t x3 = vld1q_f32(&x[i + 12]);

        float32x4_t cos0 = vcos_f32_double_precision(x0);
        float32x4_t sin0 = vsin_f32_double_precision(x0);
        float32x4_t cos1 = vcos_f32_double_precision(x1);
        float32x4_t sin1 = vsin_f32_double_precision(x1);
        float32x4_t cos2 = vcos_f32_double_precision(x2);
        float32x4_t sin2 = vsin_f32_double_precision(x2);
        float32x4_t cos3 = vcos_f32_double_precision(x3);
        float32x4_t sin3 = vsin_f32_double_precision(x3);

        float32x4x2_t result0;
        result0.val[0] = cos0;
        result0.val[1] = sin0;
        vst2q_f32(&y[2 * i], result0);

        float32x4x2_t result1;
        result1.val[0] = cos1;
        result1.val[1] = sin1;
        vst2q_f32(&y[2 * (i + 4)], result1);

        float32x4x2_t result2;
        result2.val[0] = cos2;
        result2.val[1] = sin2;
        vst2q_f32(&y[2 * (i + 8)], result2);

        float32x4x2_t result3;
        result3.val[0] = cos3;
        result3.val[1] = sin3;
        vst2q_f32(&y[2 * (i + 12)], result3);
    }

    // Process 4 elements at a time
    for (; i <= nx - 4; i += 4)
    {
        float32x4_t x_vec = vld1q_f32(&x[i]);

        float32x4_t cos_vec = vcos_f32_double_precision(x_vec);
        float32x4_t sin_vec = vsin_f32_double_precision(x_vec);

        float32x4x2_t result;
        result.val[0] = cos_vec;
        result.val[1] = sin_vec;

        vst2q_f32(&y[2 * i], result);
    }

    // Handle remaining elements with standard library
    for (; i < nx; i++)
    {
        y[2 * i] = cosf(x[i]);     // Real part
        y[2 * i + 1] = sinf(x[i]); // Imaginary part
    }

    return birmSuccess;
}
