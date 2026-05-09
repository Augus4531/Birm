/**
 * @brief Complex vector multiply by complex vector conjugate (element-wise) - NEON optimized
 *
 * @param x1  Input complex vector pointer x1 (interleaved: [r0,i0,r1,i1,...])
 * @param x2  Input complex vector pointer x2 (interleaved: [r0,i0,r1,i1,...])
 * @param nx  Input vector length (number of complex numbers)
 * @param y   Output complex vector pointer y (interleaved)
 * @return int 0 for success, -1 for error
 *
 * Formula: y[i] = x1[i] * conj(x2[i]), for i = 0, 1, ..., nx-1
 *          where (a+bi) * conj(c+di) = (a+bi) * (c-di) = (ac+bd) + (bc-ad)i
 *
 */
#include "birm_arm_float_User.h"
#include <arm_neon.h>
#include <math.h>

/**
 * @brief Complex Vector Multiply Conjugate: y = x1 * conj(x2)
 * Optimized for Phytium S5000C (ARMv8)
 * * Formula:
 * Let x1 = a + bi
 * Let x2 = c + di, conj(x2) = c - di
 * y = (a + bi)(c - di) = (ac + bd) + i(bc - ad)
 * * Optimization:
 * - Uses vld2q/vst2q for zero-overhead de-interleaving/interleaving
 * - 32-way Unrolling for latency hiding
 * - Fused Multiply-Add/Subtract
 */
