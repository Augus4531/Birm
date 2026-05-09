
#include "birm_arm_float_User.h"

/**
 * @brief 浮点单精度实数矢量最小值及索引值
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] id - 输出最小位所在指针
 * @param[out] y - 输出实数标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmini_f(const float *x, const int nx,int *id, float *y)
{
	int i=0,_index=0;
	float _temp=0;
	if(!x||!id||!y){return birmParamNullError;}
	if(nx<=0){return birmParamLengthInvalidError;}
	_temp=x[0];
	_index=0;
	for(i=0;i<nx;i++)
	{
		if(x[i]<_temp)
		{
			_temp=x[i];
			_index=i;
		}
	}
	*y=_temp;
	*id=_index;
	return birmSuccess;
}

