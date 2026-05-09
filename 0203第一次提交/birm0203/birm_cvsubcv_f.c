#include "birm_arm_float_User.h"
// 512k之前较快，best
/**
 * @brief 复数矢量与复数矢量的减法
 */
int birm_cvsubcv_f(const float *x1, const float *x2, const int nx, float *y) {
    if (x1 == NULL || x2 == NULL || y == NULL) return -1;
    if (nx <= 0) return -2;

    const float *px1 = x1;
    const float *px2 = x2;
    float *py = y;

    // 针对4k（4096）及以上数据量的优化路径（缓存友好型块处理）
    if (nx >= 65536) {
        // 块大小设置为256个复数（512字节），匹配飞腾L1缓存
        const int block_size = 512; 
        int total_blocks = nx / block_size;
        int remaining = nx % block_size;

        // 块循环：编译器自动矢量化友好
        for (int b = 0; b < total_blocks; b++) {
            // 对每个块进行显式展开（4次），帮助编译器优化
            for (int i = 0; i < block_size; i += 4) {
                // 显式计算，便于PHYGCC自动生成飞腾向量指令
                py[0] = px1[0] - px2[0];
                py[1] = px1[1] - px2[1];
                py[2] = px1[2] - px2[2];
                py[3] = px1[3] - px2[3];
                py[4] = px1[4] - px2[4];
                py[5] = px1[5] - px2[5];
                py[6] = px1[6] - px2[6];
                py[7] = px1[7] - px2[7];

                px1 += 8;
                px2 += 8;
                py += 8;
            }
        }

        // 处理剩余元素
        for (int i = 0; i < remaining; i++) {
            py[0] = px1[0] - px2[0];
            py[1] = px1[1] - px2[1];
            px1 += 2;
            px2 += 2;
            py += 2;
        }
    } 
    else {
        // 小数据量保持原4次展开逻辑（不影响性能）
        int unroll_count = nx / 4;
        for (int i = 0; i < unroll_count; i++) {
                py[0] = px1[0] - px2[0];
                py[1] = px1[1] - px2[1];
                py[2] = px1[2] - px2[2];
                py[3] = px1[3] - px2[3];
                py[4] = px1[4] - px2[4];
                py[5] = px1[5] - px2[5];
                py[6] = px1[6] - px2[6];
                py[7] = px1[7] - px2[7];

            px1 += 8;
            px2 += 8;
            py += 8;
        }

        int remaining = nx % 4;
        for (int i = 0; i < remaining; i++) {
            py[0] = px1[0] - px2[0];
            py[1] = px1[1] - px2[1];
            px1 += 2;
            px2 += 2;
            py += 2;
        }
    }

    return 0;
}