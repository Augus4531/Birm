#include <arm_neon.h>
#include "birm_arm_float_User.h"
int birm_cvmulcs_f(const float * x1, const float * x2, const int nx, float * y)
{
    if(!x1 || !x2 || !y){return birmParamNullError;}
    if(nx <= 0){return birmParamLengthInvalidError;}

    int i = 0;
    int nx_loop = nx & ~3; // 处理 4 的倍数

    if (nx_loop > 0) {
        __asm__ volatile (
            // 加载标量 x2[0], x2[1] 并广播到 v2, v3
            "ld1r {v2.4s}, [%[x2]]\n\t"         // v2 = {c, c, c, c}
            "add x4, %[x2], #4\n\t"
            "ld1r {v3.4s}, [x4]\n\t"            // v3 = {d, d, d, d}

            "1:\n\t"
            // 加载 4 个复数: v0=实部(a), v1=虚部(b)
            "ld2 {v0.4s, v1.4s}, [%[x1]], #32\n\t"
            
            // 计算实部: res_re = a*c - b*d
            "fmul v4.4s, v0.4s, v2.4s\n\t"      // v4 = a*c
            "fmls v4.4s, v1.4s, v3.4s\n\t"      // v4 = a*c - b*d
            
            // 计算虚部: res_im = a*d + b*c
            "fmul v5.4s, v0.4s, v3.4s\n\t"      // v5 = a*d
            "fmla v5.4s, v1.4s, v2.4s\n\t"      // v5 = a*d + b*c
            
            // 存储结果
            "st2 {v4.4s, v5.4s}, [%[y]], #32\n\t"
            
            "subs %w[nx], %w[nx], #4\n\t"
            "b.ne 1b\n\t"
            : [x1] "+r" (x1), [y] "+r" (y), [nx] "+r" (nx_loop)
            : [x2] "r" (x2)
            : "v0", "v1", "v2", "v3", "v4", "v5", "x4", "cc", "memory"
        );
        i = nx & ~3;
    }

    // 处理剩余数据
    float c = x2[0], d = x2[1];
    for (; i < nx; i++) {
        float a = x1[0]; float b = x1[1];
        y[0] = a * c - b * d;
        y[1] = a * d + b * c;
        x1 += 2; y += 2;
    }

    return birmSuccess;
}