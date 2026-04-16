#ifndef BIRM_ARM_FLOAT_USER_H_
#define BIRM_ARM_FLOAT_USER_H_
#include <stdlib.h>
//#include <fftw3.h>
/* 版本信息 */
#define BIRM_S5000C_LIB_VERSION_NO		(0x00030A01)	// v3.a.01版本
#define BIRM_S5000C_LIB_DATE			(0x20251023)	// 库生成日期，(20251023：添加birm_vmulv_sf函数)

typedef enum
{
	birmSuccess = 0,
	birmParamNullError = -1,
	birmParamLengthInvalidError = -2,
	birmParamOtherError = -3,
	birmAPLReturnError = -4,
}BirmError;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief 浮点单精度实数矢量以10为底的对数
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_v10log_f(const float *x,const  int nx, float *y);	//0

/**
 * @brief 浮点单精度实数矢量的正弦值
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsin_f(const float *x, const int nx, float *y);	//1

/**
 * @brief 浮点单精度实数矢量的正弦值
 * @param[in] x - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vcos_f(const float *x, const int nx, float *y);	//2

/**
 * @brief 完成32位浮点实数矢量的开方
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsqrt_f(const float *x, const int nx, float *y);	//3

/**
 * @brief 实数矢量的复指数运算
 * @param[in] x - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvexp_f(const float *x, const int nx, float *y);	//4

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的加法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vadds_f(const float *x1, const float x2, const int nx, float *y);//5

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的加法，带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长， 如果sa为负数，则先将x1指针偏移到末尾，然后再计算
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输入y步长，如果sb为负数，则先将y指针偏移到末尾，然后再计算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vadds_sf(const float *x1,const int sa,const float x2,const int nx,float *y,const int sb);//6

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的减法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsubs_f(const float *x1, const float x2, const int nx, float *y);//7

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的减法，带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长， 如果sa为负数，则先将x1指针偏移到末尾，然后再计算
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输入y步长，如果sb为负数，则先将y指针偏移到末尾，然后再计算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsubs_sf(const float *x1,const int sa,const float x2,const int nx,float *y,const int sb);//8

/**
 * @brief 浮点单精度实数标量与单精度实数矢量的减法
 * @param[in] x1 - 输入实数标量 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x2与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_ssubv_f(const float x1, const float* x2, const int nx, float *y);//9

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的乘法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmuls_f(const float *x1, const float x2, const int nx, float *y);//10

/**
 * @brief 浮点单精度实数矢量与单精度实数标量的乘法，带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长， 如果sa为负数，则先将x1指针偏移到末尾，然后再计算
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输入y步长，如果sb为负数，则先将y指针偏移到末尾，然后再计算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmuls_sf(const float *x1, const int sa, const float x2, const int nx, float *y, const int sb);//11

/**
 * @brief 浮点单精度实数矢量与实数标量的除法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vdivs_f(const float *x1, const float x2, const int nx, float *y);//12

/**
 * @brief 浮点单精度实数矢量与实数矢量的加法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vaddv_f(const float *x1, const float* x2, const int nx, float *y);//13

/**
 * @brief 浮点单精度实数矢量与实数矢量的减法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsubv_f(const float *x1, const float* x2, const int nx, float *y);//14

/**
 * @brief 浮点单精度实数矢量与实数矢量的乘法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针, x1与y可以同源算, x2与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmulv_f(const float *x1, const float* x2, const int nx, float *y);//15

/**
 * @brief 浮点单精度实数矢量与实数矢量的乘法,带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa1 - 输入x1步长
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] sa2 - 输入x1步长
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @param[in] sb - 输入y步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmulv_sf(const float *x1, const int sa1,const float* x2, const int sa2, const int nx, float *y,const int sb);


/**
 * @brief 浮点单精度实数矢量与实数矢量的除法
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vdivv_f(const float *x1, const float* x2, const int nx, float *y);//16

/**
 * @brief 浮点单精度实数矢量与实数矢量的点乘
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vdotv_f(const float *x1, const float *x2, const int nx, float *y);//17

/**
 * @brief 浮点单精度实数矢量与实数矢量的点乘,带步长
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] sa - 输入x1步长， 如果sa为负数，则先将x1指针偏移到末尾，然后再计算
 * @param[in] x2 - 输入实数标量 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输入y步长，如果sb为负数，则先将y指针偏移到末尾，然后再计算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vdotv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y);//18

/**
 * @brief 复数矢量和复数标量的加法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvaddcs_f(const float *x1, const float *x2, const int nx, float *y);//19

/**
 * @brief 复数矢量和复数标量的减法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvsubcs_f(const float *x1, const float *x2, const int nx, float *y);//20

/**
 * @brief 复数矢量和实数标量的乘法
 * @param[in] x1 - 输入输出复数矢量指针 x1(输入输出同一个指针)
 * @param[in] x2 - 输入复数标量     x2
 * @param[in] nx - 输入复数矢量长度
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmuls_if(float *x1, const float x2, const int nx);//21

/**
 * @brief 复数矢量与复数标量的乘
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulcs_f(const float *x1, const float *x2, const int nx, float *y);//22

/**
 * @brief 复数矢量与复数标量的乘加
 * @param[in] x1 - 输入矢量指针 x1
 * @param[in] x2 - 输入矢量指针 x2
 * @param[in] x3 - 输入矢量指针x3
 * @param[in] nx - 输入矢量长度
 * @param[out] y - 输出矢量指针,x3和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulcsaddcv_f(const float *x1,const float *x2, const float *x3, const int nx, float *y);//23

/**
 * @brief 复数矢量和复数矢量的加法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvaddcv_f(const float *x1, const float *x2, const int nx, float *y);//24

/**
 * @brief 复数矢量和复数矢量的减法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvsubcv_f(const float *x1, const float *x2, const int nx, float *y);//25

/**
 * @brief 复数矢量和复数矢量的乘法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源,x2与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulcv_f(const float *x1, const float *x2, const int nx, float *y);//26

/**
 * @brief 复数矢量和复数矢量的除法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y, x1与y可以同源,x2与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvdivcv_f(const float *x1, const float *x2, const int nx, float *y);//27

/**
 * @brief 复数矢量和复数矢量共轭的乘法
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针y,x1与y可以同源,x2与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmuljv_f(const float *x1, const float *x2, const int nx, float *y);//28

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
int birm_cvdotcv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y);//29

/**
 * @brief 浮点单精度复数矢量与复数矢量的共轭点乘,带步近
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
int birm_cvdotjv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y);//30

/**
 * @brief 浮点单精度复数矢量与复数矢量共轭的点乘
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数矢量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvdotjv_f(const float *x1, const float *x2, const int nx, float *y);//31

/**
 * @brief 浮点单精度复数矢量与复数矢量的点乘
 * @param[in] x1 - 输入复数矢量指针 x1
 * @param[in] x2 - 输入复数矢量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvdotcv_f(const float *x1, const float *x2, const int nx, float *y);//32

/**
 * @brief 浮点单精度复数矩阵转置
 * @param[in] x - 输入复数矢量指针
 * @param[in] nn - 矩阵行数目
 * @param[in] mm - 矩阵列数目
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cmtrans_f(const float *x, const int nn, const int mm, float *y);//33

/**
 * @brief 浮点单精度实数矩阵转置
 * @param[in] x - 输入实数矢量指针
 * @param[in] nn - 矩阵行数目
 * @param[in] mm - 矩阵列数目
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_mtrans_f(const float *x, const int nn, const int mm, float *y);//34

/**
 * @brief 浮点单精度实数矢量求绝对值
 * @param[in] x1 - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vabs_f(const float *x,const int nx, float *y);	//35

/**
 * @brief 浮点单精度实数矢量填充标量数
 * @param[in] x - 输入实数标量数
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vfill_f(const float x,const int nx, float *y);	//36

/**
 * @brief 浮点单精度实数矢量填充标量数,带步长
 * @param[in] x - 输入实数标量数
 * @param[in] nx - 输入实数矢量长度
 * @param[in] sb - 输入步长
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vfill_sf(const float x,const int nx,const int sb, float *y);//37

/**
 * @brief 浮点单精度复数矢量求模
 * @param[in] x - 输入复数矢量指针
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvabs_f(const float *x,const int nx, float *y);//38

/**
 * @brief 浮点单精度复数矢量求模平方
 * @param[in] x - 输入复数矢量指针
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvabss_f(const float *x,const int nx, float *y);//39

/**
 * @brief 浮点单精度复数矢量求共轭
 * @param[in] x - 输入复数矢量指针
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_jv_f(const float *x,const int nx, float *y);	//40

/**
 * @brief 浮点单精度复数矢量实部和虚部互换
 * @param[in] x - 输入复数矢量指针
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出复数矢量指针,x1和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvswap_f(const float *x,const int nx, float *y);//41

/**
 * @brief 两个实数矢量比较
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数矢量指针 x2
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针,x1和y可以同源,x2和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vlg_f(const float *x1,const float *x2,const int nx,float *y);//42

/**
 * @brief 浮点实数矢量进行数据范围限制
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入最大值 x2
 * @param[in] x3 - 输入最小值 x3
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vclip_f(const float *x1, const float x2, const float x3, const int nx, float * y);//43

/**
 * @brief 浮点实数矢量与浮点实数标量的比较，输出较大值
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针,x1和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vthrsh_f(const float *x1, const float x2, const int nx, float *y);//44

/**
 * @brief 浮点实数矢量与浮点实数标量的比较，输出过门限值
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] x2 - 输入实数标量指针 x2
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vthres_f(const float *x1, const float x2, const int nx, float *y);//45

/**
 * @brief 浮点实数矢量的冒泡排序
 * @param[in] x - 输入实数矢量指针 x
 * @param[in] k - 输出位置矢量k，可以为NULL
 * @param[in] nx - 输入复数矢量长度
 * @param[in] f -  输入排序方式 0：从大到小 1：从小到大
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vbsorti_f(float *x, int *k, const int nx, const int f);//46

/**
 * @brief 浮点单精度复数矢量求和
 * @param[in] x  - 输入复数矢量指针 x
 * @param[in] nx - 输入复数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvsum_f(const float *x1, const int nx, float *y);//47

/**
 * @brief 浮点单精度实数矢量求和
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsum_f(const float *x, const int nx, float *y);//48

/**
 * @brief 浮点单精度实数矢量求和,带步长
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] sa - 输入 x 步长
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsum_sf(const float *x,const int sa,const int nx, float *y);//49

/**
 * @brief 浮点单精度实数矢量求平方和
 * @param[in] x1 - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsums_f(const float *x, const int nx, float *y);//50

/**
 * @brief 浮点单精度实数矢量求平方和
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] sa - 输入 x 步长
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsums_sf(const float *x,const int sa,const int nx, float *y);//51

/**
 * @brief 浮点单精度实数矢量绝对值求和
 * @param[in] x - 输入实数矢量指针 x1
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vsumm_f(const float *x, const int nx, float *y);//52

/**
 * @brief 浮点单精度实数矢量求平均
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmean_f(const float *x,const int nx, float *y);//53

/**
 * @brief 浮点单精度实数矢量与实数矢量求最大值
 * @param[in] x1 - 输入实数矢量指针
 * @param[in] x2 - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针,x1和y可以同源,x2和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmaxv_f(const float *x1,const float *x2,const int nx, float *y);//54

/**
 * @brief 浮点单精度实数矢量与实数矢量求最小值
 * @param[in] x1 - 输入实数矢量指针
 * @param[in] x2 - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针,x1和y可以同源,x2和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vminv_f(const float *x1,const float *x2,const int nx, float *y);//55

/**
 * @brief 浮点单精度实数矢量最大值
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmax_f(const float *x, const int nx, float *y);//56

/**
 * @brief 浮点单精度实数矢量最小值
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmin_f(const float *x, const int nx, float *y);//57

/**
 * @brief 浮点单精度实数矢量最小值及索引值
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] id - 输出最小位所在指针
 * @param[out] y - 输出实数标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmini_f(const float *x, const int nx, int *id, float *y);//58

/**
 * @brief 浮点单精度实数矢量最大值及索引值
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] id - 输出最大位所在指针
 * @param[out] y - 输出实数标量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmaxi_f(const float *x, const int nx, int *id, float *y);//59

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
int birm_vmovavg_f(const float* x, const int w, const float coeff, const float offset, const int nx, float *y);//60

/**
 * @brief 浮点单精度实数矢量搬移
 * @param[in] x - 输入实数矢量指针
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmov_f(const float *x1, const int n, float *y);//61

/**
 * @brief 浮点单精度实数矢量带步进搬移
 * @param[in] x - 输入实数矢量指针
 * @param[in] sa - 输入实数矢量x步长
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y
 * @param[in] sb - 输出实数矢量步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vmov_sf(const float *x1, const int sa, const int nx, float *y, const int sb);//62

/**
 * @brief 浮点单精度复数矢量与复数矢量的点乘
 * @param[in] x - 指向输入数据缓冲区x
 * @param[in] sa - 输入步长
 * @param[in] nx - 输入矢量长度
 * @param[out] y - 指向输出数据缓冲区y
 * @param[in] sb - 输出步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmov_sf(const float *x, const int sa, const int n, float *y, const int sb);//63

/**
 * @brief 整数转化为浮点数
 * @param[in] x1 - 输入整数矢量指针
 * @param[in] nx - 输入矢量长度
 * @param[out] y - 输出浮点数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_int2float_f(const int *x1, const int n, float *y);//64

/**
 * @brief 转浮点实数化为整数
 * @param[in] x - 输入浮点数矢量指针
 * @param[in] nx - 输入矢量长度
 * @param[out] y - 输出整数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vfloat2int_f(const float *x1, const int nx, int *y);//65

/**
 * @brief 实现一维数据的翻转
 * @param[in] x 输入矢量指针x
 * @param[in] nx 输入矢量长度
 * @param[in] y 输出标量指针y
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vreversal_f(const float *x, const int nx, float *y);//66

/**
 * @brief 对一个无符号整数向上取2的整数次幂
 * @param[in] n 输入一个无符号整数
 * @param[out] y 输出其对应的向上取2的整数次幂的结果
 *
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vpow2_f(const int n, int *y);//67

/**
 * @brief 复数矢量FFT变换
 * @param[in] x  - 输入复数矢量指针 x
 * @param[in] fftN - FFT点数
 * @param[in] ffthandle - FFT句柄
 * @param[out] y - 输出复数矢量指针 y,y与x可同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
//int birm_fft_f(const float *x, const int fftN, fftwf_plan ffthandle, float *y);//68

/**
 * @brief 复数矢量iFFT变换
 * @param[in] x  - 输入复数矢量指针 x
 * @param[in] fftN - iFFT点数
 * @param[in] ffthandle - FFT句柄
 * @param[out] y - 输出复数矢量指针 y,y与x可同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
//int birm_ifft_f(const float *x, const int fftN, fftwf_plan ffthandle, float *y);//69

/**
 * @brief 把2个16bit数构成的32bit整型数转化为浮点复数
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y,
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vint16tocv_f(const int *x, const int nx, float *y);//70

/**
 * @brief 把2个16bit数构成的32bit整型数转化为浮点复数,带步长
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  sa - 输入矢量指针 x的步长
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y
 * @param[in]  sb - 输出矢量指针y的步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vint16tocv_sf(const int *x, const int sa, const int nx, float *y, const int sb);//71

/**
 * @brief 浮点复数打包转化为2个16bit构成的32bit整形数
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y,
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvtovint16_f(const float *x, const int nx, int *y);//72

/**
 * @brief 浮点复数打包转化为2个16bit构成的32bit整形数,带步长
 * @param[in]  x  - 输入矢量指针 x
 * @param[in]  sa  - 输入复数矢量指针x的步长
 * @param[in]  nx - 输入矢量长度
 * @param[out] y  - 输出矢量指针y,
 * @param[in]  sb  - 输出实数矢量指针y的步长
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvtovint16_sf(const float *x, const int sa, const int nx, int *y, const int sb);//73

/**
 * @brief 浮点单精度实数完成32bit数据压缩为16bit
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vpack16_f(const float *x, const int nx, int *y);//74

/**
 * @brief 浮点单精度实数完成32bit数据压缩为8bit
 * @param[in] x  - 输入实数矢量指针 x
 * @param[in] nx - 输入实数矢量长度
 * @param[out] y - 输出实数矢量指针y, x1与y可以同源算
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vpack8_f(const float *x, const int nx, int *y);//75

/**
 * @brief 求方差
 * @param[in] x - 输入数据指针
 * @param[in] nx - 一维数据长度
 * @param[out] y - 方差值
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vvar_f(const float *x, const int nx, float *y);//76

/**
 * @brief 一维数据循环移位
 * @param[in] x - 输入数据指针
 * @param[in] nx - 一维数据长度
 * @param[in] shift - 循环移位长度 shift>0 右移 shift<0 左移
 * @param[out] y - 输出数据指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vshift_f(const float *x, const int nx, const int shift, float *y);//77

/**
 * @brief 浮点单精度复数矩阵截取转置
 * @param[in] x - 输入复数矢量指针
 * @param[in] nn - 输入矩阵行数目
 * @param[in] mm - 输入矩阵列数目
 * @param[in] sa - 输出转置矩阵行数目
 * @param[in] sb - 输出转置矩阵列数目
 * @param[out] y - 输出复数矢量指针
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cmtrans_sf(const float *x, const int nn, const int mm, const int sa, const int sb, float *y);//78

/**
 * Function: 产生上升或者下降的斜坡,y=a+b*i
 * @brief 浮点单精度复数矢量与单精度实数矢量的乘法
 * @param[in] x1 - 输入实数x1
 * @param[in] x2 - 输入实数x2
 * @param[in] nx - 离散点数nx
 * @param[out] y - 输出一维向量指针y
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_vramp_f(const float x1,const float x2,const int nx,float *y);//79

/**
 * @brief 浮点单精度复数矢量与单精度实数矢量的乘法
 * @param[in] x1 - 输入复数矢量指针x1
 * @param[in] x2 - 输入实数矢量指针x2
 * @param[in] nx - 复数矢量长度nx
 * @param[out] y - 输出矢量指针y,x1和y可以同源
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmulv_f(const float *x1, const float *x2, const int nx, float *y);//80
/**
 * @brief 复数矢量和实数标量的乘法
 * @param[in] x1 - 输入输出复数矢量指针 x1(输入输出同一个指针)
 * @param[in] x2 - 输入复数标量     x2
 * @param[in] step - 步长
 * @param[in] nx - 输入复数矢量长度
 * @return 成功返回0，错误返回负数，警告返回正数
 * ::birmSuccess,
 * ::birmParamNullError,
 * ::birmParamLengthInvalidError.
 */
