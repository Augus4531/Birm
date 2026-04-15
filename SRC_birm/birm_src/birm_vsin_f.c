#include "birm_arm_float_User.h"
#include "math.h"

/**
 * @brief 浮点单精度实数矢量的正弦值
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsin_f(const float *x, const int nx, float *y)
{
	if(!x || !y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}

	int i;
	for(i=0;i<nx;i++)
	{
		y[i] = sinf(x[i]);
	}

	return birmSuccess;
}

