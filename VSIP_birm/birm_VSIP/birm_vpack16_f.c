#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数完成32bit数据压缩为16bit
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vpack16_f(const float *x, const int nx, int *y)
{
    if(!x || !y){return birmParamNullError;}
    if(nx<=0){return birmParamLengthInvalidError;}

    int i,iProcL;

    iProcL = nx-(nx%2);
    if((nx%2) != 0)
    {
    	y[iProcL/2] = ((int)(x[nx-1]) & 0xffff) | (0 << 16);
    }
	for(i=0; i<iProcL/2; i++)
	{
		y[i] = ((int)(x[i*2]) & 0xffff) | (((int)(x[i*2+1]) & 0xffff)<<16);
	}

    return birmSuccess;
}
