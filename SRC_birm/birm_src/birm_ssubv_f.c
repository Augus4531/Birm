#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数标量与单精度实数矢量的减法
 * @param[in] x1 - 输入实数标量 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x2与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_ssubv_f(const float x1, const float *x2, const int nx, float *y)
{
    if(!x2 || !y){return birmParamNullError;}
    if(nx<=0){return birmParamLengthInvalidError;}

    int i = 0;

	for(i = 0; i < nx; i++)
	{
		y[i] = x1 - x2[i];
	}

    return birmSuccess;
}
