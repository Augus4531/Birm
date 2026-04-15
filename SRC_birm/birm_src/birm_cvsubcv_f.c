#include "birm_arm_float_User.h"
#include "armpl.h"
/**
 * @brief 复数矢量和复数矢量的减法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvsubcv_f(const float *x1, const float *x2, const int nx, float *y)
{
	if(!x1 || !y || !x2){return birmParamNullError;}
	if(nx <= 0) {return birmParamLengthInvalidError;}

	armpl_int_t len = nx*2;
	float alpha = 1.0;
	float beta = -1.0;
	armpl_int_t inc = 1;

	cblas_swaxpby(len,alpha,x1,inc,beta,x2,inc,y,inc);

	return birmSuccess;
}
