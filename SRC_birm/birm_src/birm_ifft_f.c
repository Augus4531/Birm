#include "birm_arm_float_User.h"
#include <fftw3.h>
#include "armpl.h"

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
	if (!x || !y)
	{
		return birmParamNullError;
	}
	if (fftN <= 0)
	{
		return birmParamLengthInvalidError;
	}

	float N = (1 / (float)fftN);
	fftwf_execute_dft(ffthandle, (fftwf_complex *)x, (fftwf_complex *)y);
	cblas_csscal((armpl_int_t)fftN, N, y, 1);

	return birmSuccess;
}
