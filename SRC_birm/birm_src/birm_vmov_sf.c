#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度实数矢量带步进搬移
 * @param[in] x - 输入实数矢量指针
 * @param[in] sa - 输入实数矢量x步长
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输出实数矢量步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmov_sf(const float *x, const int sa, const int nx, float *y, const int sb)
{
	if(!x || !y){return birmParamNullError;}
	if(nx<=0 || sa<=0 || sb<=0){return birmParamLengthInvalidError;}

	scopy_(&nx, x, &sa, y, &sb);


	return birmSuccess;
}
