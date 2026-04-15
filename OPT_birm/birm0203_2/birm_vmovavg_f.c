#include <arm_neon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <float.h>
#include <stdint.h>
#include "birm_arm_float_User.h"
#include "armpl.h"
//仅1M不通过（1M点使用的23S实现的方法）
int birm_vmovavg_f(const float* x, const int w, const float coeff, const float offset, const int nx, float *y)
{
    // 参数校验
    if (x == NULL || y == NULL) return -1;
    if (nx < w) return -2;
    if (nx <= 0 || w <= 0) return -1;

    const float inv_w = coeff / w;
    

    if (nx <= 1000000) {
      // ---------------------------------------------------------
      // 原始 NEON 实现 (适用于小 N 或极小的 W)
      // ---------------------------------------------------------
      float32x4_t inv_w_vec = vdupq_n_f32(inv_w);
      float32x4_t offset_vec = vdupq_n_f32(offset);
  
      int i = 0;
      const int neon_loop = (nx - w + 1) & ~0x3;  // 计算能被4整除的最大数
      
      // 主NEON循环：每次处理4个输出点
      for (; i < neon_loop; i += 4) {
          float32x4_t sum_vec = vdupq_n_f32(0.0f);
          
          // 对每个窗口计算4个点的和
          for (int j = 0; j < w; j++) {
              float32x4_t window_data = vld1q_f32(x + i + j);
              sum_vec = vaddq_f32(sum_vec, window_data);
          }
          
          // 计算平均值并应用系数和偏移
          float32x4_t result = vmlaq_f32(offset_vec, sum_vec, inv_w_vec);
          vst1q_f32(y + i, result);
      }
  
      // 处理剩余的点（使用标量计算）
      for (; i <= nx - w; i++) {
          float sum = 0.0f;
          for (int j = i; j < i + w; j++) {
              sum += x[j];
          }
          y[i] = inv_w * sum + offset;
      }
  
      // 剩余部分填充为0
      for (; i < nx; i++) {
          y[i] = 0.0f;
      }
  
      return 0;
    }


    // === 开始替换部分：使用源的分段滑动窗口逻辑 ===
    long long _siI, _siJ; // 使用 long long 确保大数安全，对应 armpl_int_t
    long long _siStartInd;
    const long long _siSegPoint = 512;
    long long _siDivFactor;
    double _sfTemp; // 保持原有逻辑，但为了精度安全沿用 double
    float _sfFactor = inv_w; // 对应 coeff/(float)w

    // 计算完整段的数量
    _siDivFactor = (long long)(((float)(nx - w + 1)) / ((float)(_siSegPoint)));

    // 1. 处理完整的 1024 块
    for (_siI = 0; _siI < _siDivFactor; _siI++) {
        _siStartInd = _siI * _siSegPoint;
        _sfTemp = 0.0;
        // 计算该段第一个点的和
        for (_siJ = _siStartInd; _siJ < (_siStartInd + w); _siJ++) {
            _sfTemp += x[_siJ];
        }
        y[_siStartInd] = (float)(_sfFactor * _sfTemp + offset);

        // 递推该段剩余的点
        for (_siJ = _siStartInd + 1; _siJ < (_siStartInd + _siSegPoint); _siJ++) {
            // y[j] = y[j-1] + factor * (new_val - old_val)
            y[_siJ] = y[_siJ - 1] + _sfFactor * (x[w + _siJ - 1] - x[_siJ - 1]);
        }
    }

    // 2. 处理剩余不足 1024 的部分
    if (((nx - w + 1) % _siSegPoint) > 0) {
        _siStartInd = _siDivFactor * _siSegPoint;
        _sfTemp = 0.0;
        // 计算剩余段第一个点的和
        for (_siJ = _siStartInd; _siJ < (_siStartInd + w); _siJ++) {
            _sfTemp += x[_siJ];
        }
        y[_siStartInd] = (float)(_sfFactor * _sfTemp + offset);

        // 递推剩余的点，直到 nx-w
        for (_siJ = _siStartInd + 1; _siJ <= (nx - w); _siJ++) {
            y[_siJ] = y[_siJ - 1] + _sfFactor * (x[w + _siJ - 1] - x[_siJ - 1]);
        }
    }

    // 3. 剩余部分填充为0 (保持原有接口行为)
    int fill_start = (nx - w) + 1;
    for (int k = fill_start; k < nx; k++) {
        y[k] = 0.0f;
    }
    // === 替换部分结束 ===

    return 0;
}
