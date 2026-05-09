#include "birm_arm_float_User.h"

/**
 * @brief 把2个16bit数构成的32bit整型数转化为浮点复数
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y,
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vint16tocv_f( const int *x, const int nx, float *y)
{
    if(!x || !y){return birmParamNullError;}
    if(nx<=0){return birmParamLengthInvalidError;}

    int i,xTmp;
    short xL, xH;

	for(i = 0; i < nx; i++)
	{
		xTmp = x[i];

		xL = xTmp & 0xFFFF;
		y[2*i] = (float)(xL);

		xH = (xTmp>>16) & 0xFFFF;
		y[2*i+1] =  (float)(xH);

	}

    return birmSuccess;
}

