
#include "birm_arm_float_User.h"
#include <arm_neon.h>

// 辅助内联函数：将分散的4个int加载到一个向量
static inline int32x4_t load_gather_4(const int *src, int stride, int s2, int s3)
{
    int32x4_t v = vdupq_n_s32(0);
    v = vld1q_lane_s32(src, v, 0);
    v = vld1q_lane_s32(src + stride, v, 1);
    v = vld1q_lane_s32(src + s2, v, 2);
    v = vld1q_lane_s32(src + s3, v, 3);
    return v;
}

// 辅助内联函数：处理1个向量的计算并存储回分散地址
static inline void process_and_scatter_4(int32x4_t vec_src, float *dst, int dst_stride, int ds2, int ds3)
{
    // 1. 数据解包 (Unzip)：将 [L0 H0 L1 H1 L2 H2 L3 H3] 分离
    int16x8_t vec_s16 = vreinterpretq_s16_s32(vec_src);

    int16x4_t low_parts = vuzp1_s16(vget_low_s16(vec_s16), vget_high_s16(vec_s16));
    int16x4_t high_parts = vuzp2_s16(vget_low_s16(vec_s16), vget_high_s16(vec_s16));

    // 2. 转 float
    // f_low  现在包含: [L0, L1, L2, L3] (都是实部)
    // f_high 现在包含: [H0, H1, H2, H3] (都是虚部)
    float32x4_t f_low = vcvtq_f32_s32(vmovl_s16(low_parts));
    float32x4_t f_high = vcvtq_f32_s32(vmovl_s16(high_parts));

    // 3. 交叉重组 (Interleave) - 修复编译错误并提升性能
    // 使用 zip 指令将实部和虚部交叉在一起

    // res01 变成: [L0, H0, L1, H1]
    float32x4_t res01 = vzip1q_f32(f_low, f_high);

    // res23 变成: [L2, H2, L3, H3]
    float32x4_t res23 = vzip2q_f32(f_low, f_high);

    // 4. 存储 (Scatter)
    // vst1_f32 需要 float32x2_t (64位向量，包含2个float)

    // 存储 Point 0: 取 res01 的低半部 [L0, H0]
    vst1_f32(dst, vget_low_f32(res01));

    // 存储 Point 1: 取 res01 的高半部 [L1, H1]
    vst1_f32(dst + dst_stride, vget_high_f32(res01));

    // 存储 Point 2: 取 res23 的低半部 [L2, H2]
    vst1_f32(dst + ds2, vget_low_f32(res23));

    // 存储 Point 3: 取 res23 的高半部 [L3, H3]
    vst1_f32(dst + ds3, vget_high_f32(res23));
}
int birm_vint16tocv_sf(const int *x, const int sa, const int nx, float *y, const int sb)
{
    if (!x || !y)
        return birmParamNullError;
    if (nx <= 0 || sa <= 0 || sb <= 0)
        return birmParamLengthInvalidError;

    const int *src = x;
    float *dst = y;

    // --- 连续内存优化 (保持不变，为节省篇幅略去细节) ---
    if (sa == 1 && sb == 1)
    {
        int i = 0;
        const int16_t *src_ptr = (const int16_t *)src;
        // 请保持之前修复过bug的连续内存代码
        // ...
        // 占位：实际请填入之前验证过的代码
        for (; i < nx; i++)
        {
            int xTmp = src[i];
            dst[2 * i] = (float)((short)(xTmp & 0xFFFF));
            dst[2 * i + 1] = (float)((short)((xTmp >> 16) & 0xFFFF));
        }
        return birmSuccess;
    }

    // --- 非连续内存优化 (多寄存器并行版) ---
    else
    {
        const int src_stride = sa;
        const int dst_stride = sb * 2; // float pair

        // 预计算源步长偏移
        const int sa_2 = src_stride * 2;
        const int sa_3 = src_stride * 3;
        const int sa_4 = src_stride * 4; // 单组跨度

        // 预计算目标步长偏移
        const int ds_2 = dst_stride * 2;
        const int ds_3 = dst_stride * 3;
        const int ds_4 = dst_stride * 4; // 单组跨度

        int i = 0;

        // 16路循环展开 (一次处理4组，每组4个点)
        // 需要处理 src[i] 到 src[i+15]
        for (; i <= nx - 16; i += 16)
        {
            const int *p_src = src;
            float *p_dst = dst;

            // --- 阶段 1: 并行加载 (Load Gather) ---
            // 我们同时发起 4 组 (共16个int) 的加载请求
            // 此时 CPU 的 LSU (Load/Store Unit) 会被填满

            // Group 0 (0-3)
            int32x4_t q0 = load_gather_4(p_src, src_stride, sa_2, sa_3);
            p_src += sa_4;

            // Group 1 (4-7)
            int32x4_t q1 = load_gather_4(p_src, src_stride, sa_2, sa_3);
            p_src += sa_4;

            // Group 2 (8-11)
            int32x4_t q2 = load_gather_4(p_src, src_stride, sa_2, sa_3);
            p_src += sa_4;

            // Group 3 (12-15)
            int32x4_t q3 = load_gather_4(p_src, src_stride, sa_2, sa_3);

            // --- 阶段 2: 计算与存储 (Process & Scatter) ---
            // 处理 Group 0
            process_and_scatter_4(q0, p_dst, dst_stride, ds_2, ds_3);
            p_dst += ds_4;

            // 处理 Group 1
            process_and_scatter_4(q1, p_dst, dst_stride, ds_2, ds_3);
            p_dst += ds_4;

            // 处理 Group 2
            process_and_scatter_4(q2, p_dst, dst_stride, ds_2, ds_3);
            p_dst += ds_4;

            // 处理 Group 3
            process_and_scatter_4(q3, p_dst, dst_stride, ds_2, ds_3);

            // 更新主指针 (一次跳过16个点)
            src += sa_4 * 4;
            dst += ds_4 * 4;
        }

        // --- 处理剩余部分 (回退到4路处理) ---
        for (; i <= nx - 4; i += 4)
        {
            int32x4_t q = load_gather_4(src, src_stride, sa_2, sa_3);
            process_and_scatter_4(q, dst, dst_stride, ds_2, ds_3);
            src += sa_4;
            dst += ds_4;
        }

        // --- 处理最后残余 (标量) ---
        for (; i < nx; i++)
        {
            int xTmp = *src;
            dst[0] = (float)((short)(xTmp & 0xFFFF));
            dst[1] = (float)((short)((xTmp >> 16) & 0xFFFF));
            src += src_stride;
            dst += dst_stride;
        }
    }

    return birmSuccess;
}
