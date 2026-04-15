#include "birm_arm_float_User.h"

/**
 * @brief 对一个无符号整数向上取2的整数次幂
 * @param[in] x 输入一个无符号整数
 * @param[out] y 输出其对应的向上取2的整数次幂的结果
 *
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_vpow2_f(const int n, int *y)
{
	 if(!y ){return birmParamNullError;}
	 if(n <= 0) {return birmParamLengthInvalidError;}

	 int nAbs = abs(n);
	 int k = 1;
	 if(nAbs < 2)
	 {
		 k=0;
	 }
	 else
	 {
		 nAbs = nAbs - 1;
		 while(nAbs >>= 1)
		 {
			 k++;
		 }
	 }
	 *y = k;
	 return birmSuccess;
}
