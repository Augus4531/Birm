#include "birm_arm_float_User.h"

/**
 * @brief 复数矢量与复数标量的乘
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulcs_f(const float * x1, const float * x2, const int nx, float * y)
{
	if(!x1 || !x2 || !y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}

	int i = 0;
	float temp0 = 0.0;
	float temp1 = 0.0;
	for(i = 0; i < nx; i++)
	{
		temp0 = x1[i*2];
		temp1 = x1[i*2+1];
		y[i*2] = temp0 * x2[0] - temp1 * x2[1];
		y[i*2+1] = temp0 * x2[1] + temp1 * x2[0];
	}

	return birmSuccess;
}
