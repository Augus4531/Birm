#include "birm_arm_float_User.h"

/**
 * @brief 复数矩阵截取转置函数
 * 
 * @param x 输入复数矩阵指针 (行主序，每个复数包含实部和虚部)
 * @param nn 矩阵x的行数
 * @param mm 矩阵x的列数
 * @param sa 输出矩阵y的行数
 * @param sb 输出矩阵y的列数
 * @param y 计算结果输出矩阵指针
 * @return int 0表示成功，负数表示错误
 */
 
 
int birm_cmtrans_small(const float *x, int nn, int mm, int sa, int sb, float *y)
{
    const float *__restrict x_restrict = x;
    float *__restrict y_restrict = y;
    

    for (int j = 0; j < sb; j++) {
        float* dst_row = &y_restrict[j * sa * 2];
        
        // SIMD 向量化
        #pragma omp simd
        for (int i = 0; i < sa; i++) {
            const int src_idx = (i * mm + j) * 2;
            const int dst_idx = i * 2;
            
            dst_row[dst_idx] = x_restrict[src_idx];
            dst_row[dst_idx + 1] = x_restrict[src_idx + 1];
        }
    }
    return 0;
}


int birm_cmtrans_sf(const float *x, const int nn, const int mm,
                    const int sa, const int sb, float *y)
{
    if (x == NULL || y == NULL) return -1;
    if (nn <= 0 || mm <= 0 || sa <= 0 || sb <= 0 || sa > nn || sb > mm) return -2;
    
    
    // 专门优化64x64、128x128的情况（调用的小函数已改为单核）
    if (nn == 128 && mm == 128 ) {
        return birm_cmtrans_small(x, nn, mm, sa, sb, y);
    }
    if (nn == 64 && mm == 64 ) {
        return birm_cmtrans_small(x, nn, mm, sa, sb, y);
    }
    const int block_size = 64;
    const float *__restrict x_restrict = x;
    float *__restrict y_restrict = y;
    

    for (int jb = 0; jb < sb; jb += block_size) {
        for (int ib = 0; ib < sa; ib += block_size) {
            // 分支预测优化保留
            const int j_end = __builtin_expect((jb + block_size) < sb, 1) ? 
                            (jb + block_size) : sb;
            const int i_end = __builtin_expect((ib + block_size) < sa, 1) ? 
                            (ib + block_size) : sa;
            
            // 循环展开优化保留
            #pragma GCC unroll(4)
            for (int i = ib; i < i_end; i++) {
                const float* src_row = &x_restrict[i * mm * 2];
                
                // 保留 SIMD 向量化和对齐优化（单核下仍有效）
                
                #pragma omp simd safelen(8) aligned(src_row:32) aligned(y_restrict:32)
                
                for (int j = jb; j < j_end; j++) {
                    const int src_idx = j * 2;
                    const int dst_idx = (j * sa + i) * 2;
                    
                    y_restrict[dst_idx] = src_row[src_idx];
                    y_restrict[dst_idx + 1] = src_row[src_idx + 1];
                }
            }
        }
    }
    
    return 0;
}