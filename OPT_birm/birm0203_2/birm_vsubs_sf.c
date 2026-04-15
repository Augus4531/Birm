/**
 * @brief 浮点单精度实数矢量与单精度实数标量的减法，带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输入y步长
 * @return 成功返回0，错误返回负数
 */

#include "birm_arm_float_User.h"
#include <arm_neon.h>

int birm_vsubs_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb)
{
    if (!x1 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    int i = 0;

    // ---------------------------------------------------------
    // Case 1: 全连续内存访问 (sa=1, sb=1)
    // ---------------------------------------------------------
    if (sa == 1 && sb == 1)
    {
        float32x4_t v_x2 = vdupq_n_f32(x2);

        // 32路循环展开：一次处理 32 个 float (8个 NEON 寄存器)
        for (; i <= nx - 32; i += 32)
        {
            // 预取策略：
            // 读取流 x1: 预取到当前位置 + 32 (128字节后)，hint=0 (非时效性数据)
            // 写入流 y : 预取到当前位置 + 32，hint=1 (准备写入)
            __builtin_prefetch(&x1[i + 32], 0, 0);
            __builtin_prefetch(&y[i + 32], 1, 0);

            float32x4_t v1 = vld1q_f32(&x1[i]);
            float32x4_t v2 = vld1q_f32(&x1[i + 4]);
            float32x4_t v3 = vld1q_f32(&x1[i + 8]);
            float32x4_t v4 = vld1q_f32(&x1[i + 12]);
            float32x4_t v5 = vld1q_f32(&x1[i + 16]);
            float32x4_t v6 = vld1q_f32(&x1[i + 20]);
            float32x4_t v7 = vld1q_f32(&x1[i + 24]);
            float32x4_t v8 = vld1q_f32(&x1[i + 28]);

            // 流水线执行减法
            v1 = vsubq_f32(v1, v_x2);
            v2 = vsubq_f32(v2, v_x2);
            v3 = vsubq_f32(v3, v_x2);
            v4 = vsubq_f32(v4, v_x2);
            v5 = vsubq_f32(v5, v_x2);
            v6 = vsubq_f32(v6, v_x2);
            v7 = vsubq_f32(v7, v_x2);
            v8 = vsubq_f32(v8, v_x2);

            vst1q_f32(&y[i], v1);
            vst1q_f32(&y[i + 4], v2);
            vst1q_f32(&y[i + 8], v3);
            vst1q_f32(&y[i + 12], v4);
            vst1q_f32(&y[i + 16], v5);
            vst1q_f32(&y[i + 20], v6);
            vst1q_f32(&y[i + 24], v7);
            vst1q_f32(&y[i + 28], v8);
        }

        // 16路处理
        for (; i <= nx - 16; i += 16)
        {
            float32x4_t v1 = vld1q_f32(&x1[i]);
            float32x4_t v2 = vld1q_f32(&x1[i + 4]);
            float32x4_t v3 = vld1q_f32(&x1[i + 8]);
            float32x4_t v4 = vld1q_f32(&x1[i + 12]);

            vst1q_f32(&y[i], vsubq_f32(v1, v_x2));
            vst1q_f32(&y[i + 4], vsubq_f32(v2, v_x2));
            vst1q_f32(&y[i + 8], vsubq_f32(v3, v_x2));
            vst1q_f32(&y[i + 12], vsubq_f32(v4, v_x2));
        }

        // 4路处理
        for (; i <= nx - 4; i += 4)
        {
            float32x4_t v = vld1q_f32(&x1[i]);
            vst1q_f32(&y[i], vsubq_f32(v, v_x2));
        }
    }
    // ---------------------------------------------------------
    // Case 2:输入步长为2，输出连续
    // ---------------------------------------------------------
    else if (sa == 2 && sb == 1)
    {
        float32x4_t v_x2 = vdupq_n_f32(x2);

        // 16路循环展开 (处理16个有效元素，实际加载32个float)
        for (; i <= nx - 16; i += 16)
        {
            __builtin_prefetch(&x1[(i + 16) * 2], 0, 0);

            // 使用 vld2q 加载并分离奇偶位置数据
            float32x4x2_t d1 = vld2q_f32(&x1[i * 2]);
            float32x4x2_t d2 = vld2q_f32(&x1[(i + 4) * 2]);
            float32x4x2_t d3 = vld2q_f32(&x1[(i + 8) * 2]);
            float32x4x2_t d4 = vld2q_f32(&x1[(i + 12) * 2]);

            // 只计算 val[0] (偶数位数据)，忽略 val[1] (奇数位数据)
            vst1q_f32(&y[i], vsubq_f32(d1.val[0], v_x2));
            vst1q_f32(&y[i + 4], vsubq_f32(d2.val[0], v_x2));
            vst1q_f32(&y[i + 8], vsubq_f32(d3.val[0], v_x2));
            vst1q_f32(&y[i + 12], vsubq_f32(d4.val[0], v_x2));
        }

        // 4路处理
        for (; i <= nx - 4; i += 4)
        {
            float32x4x2_t d = vld2q_f32(&x1[i * 2]);
            vst1q_f32(&y[i], vsubq_f32(d.val[0], v_x2));
        }
    }
    // ---------------------------------------------------------
    // Case 3: 复数模式 (sa=2, sb=2)
    // ---------------------------------------------------------
    else if (sa == 2 && sb == 2)
    {
        if (nx >= 262144)
        {
            float32x4_t v_x2 = vdupq_n_f32(x2);

            // 8路循环展开
            for (; i <= nx - 8; i += 8)
            {
                __builtin_prefetch(&x1[(i + 16) * 2], 0, 0);

                // 高效加载：利用 vld2q 一次读取连续内存
                float32x4x2_t d1 = vld2q_f32(&x1[i * 2]); // 加载 8 个 float
                float32x4x2_t d2 = vld2q_f32(&x1[(i + 4) * 2]);

                // 向量计算
                float32x4_t res1 = vsubq_f32(d1.val[0], v_x2);
                float32x4_t res2 = vsubq_f32(d2.val[0], v_x2);

                // 离散存储：手动解包写入，跳过间隔
                // 这种写法比纯标量循环快，因为减少了加载指令和计算指令的数量

                // 处理第一组 res1
                vst1q_lane_f32(&y[(i + 0) * 2], res1, 0);
                vst1q_lane_f32(&y[(i + 1) * 2], res1, 1);
                vst1q_lane_f32(&y[(i + 2) * 2], res1, 2);
                vst1q_lane_f32(&y[(i + 3) * 2], res1, 3);

                // 处理第二组 res2
                vst1q_lane_f32(&y[(i + 4) * 2], res2, 0);
                vst1q_lane_f32(&y[(i + 5) * 2], res2, 1);
                vst1q_lane_f32(&y[(i + 6) * 2], res2, 2);
                vst1q_lane_f32(&y[(i + 7) * 2], res2, 3);
            }
        }
        else
        {
            // 预计算索引偏移，避免乘法指令
            int sa_2 = sa * 2;
            int sa_3 = sa * 3;
            int sb_2 = sb * 2;
            int sb_3 = sb * 3;

            int step_in = sa * 4;
            int step_out = sb * 4;

            int idx_in = 0;
            int idx_out = 0;

            // 4路标量展开
            // 为什么不用 NEON? 因为 sb>1 时，NEON 无法高效地做 Scatter Store (离散写)。
            // 强行用 vector store 会破坏输出数组的“间隙”数据。
            // 标量多路展开利用了 CPU 的多 ALU 特性，是此场景下的最优解。
            for (; i <= nx - 4; i += 4)
            {
                float v0 = x1[idx_in];
                float v1 = x1[idx_in + sa];
                float v2 = x1[idx_in + sa_2];
                float v3 = x1[idx_in + sa_3];

                y[idx_out] = v0 - x2;
                y[idx_out + sb] = v1 - x2;
                y[idx_out + sb_2] = v2 - x2;
                y[idx_out + sb_3] = v3 - x2;

                idx_in += step_in;
                idx_out += step_out;
            }

            // 剩余部分由最后的尾部循环处理，
            // 这里无需更新 i, 因为循环条件已经保证 i 停在剩余部分之前
            // 但需要注意，下面的尾部循环是基于 i 的乘法计算索引的，所以这里逻辑是自洽的。
        }

        // 尾部处理交给最后的标量循环
    }

    // ---------------------------------------------------------
    // Case 3: 通用步长或输出步长 > 1
    // ---------------------------------------------------------
    else
    {
        // 预计算索引偏移，避免乘法指令
        int sa_2 = sa * 2;
        int sa_3 = sa * 3;
        int sb_2 = sb * 2;
        int sb_3 = sb * 3;

        int step_in = sa * 4;
        int step_out = sb * 4;

        int idx_in = 0;
        int idx_out = 0;

        for (; i <= nx - 4; i += 4)
        {
            float v0 = x1[idx_in];
            float v1 = x1[idx_in + sa];
            float v2 = x1[idx_in + sa_2];
            float v3 = x1[idx_in + sa_3];

            y[idx_out] = v0 - x2;
            y[idx_out + sb] = v1 - x2;
            y[idx_out + sb_2] = v2 - x2;
            y[idx_out + sb_3] = v3 - x2;

            idx_in += step_in;
            idx_out += step_out;
        }
    }

    // 尾部循环：处理剩余不足 4 个的元素
    for (; i < nx; i++)
    {
        y[i * sb] = x1[i * sa] - x2;
    }

    return birmSuccess;
}