int birm_cvmuljv_f(const float *__restrict__ x1,
                   const float *__restrict__ x2,
                   const int nx,
                   float *__restrict__ y)
{
    if (!x1 || !y || !x2)
    {
        return birmParamNullError;
    }
    if (nx <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    // ---------------------------------------------------------
    // Case 1: Main Loop - Process 32 Complex Numbers per iter
    // Data size per iter: 32 * 8 = 256 Bytes per array
    // ---------------------------------------------------------
    for (; i <= nx - 32; i += 32)
    {
        // === Block 1: First 8 Complex Numbers ===
        // vld2q load: .val[0] = Real parts, .val[1] = Imag parts
        float32x4x2_t ra_0 = vld2q_f32(&x1[2 * i]);
        float32x4x2_t ra_1 = vld2q_f32(&x1[2 * i + 8]);

        float32x4x2_t rb_0 = vld2q_f32(&x2[2 * i]);
        float32x4x2_t rb_1 = vld2q_f32(&x2[2 * i + 8]);

        // Calc Real: ac + bd
        float32x4_t re_0 = vmulq_f32(ra_0.val[0], rb_0.val[0]);
        float32x4_t re_1 = vmulq_f32(ra_1.val[0], rb_1.val[0]);
        re_0 = vmlaq_f32(re_0, ra_0.val[1], rb_0.val[1]);
        re_1 = vmlaq_f32(re_1, ra_1.val[1], rb_1.val[1]);

        // Calc Imag: bc - ad
        // Note: vmlsq_f32(a, b, c) -> a - (b * c)
        float32x4_t im_0 = vmulq_f32(ra_0.val[1], rb_0.val[0]); // b*c
        float32x4_t im_1 = vmulq_f32(ra_1.val[1], rb_1.val[0]);
        im_0 = vmlsq_f32(im_0, ra_0.val[0], rb_0.val[1]); // - a*d
        im_1 = vmlsq_f32(im_1, ra_1.val[0], rb_1.val[1]);

        // Store Block 1
        float32x4x2_t res_0 = {{re_0, im_0}};
        float32x4x2_t res_1 = {{re_1, im_1}};
        vst2q_f32(&y[2 * i], res_0);
        vst2q_f32(&y[2 * i + 8], res_1);

        // === Block 2: Next 8 Complex Numbers ===
        float32x4x2_t ra_2 = vld2q_f32(&x1[2 * i + 16]);
        float32x4x2_t ra_3 = vld2q_f32(&x1[2 * i + 24]);

        float32x4x2_t rb_2 = vld2q_f32(&x2[2 * i + 16]);
        float32x4x2_t rb_3 = vld2q_f32(&x2[2 * i + 24]);

        float32x4_t re_2 = vmulq_f32(ra_2.val[0], rb_2.val[0]);
        float32x4_t re_3 = vmulq_f32(ra_3.val[0], rb_3.val[0]);
        re_2 = vmlaq_f32(re_2, ra_2.val[1], rb_2.val[1]);
        re_3 = vmlaq_f32(re_3, ra_3.val[1], rb_3.val[1]);

        float32x4_t im_2 = vmulq_f32(ra_2.val[1], rb_2.val[0]);
        float32x4_t im_3 = vmulq_f32(ra_3.val[1], rb_3.val[0]);
        im_2 = vmlsq_f32(im_2, ra_2.val[0], rb_2.val[1]);
        im_3 = vmlsq_f32(im_3, ra_3.val[0], rb_3.val[1]);

        float32x4x2_t res_2 = {{re_2, im_2}};
        float32x4x2_t res_3 = {{re_3, im_3}};
        vst2q_f32(&y[2 * i + 16], res_2);
        vst2q_f32(&y[2 * i + 24], res_3);

        // === Block 3: Next 8 Complex Numbers ===
        float32x4x2_t ra_4 = vld2q_f32(&x1[2 * i + 32]);
        float32x4x2_t ra_5 = vld2q_f32(&x1[2 * i + 40]);

        float32x4x2_t rb_4 = vld2q_f32(&x2[2 * i + 32]);
        float32x4x2_t rb_5 = vld2q_f32(&x2[2 * i + 40]);

        float32x4_t re_4 = vmulq_f32(ra_4.val[0], rb_4.val[0]);
        float32x4_t re_5 = vmulq_f32(ra_5.val[0], rb_5.val[0]);
        re_4 = vmlaq_f32(re_4, ra_4.val[1], rb_4.val[1]);
        re_5 = vmlaq_f32(re_5, ra_5.val[1], rb_5.val[1]);

        float32x4_t im_4 = vmulq_f32(ra_4.val[1], rb_4.val[0]);
        float32x4_t im_5 = vmulq_f32(ra_5.val[1], rb_5.val[0]);
        im_4 = vmlsq_f32(im_4, ra_4.val[0], rb_4.val[1]);
        im_5 = vmlsq_f32(im_5, ra_5.val[0], rb_5.val[1]);

        float32x4x2_t res_4 = {{re_4, im_4}};
        float32x4x2_t res_5 = {{re_5, im_5}};
        vst2q_f32(&y[2 * i + 32], res_4);
        vst2q_f32(&y[2 * i + 40], res_5);

        // === Block 4: Next 8 Complex Numbers ===
        float32x4x2_t ra_6 = vld2q_f32(&x1[2 * i + 48]);
        float32x4x2_t ra_7 = vld2q_f32(&x1[2 * i + 56]);

        float32x4x2_t rb_6 = vld2q_f32(&x2[2 * i + 48]);
        float32x4x2_t rb_7 = vld2q_f32(&x2[2 * i + 56]);

        float32x4_t re_6 = vmulq_f32(ra_6.val[0], rb_6.val[0]);
        float32x4_t re_7 = vmulq_f32(ra_7.val[0], rb_7.val[0]);
        re_6 = vmlaq_f32(re_6, ra_6.val[1], rb_6.val[1]);
        re_7 = vmlaq_f32(re_7, ra_7.val[1], rb_7.val[1]);

        float32x4_t im_6 = vmulq_f32(ra_6.val[1], rb_6.val[0]);
        float32x4_t im_7 = vmulq_f32(ra_7.val[1], rb_7.val[0]);
        im_6 = vmlsq_f32(im_6, ra_6.val[0], rb_6.val[1]);
        im_7 = vmlsq_f32(im_7, ra_7.val[0], rb_7.val[1]);

        float32x4x2_t res_6 = {{re_6, im_6}};
        float32x4x2_t res_7 = {{re_7, im_7}};
        vst2q_f32(&y[2 * i + 48], res_6);
        vst2q_f32(&y[2 * i + 56], res_7);
    }

    // ---------------------------------------------------------
    // Case 2: Handle remaining 4-complex blocks
    // ---------------------------------------------------------
    for (; i <= nx - 4; i += 4)
    {
        float32x4x2_t ra = vld2q_f32(&x1[2 * i]);
        float32x4x2_t rb = vld2q_f32(&x2[2 * i]);

        // Re = ac + bd
        float32x4_t re = vmulq_f32(ra.val[0], rb.val[0]);
        re = vmlaq_f32(re, ra.val[1], rb.val[1]);

        // Im = bc - ad
        float32x4_t im = vmulq_f32(ra.val[1], rb.val[0]);
        im = vmlsq_f32(im, ra.val[0], rb.val[1]);

        float32x4x2_t res = {{re, im}};
        vst2q_f32(&y[2 * i], res);
    }

    // ---------------------------------------------------------
    // Case 3: Handle Scalar Tail
    // ---------------------------------------------------------
    for (; i < nx; i++)
    {
        float a = x1[2 * i];
        float b = x1[2 * i + 1];
        float c = x2[2 * i];
        float d = x2[2 * i + 1];

        // y = (a+bi)(c-di) = (ac+bd) + i(bc-ad)
        y[2 * i] = a * c + b * d;
        y[2 * i + 1] = b * c - a * d;
    }

    return birmSuccess;
}