#include "birm_arm_float_User.h"

/**
 * @brief 复数矢量和复数标量的加法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvaddcs_f(const float *x1, const float *x2, const int nx, float *y)
{
	 if(!x1 || !y || !x2){return birmParamNullError;}
	 if(nx <= 0) {return birmParamLengthInvalidError;}

	 int i = 0;

	 for(i = 0;i < nx ;i++)
	 {
		 y[2*i] = x1[2*i] + x2[0];
		 y[2*i + 1] = x1[2*i + 1] + x2[1];
	 }

	 return birmSuccess;
}


