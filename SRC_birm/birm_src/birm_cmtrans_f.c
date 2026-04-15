#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度复数矩阵转置
 * @param[in] x - 输入复数矢量指针
 * @param[in] nn - 矩阵行数目
 * @param[in] mm - 矩阵列数目
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cmtrans_f(const float *x, const int nn, const int mm, float *y)
{
	if(!x || !y){return birmParamNullError;}
	if(nn<=0 || mm<=0){return birmParamLengthInvalidError;}

	armpl_int_t i;
	armpl_int_t incy = 1;
	for(i=0; i<mm; i++)
	{
		armpl_singlecomplex_t *_x = (armpl_singlecomplex_t*)&x[2*i];
		armpl_singlecomplex_t *_y = (armpl_singlecomplex_t*)&y[2*i*nn];
		ccopy_((armpl_int_t*)&nn, _x, (armpl_int_t*)&mm, (armpl_singlecomplex_t*)_y, &incy);
	}
	return birmSuccess;
}



