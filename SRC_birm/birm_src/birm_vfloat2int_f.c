#include "birm_arm_float_User.h"
#include <math.h>

/**
 * @brief 转浮点实数化为整数
 * @param[in] x - 输入浮点数矢量指针
 * @param[in] nx - 输入矢量长度
 * @param[out] y - 输出整数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vfloat2int_f(const float *x, const int nx, int *y)
{
	if(!x||!y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}
	int i=0;
	for(i=0;i<nx;i++)
	{
		y[i]=(int)floor(x[i]);
	}
	return birmSuccess;
}

