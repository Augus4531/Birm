#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 一维数据循环移位
 * @param[in] x - 输入数据指针
 * @param[in] nx - 一维数据长度
 * @param[in] shift - 循环移位长度 shift>0 右移 shift<0 左移
 * @param[out] y - 输出数据指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vshift_f(const float *x, const int nx, const int shift, float *y)
{
	if(!x || !y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}

	armpl_int_t incy = 1;
	armpl_int_t incx = 1;
	armpl_int_t shiftLen;
	armpl_int_t shift1;

	if(shift>0)
	{
		shift1 = shift%nx;
		shiftLen = nx - shift1;
		float *_x = (float*)&x[0];
		float *_y = (float*)&y[shift1];
		scopy_(&shiftLen, _x, &incx, _y, &incy);
		_x = (float*)&x[shiftLen];
		_y = (float*)&y[0];
		scopy_(&shift1, _x, &incx, _y, &incy);
	}
	else if(shift == 0)
	{
		float *_x = (float*)&x[0];
		float *_y = (float*)&y[0];
		scopy_((armpl_int_t*)&nx, _x, &incx, _y, &incy);
	}
	else
	{
		shift1 = (-shift)%nx;
		shiftLen = nx - shift1;
		float *_x = (float*)&x[shift1];
		float *_y = (float*)&y[0];
		scopy_(&shiftLen, _x, &incx, _y, &incy);
		_x = (float*)&x[0];
		_y = (float*)&y[shiftLen];
		scopy_(&shift1, _x, &incx, _y, &incy);
	}

	return birmSuccess;
}



