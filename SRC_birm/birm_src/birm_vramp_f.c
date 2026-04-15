#include "birm_arm_float_User.h"

/**
 * Function: 产生上升或者下降的斜坡,y=a+b*i
 * @brief 浮点单精度复数矢量与单精度实数矢量的乘法
 * @param[in] x1 - 输入实数x1
 * @param[in] x2 - 输入实数x2
 * @param[in] nx - 离散点数nx
 * @param[out] y - 输出一维向量指针y
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vramp_f(const float x1,const float x2,const int nx,float *y)
{
	if(!y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}

	int i = 0;
	for(i = 0; i < nx; i++)
	{
		y[i] = x1 + i*x2 ;
	}

    return birmSuccess;
}
