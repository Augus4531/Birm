#include "birm_arm_float_User.h"
#include <stdint.h>
#include <string.h> // 用于 memcpy, 编译器会优化为寄存器操作



// 针对 Phytium S5000C (ARMv8) 的预取参数
// 步长较大时预取距离要适当增加，这里设为约 32 个元素后
#define PREFETCH_DIST 32 

int birm_cvmov_sf(const float *x, const int sa, const int nx, float *y, const int sb)
{
    // 1. 基础检查
    if (!x || !y) return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0) return birmParamLengthInvalidError;

    // 2. 指针转为 char* 以便进行字节级精细控制
    const char *src = (const char *)x;
    char *dst = (char *)y;

    // 3. 计算字节步长 (1个复数 = 2个 float = 8 bytes)
    int64_t src_step = (int64_t)sa * 8;
    int64_t dst_step = (int64_t)sb * 8;

    int i = 0;

    // ============================================================
    // 大数据量优化路径：循环展开 + 预取 + 批量偏移计算
    // ============================================================
    // 阈值设为 16，保证展开带来的收益大于初始化开销
    if (nx >= 16) {
        // 预计算 2倍、3倍、4倍 步长，避免循环内重复乘法或加法
        int64_t src_step_2 = src_step * 2;
        int64_t src_step_3 = src_step * 3;
        int64_t src_step_4 = src_step * 4;

        int64_t dst_step_2 = dst_step * 2;
        int64_t dst_step_3 = dst_step * 3;
        int64_t dst_step_4 = dst_step * 4;

        // 主循环：每次处理 4 个元素
        for (; i <= nx - 4; i += 4) {
            // [关键优化 1] 软件预取
            // 告诉 CPU 提前抓取未来的数据到 L1 缓存
            __builtin_prefetch(src + src_step * PREFETCH_DIST, 0, 3); // Read prefetch
            __builtin_prefetch(dst + dst_step * PREFETCH_DIST, 1, 3); // Write prefetch

            uint64_t v0, v1, v2, v3;

            // [关键优化 2] 64位加载 (代替2次32位浮点加载)
            // 使用 memcpy 避免 Strict Aliasing 问题，编译器会在 -O3 下优化为 LDR 指令
            // 这里利用了 base + offset 寻址，指令之间无依赖，可并行发射
            memcpy(&v0, src, 8);
            memcpy(&v1, src + src_step, 8);
            memcpy(&v2, src + src_step_2, 8);
            memcpy(&v3, src + src_step_3, 8);

            // [关键优化 3] 64位存储
            memcpy(dst, &v0, 8);
            memcpy(dst + dst_step, &v1, 8);
            memcpy(dst + dst_step_2, &v2, 8);
            memcpy(dst + dst_step_3, &v3, 8);

            // 更新基地址
            src += src_step_4;
            dst += dst_step_4;
        }
    }

    // ============================================================
    // 小数据量 / 尾部处理路径
    // ============================================================
    // 对于极小数据 (nx < 16)，直接进入这里，没有初始化开销
    // 依然保持 64位 (8字节) 搬运的优势
    for (; i < nx; i++) {
        uint64_t v;
        memcpy(&v, src, 8);
        memcpy(dst, &v, 8);
        src += src_step;
        dst += dst_step;
    }

    return birmSuccess;
}
