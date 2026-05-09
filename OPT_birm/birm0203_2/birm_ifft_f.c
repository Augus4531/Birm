#include <fftw3.h>
#include <stddef.h>

/**
 * @brief 浮点复数向量 IFFT
 * * @param x         输入数据缓冲区指针 (复数，格式：[real, imag, real, imag...])
 * @param fftN      FFT 点数
 * @param ffthandle FFTW 句柄 (fftwf_plan)
 * @param y         输出数据缓冲区指针 (复数，格式：[real, imag, real, imag...])
 * @return int      0: 成功, -1: 指针为空, -2: 点数错误
 */
int birm_ifft_f(const float *x, const int fftN, fftwf_plan ffthandle, float *y)
{
    // 1. 参数校验
    // 输入指针 x 或输出指针 y 为空：异常返回 -1
    if (x == NULL || y == NULL)
    {
        return birmParamNullError;
    }

    // 输入 FFT 点数小于等于 0：返回 -2
    if (fftN <= 0)
    {
        return birmParamLengthInvalidError;
    }

    // 2. 执行 FFTW 逆变换
    // 注意：FFTW 的 float 复数类型 fftwf_complex 本质上是 float[2]，
    // 在内存布局上与 float * (real, imag 交替) 是兼容的。
    // 使用 fftwf_execute_dft 允许我们利用预先生成的 plan 来处理传入的 x 和 y 缓冲区。
    // 这里的 plan 必须是在外部以 FFTW_BACKWARD 方向创建的。
    fftwf_execute_dft(ffthandle, (fftwf_complex *)x, (fftwf_complex *)y);

    // 3. 归一化处理 (Scaling)
    // 根据图片公式：x(n) = (1/N) * sum(...)
    // FFTW 默认不进行 1/N 处理，需要手动完成。
    // 为了效率，先计算倒数，将其转为乘法操作。
    float scale = 1.0f / (float)fftN;
    int total_elements = fftN * 2; // 复数数组的总 float 数量

    // 循环展开或 SIMD 优化通常由现代编译器 (-O2/-O3) 自动处理
    // 只要代码结构清晰，编译器会自动使用 AVX/NEON 指令进行向量化加速
    for (int i = 0; i < total_elements; ++i)
    {
        y[i] *= scale;
    }

    // 正常：返回 0
    return birmSuccess;
}
