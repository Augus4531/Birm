#include "birm_arm_float_User.h"
#include <fftw3.h>
#include <cblas.h>  // <--- 【修改点1】：用 cblas.h 替换 armpl.h

/**
 * @brief 复数矢量iFFT变换
 * @param[in] x  - 输入复数矢量指针 x
 * @param[in] fftN - iFFT点数
 * @param[out] y - 输出复数矢量指针 y,y与x可同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_ifft_f(const float *x, const int fftN, fftwf_plan ffthandle, float *y)
{
    if(!x || !y){return birmParamNullError;}
    if(fftN<=0){return birmParamLengthInvalidError;}

    float N = (1.0f / (float)fftN);
    
    // 1. 调用 FFTW 执行频域逆变换
    fftwf_execute_dft(ffthandle, (fftwf_complex*)x, (fftwf_complex*)y);
    
    // 2. 调用 OpenBLAS 执行 1/N 幅度归一化
    // 【修改点2】：去掉了特有的 (armpl_int_t) 强转，直接传 int 类型的 fftN
    cblas_csscal(fftN, N, y, 1);

    return birmSuccess;
}
