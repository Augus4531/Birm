#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度实数矩阵转置
 * @param[in] x - 输入实数矢量指针
 * @param[in] nn - 矩阵行数目
 * @param[in] mm - 矩阵列数目
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_mtrans_f(const float *x, const int nn, const int mm, float *y)
{
	if(!x || !y){return birmParamNullError;}
	if(nn<=0 || mm<=0){return birmParamLengthInvalidError;}

	armpl_int_t i;
	armpl_int_t incy = 1;
	for(i=0; i<mm; i++)
	{
		float *_x = (float*)&x[i];
		float *_y = (float*)&y[i*nn];
		scopy_((armpl_int_t*)&nn, _x, (armpl_int_t*)&mm, _y, &incy);
	}

	return birmSuccess;
}