int birm_cvmuls_sf(float *x1, const float x2, const int step, const int nx);//81

#define birmV10logF birm_v10log_f	//0
#define birmVsinF birm_vsin_f		//1
#define birmVcosF birm_vcos_f		//2
#define birmVsqrtF birm_vsqrt_f		//3
#define birmCvexpF birm_cvexp_f		//4
#define birmVaddsF birm_vadds_f		//5
#define birmVaddsSF birm_vadds_sf	//6
#define birmVsubsF birm_vsubs_f		//7
#define birmVsubsSF birm_vsubs_sf	//8
#define birmSsubvF birm_ssubv_f		//9
#define birmVmulsF birm_vmuls_f		//10
#define birmVmulsSF birm_vmuls_sf	//11
#define birmVdivsF birm_vdivs_f		//12
#define birmVaddvF birm_vaddv_f		//13
#define birmVsubvF birm_vsubv_f		//14
#define birmVmulvF birm_vmulv_f		//15
#define birmVdivvF birm_vdivv_f		//16
#define birmVdotvF birm_vdotv_f		//17
#define birmVdotvSF birm_vdotv_sf	//18
#define birmCvaddcsF birm_cvaddcs_f//19
#define birmCvsubcsF birm_cvsubcs_f//20
#define birmCvmulsIF birm_cvmuls_if//21
#define birmCvmulcsF birm_cvmulcs_f//22
#define birmCvmulcsaddcvF birm_cvmulcsaddcv_f//23
#define birmCvaddcvF birm_cvaddcv_f//24
#define birmCvsubcvF birm_cvsubcv_f//25
#define birmCvmulcvF birm_cvmulcv_f//26
#define birmCvdivcvF birm_cvdivcv_f//27
#define birmCvmuljvF birm_cvmuljv_f//28
#define birmCvdotcvSF birm_cvdotcv_sf//29
#define birmCvdotjvSF birm_cvdotjv_sf//30
#define birmCvdotjvF birm_cvdotjv_f//31
#define birmCvdotcvF birm_cvdotcv_f//32
#define birmCmtransF birm_cmtrans_f//33
#define birmMtransF birm_mtrans_f	//34
#define birmVabsF birm_vabs_f		//35
#define birmVfillF birm_vfill_f		//36
#define birmVfillSF birm_vfill_sf	//37
#define birmCvabsF birm_cvabs_f		//38
#define birmCvabssF birm_cvabss_f	//39
#define birmJvF birm_jv_f			//40
#define birmCvswapF birm_cvswap_f	//41
#define birmVlgF birm_vlg_f			//42
#define birmVclipF birm_vclip_f		//43
#define birmVthrshF birm_vthrsh_f	//44
#define birmVthresF birm_vthres_f	//45
#define birmVbsortiF birm_vbsorti_f//46
#define birmCvsumF birm_cvsum_f		//47
#define birmVsumF birm_vsum_f		//48
#define birmVsumSf birm_vsum_sf		//49
#define birmVsumsF birm_vsums_f		//50
#define birmVsumsSf birm_vsums_sf	//51
#define birmVsummF birm_vsumm_f		//52
#define birmVmeanF birm_vmean_f		//53
#define birmVmaxvF birm_vmaxv_f		//54
#define birmVminvF birm_vminv_f		//55
#define birmVmaxF birm_vmax_f		//56
#define birmVminF birm_vmin_f		//57
#define birmVminiF birm_vmini_f		//58
#define birmVmaxiF birm_vmaxi_f		//59
#define birmVmovavgF birm_vmovavg_f//60
#define birmVmovF birm_vmov_f		//61
#define birmVmovSF birm_vmov_sf		//62
#define birmCvmovSF birm_cvmov_sf	//63
#define birmInt2floatF birm_int2float_f//64
#define birmVfloat2intF birm_vfloat2int_f//65
#define birmVreversalF birm_vreversal_f//66
#define birmVpow2F birm_vpow2_f		//67
#define birmFftF birm_fft_f			//68
#define birmIfftF birm_ifft_f		//69
#define birmVint16tocvF birm_vint16tocv_f		//70
#define birmVint16tocvSF birm_vint16tocv_sf	//71
#define birmCvtovint16F birm_cvtovint16_f		//72
#define birmCvtovint16SF birm_cvtovint16_sf	//73
#define birmVpack16F birm_vpack16_f//74
#define birmVpack8F birm_vpack8_f	//75
#define birmVvarF birm_vvar_f		//76
#define birmVshiftF birm_vshift_f	//77
#define birmCmtranssF birm_cmtrans_sf//78
#define birmVrampF birm_vramp_f		//79
#define birmCvmulvF birm_cvmulv_f	//80
#define birmCvmulssF birm_cvmuls_sf//81
#define birmVmulvSF birm_vmulv_sf		//82

#ifdef __cplusplus
}
#endif

#endif
