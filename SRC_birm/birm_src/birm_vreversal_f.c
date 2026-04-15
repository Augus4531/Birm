
#include "birm_arm_float_User.h"

/**
 * @brief 实现一维数据的翻转
 * @param[in] x 输入一个无符号整数
 * @param[out] y 输出其对应的向上取2的整数次幂的结果
 *
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_vreversal_f(const float *x, const int nx, float *y)
{
	 if(!y || !x ){return birmParamNullError;}
	 if(nx <= 0) {return birmParamLengthInvalidError;}

	 int i = 0;
	 int nxHalf = nx / 2;
	 int nxHalfFlag = nx % 2;

	 if(nxHalfFlag == 0)
	 {
		 for(i=0; i< nxHalf; i++)
		 {
			 y[i] = x[nx-i-1];
			 y[i+ nxHalf] = x[nxHalf-1 -i];
		 }
	 }
	 else
	 {
		 y[nxHalf] = x[nxHalf];
		 for(i=0; i< nxHalf; i++)
		 {
			 y[i] = x[nx-i-1];
			 y[i+ nxHalf + 1] = x[nxHalf-1 -i];
		 }
	 }
	 return birmSuccess;
}
