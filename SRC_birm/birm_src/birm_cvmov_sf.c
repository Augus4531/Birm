#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度复数矢量与复数矢量的点乘
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数矢量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmov_sf(const float *x, const int sa, const int nx, float *y, const int sb)
{
	if (!x || !y)
	{
		return birmParamNullError;
	}
	if (nx <= 0 || sa <= 0 || sb <= 0)
	{
		return birmParamLengthInvalidError;
	}

	ccopy_((armpl_int_t *)&nx, (armpl_singlecomplex_t *)x, (armpl_int_t *)&sa, (armpl_singlecomplex_t *)y, (armpl_int_t *)&sb);

	return birmSuccess;
}
