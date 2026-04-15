#include "birm_arm_float_User.h"
/**
 * @brief 把2个16bit数构成的32bit整型数转化为浮点复数,带步长
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  sa - 输入矢量指针 x的步长
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y
 * @param[in]  sb - 输出矢量指针y的步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_vint16tocv_sf(const int *x, const int sa, const int nx, float *y, const int sb)
{
    if(!x || !y){return birmParamNullError;}
    if(nx<=0 || sa<=0 || sb<=0){return birmParamLengthInvalidError;}

    int i,xTmp;
    short xL, xH;

	for(i = 0; i < nx; i++)
	{
		xTmp = x[i*sa];

		xL = xTmp & 0xFFFF;
		y[2*i*sb] = (float)(xL);

		xH = (xTmp>>16) & 0xFFFF;
		y[2*i*sb+1] =  (float)(xH);


	}

    return birmSuccess;
}

