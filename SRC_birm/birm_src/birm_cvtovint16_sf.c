#include "birm_arm_float_User.h"
/**
 * @brief 浮点复数打包转化为2个16bit构成的32bit整形数,带步长
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y,
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvtovint16_sf( const float *x, const int sa, const int nx, int *y, const int sb)
{
    if(!x || !y){return birmParamNullError;}
    if(nx<=0 || sa<=0 || sb<=0){return birmParamLengthInvalidError;}

    int i;
    short _siXL, _siXH;
    short *yTemp;
    float re, im;

    yTemp = (short *)y;

	for(i = 0; i < nx; i++)
	{
		if(x[2*i*sa]<-32768)
		{
			re = -32768;
		}
		else if(x[2*i*sa]>32767)
		{
			re = 32767;
		}
		else
		{
			re = x[2*i*sa];
		}

		if(x[2*i*sa+1]<-32768)
		{
			im = -32768;
		}
		else if(x[2*i*sa+1]>32767)
		{
			im = 32767;
		}
		else
		{
			im = x[2*i*sa+1];
		}

		_siXL=(short)re;
		_siXH=(short)im;
		yTemp[2*i*sb] =_siXL;
		yTemp[2*i*sb+1] =_siXH;
	}



    return birmSuccess;
}

