#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度复数矢量求和
 * @param[in] x  - 输入复数矢量指针 x
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvsum_f(const float *x, const int nx, float *y)
{
	int i;
 	if(!x || !y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}

	y[0] = 0.0; y[1]=0.0;
	for(i = 0; i < nx; i++)
	{
		y[0] += x[i*2];
		y[1] += x[i*2+1];
	}

	return birmSuccess;
}
