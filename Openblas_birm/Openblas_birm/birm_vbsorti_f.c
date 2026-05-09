#include "birm_arm_float_User.h"
//#include "armpl.h"

/**
 * @brief 浮点实数矢量的冒泡排序
 * @param[in] x1 - 输入实数矢量指针 x
 * @param[in] x2 - 输出位置矢量k，可以为NULL
 * @param[in] nx - 输入复数矢量长度
 * @param[in] f -  输入排序方式 0：从大到小 1：从小到大
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vbsorti_f(float *x, int *k, const int nx, const int f)
{
	if (!x)
	{
		return birmParamNullError;
	}
	if (nx <= 0)
	{
		return birmParamLengthInvalidError;
	}
	if ((f != 0) && (f != 1))
	{
		return birmParamOtherError;
	}

	int i, j;
	int index;
	float amp;

	switch (f)
	{
	case 0: // 从大到小
	{
		if (k != NULL) // 包含序号
		{
			for (i = 0; i < nx; i++)
			{
				k[i] = i;
			}

			for (i = 0; i < (nx - 1); i++)
			{
				for (j = (i + 1); j < nx; j++)
				{
					if (x[j] > x[i])
					{
						amp = x[i];
						x[i] = x[j];
						x[j] = amp;
						index = k[i];
						k[i] = k[j];
						k[j] = index;
					}
				}
			}
		}
		else
		{
			for (i = 0; i < (nx - 1); i++)
			{
				for (j = (i + 1); j < nx; j++)
				{
					if (x[j] > x[i])
					{
						amp = x[i];
						x[i] = x[j];
						x[j] = amp;
					}
				}
			}
		}
	}
	break;
	default: // 从小到大
	{
		if (k != NULL) // 包含序号
		{
			for (i = 0; i < nx; i++)
			{
				k[i] = i;
			}

			for (i = 0; i < (nx - 1); i++)
			{
				for (j = (i + 1); j < nx; j++)
				{
					if (x[j] < x[i])
					{
						amp = x[i];
						x[i] = x[j];
						x[j] = amp;
						index = k[i];
						k[i] = k[j];
						k[j] = index;
					}
				}
			}
		}
		else
		{
			for (i = 0; i < (nx - 1); i++)
			{
				for (j = (i + 1); j < nx; j++)
				{
					if (x[j] < x[i])
					{
						amp = x[i];
						x[i] = x[j];
						x[j] = amp;
					}
				}
			}
		}
	}
	break;
	}

	return birmSuccess;
}
