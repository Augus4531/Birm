#include "birm_arm_float_User.h"
#include <fftw3.h>

/**
 * @brief 复数矢量FFT变换
 * @param[in] x  - 输入复数矢量指针 x
 * @param[in] fftN - FFT点数
 * @param[in] ffthandle - FFT句柄
 * @param[out] y - 输出复数矢量指针 y,y与x可同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_fft_f(const float *x, const int fftN, fftwf_plan ffthandle, float *y)
{
	if (!x || !y)
	{
		return birmParamNullError;
	}
	if (fftN <= 0)
	{
		return birmParamLengthInvalidError;
	}

	fftwf_execute_dft(ffthandle, (fftwf_complex *)x, (fftwf_complex *)y);
	return birmSuccess;
}
