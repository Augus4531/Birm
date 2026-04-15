#include "birm_arm_float_User.h"

/**
 * @brief 整数转化为浮点数
 * @param[in] x - 输入整数矢量指针
 * @param[in] nx - 输入矢量长度
 * @param[out] y - 输出浮点数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_int2float_f(const int *x, const int nx, float *y)
{
	int i=0;
	if(!x||!y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}
	for(i = 0;i<nx;i++){
		y[i]=(float)x[i];
	}
	return birmSuccess;
}

