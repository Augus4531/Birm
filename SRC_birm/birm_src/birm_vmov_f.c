#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度实数矢量搬移
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmov_f(const float *x,const int nx, float *y)
{
	armpl_int_t  incx=1,incy=1;
	if(!x || !y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}
	scopy_(&nx, x, &incx, y, &incy);

	return birmSuccess;
}
