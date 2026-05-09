#include "birm_arm_float_User.h"

/**
 * @brief 实数矢量求滑动平均
 * @param[in] x 输入矢量指针
 * @param[in] w 输入滑动窗长度
 * @param[in] coeff 输入门限系数
 * @param[in] offset 输入直流漂移
 * @param[in] nx 输入矢量长度
 * @param[out] y 输出矢量指针
 *
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmovavg_f(const float* x, const int w, const float coeff, const float offset, const int nx, float *y)
{
    if(!x || !y ){return birmParamNullError;}
    if(nx < w){return birmParamLengthInvalidError;}

    int _siI, _siJ;
    int _siStartInd;
    const int _siSegPoint = 1024;
    int _siDivFactor;
    float _sfTemp;
    float _sfFactor = coeff / (float)w;

    _siDivFactor = (nx - w + 1) / _siSegPoint;

    for(_siI = 0; _siI < _siDivFactor; _siI++)
    {
        _siStartInd = _siI * _siSegPoint;
        _sfTemp = 0.0f;
        
        // 计算当前段第一个滑动窗的累加和
        for(_siJ = _siStartInd; _siJ < (_siStartInd + w); _siJ++)
        {
            _sfTemp += x[_siJ];
        }
        y[_siStartInd] = _sfFactor * _sfTemp + offset;

        // 利用递推公式计算当前段后续的滑动平均值
        for(_siJ = _siStartInd + 1; _siJ < (_siStartInd + _siSegPoint); _siJ++)
        {
            y[_siJ] = y[_siJ - 1] + _sfFactor * (x[w + _siJ - 1] - x[_siJ - 1]);
        }
    }
    
    // 处理剩余尾部数据
    if(((nx - w + 1) % _siSegPoint) > 0)
    {
        _siStartInd = _siDivFactor * _siSegPoint;
        _sfTemp = 0.0f;
        for(_siJ = _siStartInd; _siJ < (_siStartInd + w); _siJ++)
        {
            _sfTemp += x[_siJ];
        }
        y[_siStartInd] = _sfFactor * _sfTemp + offset;
        
        for(_siJ = _siStartInd + 1; _siJ <= (nx - w); _siJ++)
        {
            y[_siJ] = y[_siJ - 1] + _sfFactor * (x[w + _siJ - 1] - x[_siJ - 1]);
        }
    }

    return birmSuccess;
}
