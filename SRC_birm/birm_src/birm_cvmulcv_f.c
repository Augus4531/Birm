#include "birm_arm_float_User.h"
/**
 * @brief 复数矢量和复数矢量的乘法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y,
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvmulcv_f(const float *x1, const float *x2, const int nx, float *y)
{
	 if(!x1 || !y || !x2){return birmParamNullError;}
	 if(nx <= 0) {return birmParamLengthInvalidError;}

	 int i;
	float temp0 = 0.0;
	float temp1 = 0.0;
	float temp2 = 0.0;
	float temp3 = 0.0;
	 for(i = 0;i < nx ;i++)
	 {
			temp0 = x1[i*2];
			temp1 = x1[i*2+1];
			temp2 = x2[i*2];
			temp3 = x2[i*2 + 1];
			y[i*2] = temp0 * temp2 - temp1 * temp3;
			y[i*2+1] = temp0 * temp3 + temp1 * temp2;
	 }

	 return birmSuccess;
}
