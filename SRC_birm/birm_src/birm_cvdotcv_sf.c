#include "birm_arm_float_User.h"
#include "armpl.h"

/**
 * @brief 浮点单精度复数矢量与复数矢量的点乘,带步进
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] sa - 输入x1的步长
 * @param[in] x2 - 输入复数矢量指针 x2
 * @param[in] sb - 输入x2的步长
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */

int birm_cvdotcv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    armpl_singlecomplex_t *_y = (armpl_singlecomplex_t *)y;

    _y[0] = cdotu_(&nx, (armpl_singlecomplex_t *)x1, &sa, (armpl_singlecomplex_t *)x2, &sb);

    return birmSuccess;
}