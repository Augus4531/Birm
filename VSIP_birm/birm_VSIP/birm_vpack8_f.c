#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数完成32bit数据压缩为8bit
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vpack8_f(const float *x, const int nx, int *y)
{
    if(!x || !y){return birmParamNullError;}
    if(nx<=0){return birmParamLengthInvalidError;}

    int i = 0;
	int iProcL;
	
	iProcL = nx-(nx%4);
	if((nx%4) == 1)
	{
		y[iProcL/4] = ((int)(x[nx-1]) & 0xff) | (0 << 8) | (0 << 16) | (0 << 24);
	}
	if((nx%4) == 2)
	{
		y[iProcL/4] = ((int)(x[nx-2]) & 0xff) | (((int)(x[nx-1]) & 0xff) << 8) | (0 << 16) | (0 << 24);
	}
	if((nx%4) == 3)
	{
		y[iProcL/4] = ((int)(x[nx-3]) & 0xff) | (((int)(x[nx-2]) & 0xff) << 8) | (((int)(x[nx-1]) & 0xff) << 16) | (0 << 24);
	}

	for(i=0; i<(iProcL/4); i++)
	{
		y[i] = ((int)(x[i*4]) & 0xff) | (((int)(x[i*4+1]) & 0xff)<<8) |\
				(((int)(x[i*4+2]) & 0xff)<<16) | (((int)(x[i*4+3]) & 0xff)<<24);
	}

    return birmSuccess;
}
