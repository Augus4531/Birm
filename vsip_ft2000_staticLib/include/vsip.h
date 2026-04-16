#ifndef VSIP_HEADER_H
#define VSIP_HEADER_H

#ifdef _cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define VSIP_PI    M_PI

#define VSIP_MAX_SCALAR_F 	FLT_MAX
#define VSIP_MAX_SCALAR_I 	INT_MAX
#define VSIP_MAX_SCALAR_SI 	SHRT_MAX
#define VSIP_MAX_SCALAR_BL 	INT_MAX
#define VSIP_MAX_SCALAR_VI 	ULONG_MAX
#define VSIP_MAX_SCALAR_D 	DBL_MAX

#define VSIP_MIN_SCALAR_F 	FLT_MIN
#define VSIP_MIN_SCALAR_I 	INT_MIN
#define VSIP_MIN_SCALAR_D 	DBL_MIN
#define VSIP_MIN_SCALAR_SI 	SHRT_MIN
#define VSIP_MIN_SCALAR_BL 	INT_MIN
#define VSIP_MIN_SCALAR_VI	0

#define VSIP_FALSE 	0
#define VSIP_TRUE 	1

//#define VSIP_CMPLX_MEM VSIP_CMPLX_INTERLEAVED

typedef signed short int vsip_scalar_si;
typedef float vsip_scalar_f;
typedef signed int vsip_scalar_i;
typedef signed int vsip_scalar_bl;
typedef vsip_scalar_bl vsip_bool;
typedef struct {vsip_scalar_f r, i;} vsip_cscalar_f;
typedef unsigned int vsip_scalar_vi;
typedef struct {vsip_scalar_vi r, c;} vsip_scalar_mi;
typedef double vsip_scalar_d;
typedef struct {vsip_scalar_d r, i;} vsip_cscalar_d;


struct vsip_blockobject_d;
typedef struct vsip_blockobject_d vsip_block_d;
struct vsip_blockobject_f;
typedef struct vsip_blockobject_f vsip_block_f;
struct vsip_blockobject_si;
typedef struct vsip_blockobject_si vsip_block_si;
struct vsip_blockobject_i;
typedef struct vsip_blockobject_i vsip_block_i;
struct vsip_blockobject_bl;
typedef struct vsip_blockobject_bl vsip_block_bl;
struct vsip_blockobject_vi;
typedef struct vsip_blockobject_vi vsip_block_vi;
struct vsip_blockobject_mi;
typedef struct vsip_blockobject_mi vsip_block_mi;
struct vsip_cblockobject_f;
typedef struct vsip_cblockobject_f vsip_cblock_f;
struct vsip_cblockobject_d;
typedef struct vsip_cblockobject_d vsip_cblock_d;

typedef vsip_scalar_vi vsip_length;
typedef vsip_scalar_vi vsip_index;
typedef vsip_scalar_vi vsip_offset;
typedef signed long int vsip_stride;

typedef enum{VSIP_PRNG=0, VSIP_NPRNG=1}vsip_rng;
typedef enum{VSIP_MEM_NONE=0, VSIP_MEM_RDONLY, VSIP_MEM_CONST, VSIP_MEM_SHARED, VSIP_MEM_SHARED_RDONLY, VSIP_MEM_SHARED_CONST}vsip_memory_hint;
typedef enum{VSIP_CMPLX_INTERLEAVED=0, VSIP_CMPLX_SPLIT=1, VSIP_CMPLX_NONE=2 }vsip_cmplx_mem;
typedef enum {VSIP_NONSYM = 0,  VSIP_SYM_EVEN_LEN_ODD = 1,  VSIP_SYM_EVEN_LEN_EVEN = 2 } vsip_symmetry;
typedef enum{VSIP_STORAGE_FORMAT_ANY=3, VSIP_STORAGE_FORMAT_ARRAY=2, VSIP_STORAGE_FORMAT_SPLIT_COMPLEX=1, VSIP_STORAGE_FORMAT_INTERLEAVED_COMPLEX=0}vsip_storage_format;
typedef enum {VSIP_CCFFTOP, VSIP_CCFFTIP, VSIP_RCFFTOP, VSIP_CRFFTOP }vsip_ffttype;
//
typedef enum {VSIP_CCFFTMOP, VSIP_CCFFTMIP, VSIP_RCFFTMOP, VSIP_CRFFTMOP }vsip_fftmtype;


typedef struct { vsip_offset offset; vsip_stride stride; vsip_length length; vsip_block_f *block; } vsip_vattr_f;
typedef struct { vsip_offset offset; vsip_stride stride; vsip_length length; vsip_block_i *block; } vsip_vattr_i;
typedef struct { vsip_offset offset; vsip_stride stride; vsip_length length; vsip_cblock_f *block;} vsip_cvattr_f;
typedef struct { vsip_offset offset; vsip_stride stride; vsip_length length; vsip_block_bl *block; } vsip_vattr_bl;
typedef struct { vsip_offset offset; vsip_stride stride; vsip_length length; vsip_block_vi *block; } vsip_vattr_vi;
typedef struct { vsip_offset offset; vsip_stride stride; vsip_length length; vsip_block_mi *block;} vsip_vattr_mi;
typedef struct { vsip_offset offset; vsip_stride col_stride; vsip_length col_length; vsip_stride row_stride; vsip_length row_length; vsip_block_f* block; }vsip_mattr_f;
typedef struct { vsip_offset offset; vsip_stride row_stride; vsip_length row_length; vsip_stride col_stride; vsip_length col_length; vsip_cblock_f *block; } vsip_cmattr_f;

struct vsip_vviewobject_vi;
typedef struct vsip_vviewobject_vi vsip_vview_vi;
struct vsip_vviewobject_mi;
typedef struct vsip_vviewobject_mi vsip_vview_mi;
struct vsip_vviewobject_bl;
typedef struct vsip_vviewobject_bl vsip_vview_bl;
struct vsip_vviewobject_i;
typedef struct vsip_vviewobject_i vsip_vview_i;
struct vsip_vviewobject_f;
typedef struct vsip_vviewobject_f vsip_vview_f;
struct vsip_vviewobject_si;
typedef struct vsip_vviewobject_si vsip_vview_si;
struct vsip_cvviewobject_f;
typedef struct vsip_cvviewobject_f vsip_cvview_f;
struct vsip_cvviewobject_d;
typedef struct vsip_cvviewobject_d vsip_cvview_d;
struct vsip_mviewobject_f;
typedef struct vsip_mviewobject_f vsip_mview_f;
struct vsip_cmviewobject_f;
typedef struct vsip_cmviewobject_f vsip_cmview_f;

typedef enum{VSIP_ROW=0, VSIP_COL=1}vsip_major;

typedef enum{VSIP_ALG_TIME=0, VSIP_ALG_SPACE=1, VSIP_ALG_NOISE=2}vsip_alg_hint;

typedef enum{VSIP_FFT_FWD=-1, VSIP_FFT_INV=1}vsip_fft_dir;
typedef enum{VSIP_FFT_IP=0, VSIP_FFT_OP=1}vsip_fft_place;
typedef struct {vsip_scalar_vi input; vsip_scalar_vi output; vsip_fft_place place; vsip_scalar_f scale; vsip_fft_dir dir; }vsip_fft_attr_f;//
typedef struct {vsip_scalar_mi input; vsip_scalar_mi output; vsip_fft_place place; vsip_scalar_f scale; vsip_fft_dir dir; vsip_major major; }vsip_fftm_attr_f;
struct vsip_fftobject_f;
typedef struct vsip_fftobject_f vsip_fft_f;
struct vsip_fftmobject_f;
typedef struct vsip_fftmobject_f vsip_fftm_f;

struct vsip_firobject_f;
typedef struct vsip_firobject_f vsip_fir_f;
struct vsip_cfirobject_f;
typedef struct vsip_cfirobject_f vsip_cfir_f;
typedef enum{ VSIP_STATE_NO_SAVE=1, VSIP_STATE_SAVE=2 }vsip_obj_state;
typedef struct { vsip_scalar_vi kernel_len; vsip_symmetry symm; vsip_scalar_vi in_len; vsip_scalar_vi out_len; vsip_length decimation; vsip_obj_state state;} vsip_fir_attr_f;
typedef struct{ vsip_scalar_vi kernel_len; vsip_symmetry symm; vsip_scalar_vi in_len; vsip_scalar_vi out_len; vsip_length decimation; vsip_obj_state state;} vsip_cfir_attr_f;

struct vsip_conv1dobject_f;
typedef struct vsip_conv1dobject_f vsip_conv1d_f;
typedef enum { VSIP_SUPPORT_FULL = 0,  VSIP_SUPPORT_SAME = 1, VSIP_SUPPORT_MIN = 2} vsip_support_region;
typedef struct { vsip_scalar_vi kernel_len; vsip_symmetry symm; vsip_scalar_vi data_len; vsip_support_region support; vsip_scalar_vi out_len; vsip_length decimation;} vsip_conv1d_attr_f;

struct vsip_corr1dobject_f;
typedef struct vsip_corr1dobject_f vsip_corr1d_f;
struct vsip_ccorr1dobject_f;
typedef struct vsip_ccorr1dobject_f vsip_ccorr1d_f;
typedef enum {VSIP_BIASED=0, VSIP_UNBIASED=1}vsip_bias;
typedef struct { vsip_scalar_vi ref_len; vsip_scalar_vi data_len; vsip_support_region support; vsip_scalar_vi lag_len;} vsip_corr1d_attr_f;
typedef struct { vsip_scalar_vi ref_len; vsip_scalar_vi data_len; vsip_support_region support; vsip_scalar_vi lag_len;} vsip_ccorr1d_attr_f;

typedef enum{ VSIP_HIST_RESET = 1, VSIP_HIST_ACCUM = 2 } vsip_hist_opt;
typedef enum { VSIP_MAT_NTRANS = 0, VSIP_MAT_TRANS = 1, VSIP_MAT_HERM = 2, VSIP_MAT_CONJ = 3} vsip_mat_op;

typedef struct {vsip_length n;} vsip_lu_attr_f;
typedef struct { vsip_length n;} vsip_clu_attr_f;
struct vsip_luobject_f;
typedef struct vsip_luobject_f vsip_lu_f;
struct vsip_cluobject_f;
typedef struct vsip_cluobject_f vsip_clu_f;

typedef enum { VSIP_TR_LOW = 0, VSIP_TR_UPP = 1} vsip_mat_uplo;
typedef struct { vsip_mat_uplo uplo;  vsip_length n; }vsip_chol_attr_f;
typedef struct { vsip_mat_uplo uplo;  vsip_length n; } vsip_cchol_attr_f;
struct vsip_choldobject_f;
typedef struct vsip_choldobject_f vsip_chol_f;
struct vsip_ccholdobject_f;
typedef struct vsip_ccholdobject_f vsip_cchol_f;

typedef enum { VSIP_MAT_LSIDE=0, VSIP_MAT_RSIDE=1 }vsip_mat_side;
typedef enum { VSIP_QRD_NOSAVEQ = 0, VSIP_QRD_SAVEQ = 1, VSIP_QRD_SAVEQ1 = 2 } vsip_qrd_qopt;
typedef enum { VSIP_COV = 0,  VSIP_LLS = 1 } vsip_qrd_prob;
typedef struct {vsip_length m; vsip_length n; vsip_qrd_qopt Qopt;} vsip_qr_attr_f;
typedef struct {vsip_length m; vsip_length n; vsip_qrd_qopt Qopt;} vsip_cqr_attr_f;
struct vsip_qrobject_f;
typedef struct vsip_qrobject_f vsip_qr_f;
struct vsip_cqrobject_f;
typedef struct vsip_cqrobject_f vsip_cqr_f;


typedef enum { VSIP_SVD_UVNOS=0, VSIP_SVD_UVFULL=1, VSIP_SVD_UVPART=2 } vsip_svd_uv;
typedef struct { vsip_length m; vsip_length n; vsip_svd_uv Usave; vsip_svd_uv Vsave; } vsip_sv_attr_f;
typedef struct { vsip_length m; vsip_length n; vsip_svd_uv Usave; vsip_svd_uv Vsave; } vsip_csv_attr_f;
struct vsip_svobject_f;
typedef struct vsip_svobject_f vsip_sv_f;
struct vsip_csvdobject_f;
typedef struct vsip_csvdobject_f vsip_csv_f;


struct vsip_randomstate;
typedef struct vsip_randomstate vsip_randstate;

int vsip_init(void *ptr);
int vsip_finalize(void *ptr);

vsip_block_f  *vsip_blockcreate_f(vsip_length N, vsip_memory_hint hint);
vsip_block_f  *vsip_blockbind_f(vsip_scalar_f *data, vsip_length N, vsip_memory_hint hint);
vsip_scalar_f *vsip_blockfind_f(const vsip_block_f *block);
void vsip_blockdestroy_f(vsip_block_f *block);
int vsip_blockadmit_f(vsip_block_f *block, vsip_scalar_bl update);
vsip_scalar_f *vsip_blockrelease_f(vsip_block_f *block, vsip_scalar_bl update);
vsip_scalar_f *vsip_blockrebind_f(vsip_block_f *block, vsip_scalar_f *data);

vsip_block_i  *vsip_blockcreate_i(vsip_length N, vsip_memory_hint hint);
vsip_block_i  *vsip_blockbind_i(vsip_scalar_i *data, vsip_length N, vsip_memory_hint hint);
vsip_scalar_i *vsip_blockfind_i(const vsip_block_i *block);
void vsip_blockdestroy_i(vsip_block_i *block);
int vsip_blockadmit_i(vsip_block_i *block, vsip_scalar_bl update);
vsip_scalar_i *vsip_blockrelease_i(vsip_block_i *block, vsip_scalar_bl update);
vsip_scalar_i *vsip_blockrebind_i(vsip_block_i *block, vsip_scalar_i *data);

vsip_cblock_f  *vsip_cblockcreate_f(vsip_length N, vsip_memory_hint hint);
vsip_cblock_f  *vsip_cblockbind_f(vsip_scalar_f *data1, vsip_scalar_f *data2, vsip_length N, vsip_memory_hint hint);
void vsip_cblockfind_f(const vsip_cblock_f *block, vsip_scalar_f **data1, vsip_scalar_f **data2);
void vsip_cblockdestroy_f(vsip_cblock_f *block);
int vsip_cblockadmit_f(vsip_cblock_f *block, vsip_scalar_bl update);
void vsip_cblockrelease_f(vsip_cblock_f *block, vsip_scalar_bl update, vsip_scalar_f **data1, vsip_scalar_f ** data2);
void vsip_cblockrebind_f(vsip_cblock_f *block, vsip_scalar_f *data1, vsip_scalar_f *data2, vsip_scalar_f **prevdata1, vsip_scalar_f ** prevdata2);

vsip_block_bl  *vsip_blockcreate_bl(vsip_length N, vsip_memory_hint hint);
vsip_block_bl  *vsip_blockbind_bl(vsip_scalar_bl *data, vsip_length N, vsip_memory_hint hint);
vsip_scalar_bl *vsip_blockfind_bl(const vsip_block_bl *block);
void vsip_blockdestroy_bl(vsip_block_bl *block);
int vsip_blockadmit_bl(vsip_block_bl *block, vsip_scalar_bl update);
vsip_scalar_bl *vsip_blockrelease_bl(vsip_block_bl *block, vsip_scalar_bl update);
vsip_scalar_bl *vsip_blockrebind_bl(vsip_block_bl *block, vsip_scalar_bl *data);

vsip_block_vi  *vsip_blockcreate_vi(vsip_length N, vsip_memory_hint hint);
vsip_block_vi  *vsip_blockbind_vi(vsip_scalar_vi *data, vsip_length N, vsip_memory_hint hint);
vsip_scalar_vi *vsip_blockfind_vi(const vsip_block_vi *block);
void vsip_blockdestroy_vi(vsip_block_vi *block);
int vsip_blockadmit_vi(vsip_block_vi *block, vsip_scalar_bl update);
vsip_scalar_vi *vsip_blockrelease_vi(vsip_block_vi *block, vsip_scalar_bl update);
vsip_scalar_vi *vsip_blockrebind_vi(vsip_block_vi *block, vsip_scalar_vi *data);

vsip_block_mi  *vsip_blockcreate_mi(vsip_length N, vsip_memory_hint hint);
vsip_block_mi  *vsip_blockbind_mi(vsip_scalar_vi *data, vsip_length N, vsip_memory_hint hint);
vsip_scalar_vi *vsip_blockfind_mi(const vsip_block_mi *block);
void vsip_blockdestroy_mi(vsip_block_mi *block);
int vsip_blockadmit_mi(vsip_block_mi *block, vsip_scalar_bl update);
vsip_scalar_vi *vsip_blockrelease_mi(vsip_block_mi *block, vsip_scalar_bl update);
vsip_scalar_vi *vsip_blockrebind_mi(vsip_block_mi *block, vsip_scalar_vi *data);

//vsip_cmplx_mem vsip_cstorage();
vsip_cmplx_mem vsip_cstorage(void*a);

vsip_vview_f *vsip_vrealview_f(const vsip_cvview_f *v);
vsip_vview_f *vsip_vimagview_f(const vsip_cvview_f *v);

vsip_vview_f *vsip_vbind_f(const vsip_block_f *block, vsip_offset offset, vsip_stride stride, vsip_length length);
vsip_vview_i *vsip_vbind_i(const vsip_block_i *block, vsip_offset offset, vsip_stride stride, vsip_length length);
vsip_cvview_f *vsip_cvbind_f(const vsip_cblock_f *block, vsip_offset offset, vsip_stride stride, vsip_length length);
vsip_vview_bl *vsip_vbind_bl(const vsip_block_bl *block, vsip_offset offset, vsip_stride stride, vsip_length length);
vsip_vview_vi *vsip_vbind_vi(const vsip_block_vi *block, vsip_offset offset, vsip_stride stride, vsip_length length);
vsip_vview_mi *vsip_vbind_mi(const vsip_block_mi *block, vsip_offset offset, vsip_stride stride, vsip_length length);

vsip_vview_f *vsip_vcreate_f(vsip_length N, vsip_memory_hint hint);
vsip_vview_i *vsip_vcreate_i(vsip_length N, vsip_memory_hint hint);
vsip_cvview_f *vsip_cvcreate_f(vsip_length N, vsip_memory_hint hint);
vsip_vview_bl *vsip_vcreate_bl(vsip_length length, vsip_memory_hint hint);
vsip_vview_vi *vsip_vcreate_vi(vsip_length N, vsip_memory_hint hint);
vsip_vview_mi *vsip_vcreate_mi(vsip_length N, vsip_memory_hint hint);

vsip_block_f *vsip_vdestroy_f(vsip_vview_f *v);
vsip_block_i *vsip_vdestroy_i(vsip_vview_i *v);
vsip_cblock_f *vsip_cvdestroy_f(vsip_cvview_f *v);
vsip_block_bl *vsip_vdestroy_bl(vsip_vview_bl *v);
vsip_block_vi *vsip_vdestroy_vi(vsip_vview_vi *v);
vsip_block_mi *vsip_vdestroy_mi(vsip_vview_mi *v);

void vsip_valldestroy_f(vsip_vview_f *v);
void vsip_valldestroy_i(vsip_vview_i *v);
void vsip_cvalldestroy_f(vsip_cvview_f *v);
void vsip_valldestroy_bl(vsip_vview_bl *v);
void vsip_valldestroy_vi(vsip_vview_vi *v);
void vsip_valldestroy_mi(vsip_vview_mi *v);
void vsip_valldestroy_mi(vsip_vview_mi *v);

vsip_vview_f *vsip_vcloneview_f(const vsip_vview_f *v);
vsip_vview_i *vsip_vcloneview_i(const vsip_vview_i *v);
vsip_cvview_f *vsip_cvcloneview_f(const vsip_cvview_f *v);
vsip_vview_bl *vsip_vcloneview_bl(const vsip_vview_bl *v);
vsip_vview_vi *vsip_vcloneview_vi(const vsip_vview_vi *v);
vsip_vview_mi *vsip_vcloneview_mi(const vsip_vview_mi *v);

vsip_vview_f *vsip_vsubview_f(const vsip_vview_f *v, vsip_index index, vsip_length length);
vsip_vview_i *vsip_vsubview_i(const vsip_vview_i *v, vsip_index index, vsip_length length);
vsip_cvview_f *vsip_cvsubview_f(const vsip_cvview_f *v, vsip_index index, vsip_length length);
vsip_vview_bl *vsip_vsubview_bl(const vsip_vview_bl *v, vsip_index index, vsip_length length);
vsip_vview_vi *vsip_vsubview_vi(const vsip_vview_vi *v, vsip_index index, vsip_length length);
vsip_vview_mi *vsip_vsubview_mi(const vsip_vview_mi *v, vsip_index index, vsip_length length);

void vsip_vgetattrib_f(const vsip_vview_f *v, vsip_vattr_f *attrib);
void vsip_vgetattrib_i(const vsip_vview_i *v, vsip_vattr_i *attrib);
void vsip_cvgetattrib_f(const vsip_cvview_f *v, vsip_cvattr_f *attrib);
void vsip_vgetattrib_bl(const vsip_vview_bl *v, vsip_vattr_bl *attrib);
void vsip_vgetattrib_vi(const vsip_vview_vi *v, vsip_vattr_vi *attrib);
void vsip_vgetattrib_mi(const vsip_vview_mi *v, vsip_vattr_mi *attrib);

vsip_block_f *vsip_vgetblock_f(const vsip_vview_f *v);
vsip_block_i *vsip_vgetblock_i(const vsip_vview_i *v);
vsip_cblock_f *vsip_cvgetblock_f(const vsip_cvview_f *v);
vsip_block_bl *vsip_vgetblock_bl(const vsip_vview_bl *v);
vsip_block_vi *vsip_vgetblock_vi(const vsip_vview_vi *v);
vsip_block_mi *vsip_vgetblock_mi(const vsip_vview_mi *v);

vsip_offset vsip_vgetoffset_f(const vsip_vview_f *v);
vsip_offset vsip_vgetoffset_i(const vsip_vview_i *v);
vsip_offset vsip_cvgetoffset_f(const vsip_cvview_f *v);
vsip_offset vsip_vgetoffset_bl(const vsip_vview_bl *v);
vsip_offset vsip_vgetoffset_vi(const vsip_vview_vi *v);
vsip_offset vsip_vgetoffset_mi(const vsip_vview_mi *v);

vsip_stride vsip_vgetstride_f(const vsip_vview_f *v);
vsip_stride vsip_vgetstride_i(const vsip_vview_i *v);
vsip_stride vsip_cvgetstride_f(const vsip_cvview_f *v);
vsip_stride vsip_vgetstride_bl(const vsip_vview_bl *v);
vsip_stride vsip_vgetstride_vi(const vsip_vview_vi *v);
vsip_stride vsip_vgetstride_mi(const vsip_vview_mi *v);

vsip_length vsip_vgetlength_f(const vsip_vview_f *v);
vsip_length vsip_vgetlength_i(const vsip_vview_i *v);
vsip_length vsip_cvgetlength_f(const vsip_cvview_f *v);
vsip_length vsip_cvgetlength_d(const vsip_cvview_d *v);
vsip_length vsip_vgetlength_bl(const vsip_vview_bl *v);
vsip_length vsip_vgetlength_vi(const vsip_vview_vi *v);
vsip_length vsip_vgetlength_mi(const vsip_vview_mi *v);

vsip_vview_f *vsip_vputattrib_f(vsip_vview_f *v, const vsip_vattr_f *attrib);
vsip_vview_i *vsip_vputattrib_i(vsip_vview_i *v, const vsip_vattr_i *attrib);
vsip_cvview_f *vsip_cvputattrib_f(vsip_cvview_f *v, const vsip_cvattr_f *attrib);
vsip_vview_bl *vsip_vputattrib_bl(vsip_vview_bl *v, const vsip_vattr_bl *attrib);
vsip_vview_vi *vsip_vputattrib_vi(vsip_vview_vi *v, const vsip_vattr_vi *attrib);
vsip_vview_mi *vsip_vputattrib_mi(vsip_vview_mi *v, const vsip_vattr_mi *attrib);

vsip_vview_f *vsip_vputoffset_f(vsip_vview_f *v, vsip_offset offset);
vsip_vview_i *vsip_vputoffset_i(vsip_vview_i *v, vsip_offset offset);
vsip_cvview_f *vsip_cvputoffset_f(vsip_cvview_f *v, vsip_offset offset);
vsip_vview_bl *vsip_vputoffset_bl(vsip_vview_bl *v, vsip_offset offset);
vsip_vview_vi *vsip_vputoffset_vi(vsip_vview_vi *v, vsip_offset offset);
vsip_vview_mi *vsip_vputoffset_mi(vsip_vview_mi *v, vsip_offset offset);

vsip_vview_f *vsip_vputstride_f(vsip_vview_f *v, vsip_stride stride);
vsip_vview_i *vsip_vputstride_i(vsip_vview_i *v, vsip_stride stride);
vsip_cvview_f *vsip_cvputstride_f(vsip_cvview_f *v, vsip_stride stride);
vsip_vview_bl *vsip_vputstride_bl(vsip_vview_bl *v, vsip_stride stride);
vsip_vview_vi *vsip_vputstride_vi(vsip_vview_vi *v, vsip_stride stride);
vsip_vview_mi *vsip_vputstride_mi(vsip_vview_mi *v, vsip_stride stride);

vsip_vview_f *vsip_vputlength_f(vsip_vview_f *v, vsip_length length);
vsip_vview_i *vsip_vputlength_i(vsip_vview_i *v, vsip_length length);
vsip_cvview_f* (vsip_cvputlength_f)(vsip_cvview_f* v, vsip_length n);
vsip_vview_bl *vsip_vputlength_bl(vsip_vview_bl *v, vsip_length length);
vsip_vview_vi *vsip_vputlength_vi(vsip_vview_vi *v, vsip_length length);
vsip_vview_mi *vsip_vputlength_mi(vsip_vview_mi *v, vsip_length length);

vsip_scalar_f vsip_vget_f(const vsip_vview_f *x, vsip_index j);
vsip_scalar_i vsip_vget_i(const vsip_vview_i *x, vsip_index j);
vsip_cscalar_f vsip_cvget_f(const vsip_cvview_f *x, vsip_index j);
vsip_cscalar_d vsip_cvget_d(const vsip_cvview_d *x, vsip_index j);
vsip_scalar_bl vsip_vget_bl(const vsip_vview_bl *x, vsip_index j);
vsip_scalar_vi vsip_vget_vi(const vsip_vview_vi *x, vsip_index j);
vsip_scalar_mi vsip_vget_mi(const vsip_vview_mi *x, vsip_index j);

void vsip_vput_f(const vsip_vview_f *y, vsip_index j, vsip_scalar_f x);
void vsip_vput_i(const vsip_vview_i *y, vsip_index j, vsip_scalar_i x);
void vsip_cvput_f(const vsip_cvview_f *y, vsip_index j, vsip_cscalar_f x);
void vsip_vput_bl(const vsip_vview_bl *y, vsip_index j, vsip_scalar_bl x);
void vsip_vput_vi(const vsip_vview_vi *y, vsip_index j, vsip_scalar_vi x);
void vsip_vput_mi(const vsip_vview_mi *y, vsip_index j, vsip_scalar_mi x);

void vsip_vcopy_bl_bl(const vsip_vview_bl * A, const vsip_vview_bl * R);
void vsip_vcopy_bl_f(const vsip_vview_bl * A, const vsip_vview_f * R);
void vsip_vcopy_f_bl(const vsip_vview_f * A, const vsip_vview_bl * R);
void vsip_vcopy_f_f(const vsip_vview_f * A, const vsip_vview_f * R);
void vsip_vcopy_f_i(const vsip_vview_f * A, const vsip_vview_i * R);
void (vsip_vcopy_f_si)(const vsip_vview_f* a,const vsip_vview_si* r);
void vsip_vcopy_i_f(const vsip_vview_i * A, const vsip_vview_f * R);
void vsip_vcopy_i_i(const vsip_vview_i * A, const vsip_vview_i * R);
void vsip_cvcopy_f_f(const vsip_cvview_f * A, const vsip_cvview_f * R);
void vsip_vcopy_vi_vi(const vsip_vview_vi *x, const vsip_vview_vi *y);
void vsip_vcopy_vi_i(const vsip_vview_vi *x, const vsip_vview_i *y);
void vsip_vcopy_i_vi(const vsip_vview_i *x, const vsip_vview_vi *y);
void vsip_vcopy_mi_mi(const vsip_vview_mi *x, const vsip_vview_mi *y);

vsip_mview_f *vsip_mbind_f(const vsip_block_f *block, vsip_offset offset, vsip_stride col_stride, vsip_length col_length,
							vsip_stride row_stride, vsip_length row_length);
vsip_cmview_f *vsip_cmbind_f(const vsip_cblock_f *block, vsip_offset offset, vsip_stride col_stride, vsip_length col_length,
							vsip_stride row_stride, vsip_length row_length);
vsip_mview_f *vsip_mcreate_f(vsip_length M, vsip_length N, vsip_major major, vsip_memory_hint hint);
vsip_cmview_f *vsip_cmcreate_f(vsip_length M, vsip_length N, vsip_major major, vsip_memory_hint hint);
vsip_block_f *vsip_mdestroy_f(vsip_mview_f *X);
vsip_cblock_f *vsip_cmdestroy_f(vsip_cmview_f *X);
void vsip_malldestroy_f(vsip_mview_f *X);
void vsip_cmalldestroy_f(vsip_cmview_f *X);
vsip_mview_f *vsip_mcloneview_f(const vsip_mview_f *X);
vsip_cmview_f *vsip_cmcloneview_f(const vsip_cmview_f *X);
vsip_mview_f *vsip_msubview_f(const vsip_mview_f *X, vsip_index row_index, vsip_index col_index, vsip_length M, vsip_length N);
vsip_cmview_f *vsip_cmsubview_f(const vsip_cmview_f *X, vsip_index row_index, vsip_index col_index, vsip_length M, vsip_length N);

vsip_mview_f *vsip_mrealview_f(const vsip_cmview_f *X);
vsip_mview_f *vsip_mimagview_f(const vsip_cmview_f *X);

vsip_vview_f *vsip_mrowview_f(const vsip_mview_f *X, vsip_index row_index);
vsip_cvview_f *vsip_cmrowview_f(const vsip_cmview_f *X, vsip_index row_index);
vsip_vview_f *vsip_mcolview_f(const vsip_mview_f *X, vsip_index col_index);
vsip_cvview_f *vsip_cmcolview_f(const vsip_cmview_f *X, vsip_index col_index);
vsip_vview_f *vsip_mdiagview_f(const vsip_mview_f *X, vsip_stride index);
vsip_cvview_f *vsip_cmdiagview_f(const vsip_cmview_f *X, vsip_stride index);
vsip_mview_f *vsip_mtransview_f(const vsip_mview_f *X);
vsip_cmview_f *vsip_cmtransview_f(const vsip_cmview_f *X);


void vsip_mgetattrib_f(vsip_mview_f *X, vsip_mattr_f *attrib);
void vsip_cmgetattrib_f(vsip_cmview_f *X, vsip_cmattr_f *attrib);
vsip_block_f *vsip_mgetblock_f(const vsip_mview_f *X);
vsip_cblock_f *vsip_cmgetblock_f(const vsip_cmview_f *X);
vsip_offset vsip_mgetoffset_f(const vsip_mview_f *X);
vsip_offset vsip_cmgetoffset_f(const vsip_cmview_f *X);
vsip_stride vsip_mgetrowstride_f(const vsip_mview_f *X);
vsip_stride vsip_cmgetrowstride_f(const vsip_cmview_f *X);
vsip_stride vsip_mgetcolstride_f(const vsip_mview_f *X);
vsip_stride vsip_cmgetcolstride_f(const vsip_cmview_f *X);
vsip_length vsip_mgetrowlength_f(const vsip_mview_f *X);
vsip_length vsip_cmgetrowlength_f(const vsip_cmview_f *X);
vsip_length vsip_mgetcollength_f(const vsip_mview_f *X);
vsip_length vsip_cmgetcollength_f(const vsip_cmview_f *X);
vsip_mview_f *vsip_mputattrib_f(vsip_mview_f *X, const vsip_mattr_f *attrib);
vsip_cmview_f *vsip_cmputattrib_f(vsip_cmview_f *X, const vsip_cmattr_f *attrib);
vsip_mview_f *vsip_mputoffset_f(vsip_mview_f *X, vsip_offset offset);
vsip_cmview_f *vsip_cmputoffset_f(vsip_cmview_f *X, vsip_offset offset);
vsip_mview_f *vsip_mputrowstride_f(vsip_mview_f *X, vsip_stride stride);
vsip_cmview_f *vsip_cmputrowstride_f(vsip_cmview_f *X, vsip_stride stride);
vsip_mview_f *vsip_mputcolstride_f(vsip_mview_f *X, vsip_stride stride);
vsip_cmview_f *vsip_cmputcolstride_f(vsip_cmview_f *X, vsip_stride stride);
vsip_mview_f *vsip_mputrowlength_f(vsip_mview_f *X, vsip_length length);
vsip_cmview_f *vsip_cmputrowlength_f(vsip_cmview_f *X, vsip_length length);
vsip_mview_f *vsip_mputcollength_f( vsip_mview_f *X, vsip_length length);
vsip_cmview_f *vsip_cmputcollength_f(vsip_cmview_f *X, vsip_length length);
vsip_scalar_f vsip_mget_f(const vsip_mview_f *X, vsip_index i, vsip_index j);
vsip_cscalar_f vsip_cmget_f(const vsip_cmview_f *X, vsip_index i, vsip_index j);
void vsip_mput_f(const vsip_mview_f *y, vsip_index i, vsip_index j, vsip_scalar_f x);
void vsip_cmput_f(const vsip_cmview_f *y, vsip_index i, vsip_index j, vsip_cscalar_f x);

void vsip_mcopy_f_f(const vsip_mview_f * A, const vsip_mview_f * R);
void vsip_cmcopy_f_f(const vsip_cmview_f * A, const vsip_cmview_f * R);

//vsip_matindex_mi vsip_MATINDEX_mi
//vsip_rowindex_mi vsip_colindex_mi

vsip_scalar_f vsip_arg_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_cadd_f(vsip_cscalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_rcadd_f(vsip_scalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_conj_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_crdiv_f(vsip_cscalar_f a, vsip_scalar_f b);
vsip_cscalar_f vsip_cdiv_f(vsip_cscalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_cexp_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_cjmul_f(vsip_cscalar_f a, vsip_cscalar_f b);
vsip_scalar_f vsip_cmag_f(vsip_cscalar_f a);
vsip_scalar_f vsip_cmagsq_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_cmplx_f(vsip_scalar_f a, vsip_scalar_f b);
vsip_cscalar_f vsip_cmul_f(vsip_cscalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_rcmul_f(vsip_scalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_cneg_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_crecip_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_csub_f(vsip_cscalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_rcsub_f(vsip_scalar_f a, vsip_cscalar_f b);
vsip_cscalar_f vsip_crsub_f(vsip_cscalar_f a, vsip_scalar_f b);
vsip_cscalar_f vsip_csqrt_f(vsip_cscalar_f a);
vsip_scalar_f vsip_imag_f(vsip_cscalar_f a);
void vsip_polar_f(vsip_cscalar_f a, vsip_scalar_f *radius, vsip_scalar_f *theta);
vsip_scalar_f vsip_real_f(vsip_cscalar_f a);
vsip_cscalar_f vsip_rect_f(vsip_scalar_f radius, vsip_scalar_f theta);

void vsip_CADD_f(vsip_cscalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_RCADD_f(vsip_scalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_CONJ_f(vsip_cscalar_f a, vsip_cscalar_f *r);
void vsip_CRDIV_f(vsip_cscalar_f a, vsip_scalar_f b, vsip_cscalar_f *r);
void vsip_CDIV_f(vsip_cscalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_CEXP_f(vsip_cscalar_f a, vsip_cscalar_f *r);
void vsip_CJMUL_f(vsip_cscalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_CMPLX_f(vsip_scalar_f a, vsip_scalar_f b, vsip_cscalar_f *r);
void vsip_CMUL_f(vsip_cscalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_RCMUL_f(vsip_scalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_CNEG_f(vsip_cscalar_f a, vsip_cscalar_f *r);
void vsip_CRECIP_f(vsip_cscalar_f a, vsip_cscalar_f *r);
void vsip_CSUB_f(vsip_cscalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_RCSUB_f(vsip_scalar_f a, vsip_cscalar_f b, vsip_cscalar_f *r);
void vsip_CRSUB_f(vsip_cscalar_f a, vsip_scalar_f b, vsip_cscalar_f *r);
void vsip_CSQRT_f(vsip_cscalar_f a, vsip_cscalar_f *r);
void vsip_RECT_f(vsip_scalar_f radius, vsip_scalar_f theta, vsip_cscalar_f *r);

vsip_scalar_f vsip_randu_f(vsip_randstate *state);
vsip_cscalar_f vsip_crandu_f(vsip_randstate *state);
vsip_scalar_f vsip_randn_f(vsip_randstate *state);
vsip_cscalar_f vsip_crandn_f(vsip_randstate *state);


void vsip_vacos_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vasin_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vatan_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vatan2_f(vsip_vview_f const *a, vsip_vview_f const *b, vsip_vview_f const *r);
void vsip_vcos_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vexp_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_cvexp_f(vsip_cvview_f const *a, vsip_cvview_f const *r);
void vsip_vexp10_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vlog_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vlog10_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vsin_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_vsqrt_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_cvsqrt_f(vsip_cvview_f const *a, vsip_cvview_f const *r);
void vsip_vtan_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_cvconj_f(vsip_cvview_f const *a, vsip_cvview_f const *r);
void vsip_veuler_f(vsip_vview_f const *a, vsip_cvview_f const *r);
void vsip_vmag_f(vsip_vview_f const *a, vsip_vview_f const *r);
void vsip_cvmag_f(vsip_cvview_f const *a, vsip_vview_f const *r);
void vsip_vcmagsq_f(vsip_cvview_f const *a, vsip_vview_f const *r);


vsip_scalar_f vsip_vmeanval_f(vsip_vview_f const *a);
vsip_cscalar_f vsip_cvmeanval_f(vsip_cvview_f const *a);
vsip_scalar_f vsip_vmeansqval_f(vsip_vview_f const *a);
vsip_scalar_f vsip_cvmeansqval_f(vsip_cvview_f const *a);
void vsip_vneg_f(const vsip_vview_f *a, const vsip_vview_f *r);
void vsip_cvneg_f(const vsip_cvview_f *a, const vsip_cvview_f *r);
void vsip_vrecip_f(const vsip_vview_f *a, const vsip_vview_f *r);
void vsip_cvrecip_f(const vsip_cvview_f *a, const vsip_cvview_f *r);
void vsip_vrsqrt_f(const vsip_vview_f *a, const vsip_vview_f *r);
void vsip_vsq_f(const vsip_vview_f *a, const vsip_vview_f *r);
vsip_scalar_f vsip_vsumval_f(const vsip_vview_f *a);
vsip_scalar_f vsip_vsumsqval_f(const vsip_vview_f *a);
void vsip_vadd_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_rcvadd_f(const vsip_vview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_cvadd_f(const vsip_cvview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_svadd_f(vsip_scalar_f alpha, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_rscvadd_f(vsip_scalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_csvadd_f(vsip_cscalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *r);
vsip_cscalar_f vsip_cvjdot_f(const vsip_cvview_f *a, const vsip_cvview_f *b);

void vsip_vdiv_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_cvdiv_f(const vsip_cvview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_svdiv_f(vsip_scalar_f alpha, const vsip_vview_f *b, const vsip_vview_f *r);
vsip_scalar_f vsip_vdot_f(const vsip_vview_f *a, const vsip_vview_f *b);
vsip_cscalar_f vsip_cvdot_f(const vsip_cvview_f *a, const vsip_cvview_f *b);
void vsip_vhypot_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_cvjmul_f(const vsip_cvview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_vmul_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_rcvmul_f(const vsip_vview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_cvmul_f(const vsip_cvview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_vmmul_f(const vsip_vview_f *a, const vsip_mview_f *b, vsip_major major, const vsip_mview_f *r);
void vsip_cvmmul_f(const vsip_cvview_f *a, const vsip_cmview_f *b, vsip_major major, const vsip_cmview_f *r);
void vsip_rvcmmul_f(const vsip_vview_f *a, const vsip_cmview_f *b, vsip_major major, const vsip_cmview_f *r);
void vsip_svmul_f(vsip_scalar_f alpha, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_rscvmul_f(vsip_scalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_csvmul_f(vsip_cscalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_vsub_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_rcvsub_f(const vsip_vview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_crvsub_f(const vsip_cvview_f *a, const vsip_vview_f *b, const vsip_cvview_f *r);
void vsip_cvsub_f(const vsip_cvview_f *a, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_svsub_f(vsip_scalar_f alpha, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_rscvsub_f(vsip_scalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_csvsub_f(vsip_cscalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *r);
void vsip_vam_f(vsip_vview_f const *a, vsip_vview_f const *b, vsip_vview_f const *c, vsip_vview_f const *r);
void vsip_cvam_f(vsip_cvview_f const *a, vsip_cvview_f const *b, vsip_cvview_f const *c, vsip_cvview_f const *r);
void vsip_vma_f(vsip_vview_f const *a, vsip_vview_f const *b, vsip_vview_f const *c, vsip_vview_f const *r);
void vsip_cvma_f(vsip_cvview_f const *a, vsip_cvview_f const *b, vsip_cvview_f const *c, vsip_cvview_f const *r);
void vsip_vmsa_f(vsip_vview_f const *a, vsip_vview_f const *b, vsip_scalar_f alpha, vsip_vview_f const *r);
void vsip_cvmsa_f(vsip_cvview_f const *a, vsip_cvview_f const *b, vsip_cscalar_f alpha, vsip_cvview_f const *r);
void vsip_vmsb_f(vsip_vview_f const *a, vsip_vview_f const *b, vsip_vview_f const *c, vsip_vview_f const *r);
void vsip_cvmsb_f(vsip_cvview_f const *a, vsip_cvview_f const *b, vsip_cvview_f const *c, vsip_cvview_f const *r);
void vsip_vsam_f(vsip_vview_f const *a, vsip_scalar_f beta, vsip_vview_f const *c, vsip_vview_f const *r);
void vsip_cvsam_f(vsip_cvview_f const *a, vsip_cscalar_f beta, vsip_cvview_f const *c, vsip_cvview_f const *r);
void vsip_vsbm_f(vsip_vview_f const *a, vsip_vview_f const *b, vsip_vview_f const *c, vsip_vview_f const *r);
void vsip_cvsbm_f(vsip_cvview_f const *a, vsip_cvview_f const *b, vsip_cvview_f const *c, vsip_cvview_f const *r);
void vsip_vsma_f(vsip_vview_f const *a, vsip_scalar_f beta, vsip_vview_f const *c, vsip_vview_f const *r);
void vsip_cvsma_f(vsip_cvview_f const *a, vsip_cscalar_f beta, vsip_cvview_f const *c, vsip_cvview_f const *r);
void vsip_vsmsa_f(vsip_vview_f const *a, vsip_scalar_f beta, vsip_scalar_f gamma, vsip_vview_f const *r);
void vsip_cvsmsa_f(vsip_cvview_f const *a, vsip_cscalar_f beta, vsip_cscalar_f gamma, vsip_cvview_f const *r);
void vsip_vleq_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_bl *r);
void vsip_vlge_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_bl *r);
void vsip_vlgt_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_bl *r);
void vsip_vlle_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_bl *r);
void vsip_vllt_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_bl *r);
void vsip_vlne_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_bl *r);
void vsip_vclip_f(const vsip_vview_f *a, vsip_scalar_f t1, vsip_scalar_f t2, vsip_scalar_f c1, vsip_scalar_f c2, const vsip_vview_f *r);
void vsip_vinvclip_f(const vsip_vview_f *a, vsip_scalar_f t1, vsip_scalar_f t2, vsip_scalar_f t3, vsip_scalar_f c1, vsip_scalar_f c2, const vsip_vview_f *r);
void vsip_vmax_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_vmaxmg_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_vcmaxmgsq_f(const vsip_cvview_f * A, const vsip_cvview_f * B, const vsip_vview_f * R);			  
vsip_scalar_f vsip_vcmaxmgsqval_f(const vsip_cvview_f *a, vsip_scalar_vi *index);//vsip_vcmaxmqsqval_f
vsip_scalar_f vsip_vmaxmgval_f(const vsip_vview_f *a, vsip_scalar_vi *index);
vsip_scalar_f vsip_vmaxval_f(const vsip_vview_f *a, vsip_scalar_vi *index);
void vsip_vmin_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_vminmg_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_vview_f *r);
void vsip_vcminmgsq_f(const vsip_cvview_f * A, const vsip_cvview_f * B, const vsip_vview_f * R);
vsip_scalar_f vsip_vcminmgsqval_f(const vsip_cvview_f *a, vsip_scalar_vi *index);
vsip_scalar_f vsip_vminmgval_f(const vsip_vview_f *a, vsip_scalar_vi *index);
vsip_scalar_f vsip_vminval_f(const vsip_vview_f *a, vsip_scalar_vi *index);
void vsip_vfill_f(vsip_scalar_f alpha, const vsip_vview_f *r);
void vsip_cvfill_f(vsip_cscalar_f alpha, const vsip_cvview_f *r);
void vsip_vramp_f(vsip_scalar_f alpha, vsip_scalar_f beta, const vsip_vview_f *r);
void vsip_vcmplx_f(const vsip_vview_f *a, const vsip_vview_f *b, const vsip_cvview_f *r);
void vsip_vexpoavg_f(vsip_scalar_f alpha, const vsip_vview_f *b, const vsip_vview_f *c);
void vsip_cvexpoavg_f(vsip_scalar_f alpha, const vsip_cvview_f *b, const vsip_cvview_f *c);
void vsip_vgather_f(const vsip_vview_f *x, const vsip_vview_vi *index, vsip_vview_f *y);
void vsip_cvgather_f(const vsip_cvview_f *x, const vsip_vview_vi *index, vsip_cvview_f *y);
void vsip_vimag_f(const vsip_cvview_f *a, const vsip_vview_f *r);
vsip_scalar_f vsip_vmodulate_f(const vsip_vview_f *a, vsip_scalar_f nu, vsip_scalar_f phi, const vsip_cvview_f *r);
vsip_scalar_f vsip_cvmodulate_f(const vsip_cvview_f *a, vsip_scalar_f nu, vsip_scalar_f phi, const vsip_cvview_f *r);
void vsip_vpolar_f(const vsip_cvview_f *a, const vsip_vview_f *r, const vsip_vview_f *phi);
void vsip_vreal_f(const vsip_cvview_f *a, const vsip_vview_f *r);
void vsip_vrect_f(const vsip_vview_f *r, const vsip_vview_f *phi, const vsip_cvview_f *y);
void vsip_vscatter_f(const vsip_vview_f *x, const vsip_vview_f *y, const vsip_vview_vi *index);
void vsip_cvscatter_f(const vsip_cvview_f *x, const vsip_cvview_f *y, const vsip_vview_vi *index);
void vsip_vswap_f(const vsip_vview_f *a, const vsip_vview_f *b);
void vsip_cvswap_f(const vsip_cvview_f *a, const vsip_cvview_f *b);

void vsip_vrandu_f(vsip_randstate *state, const vsip_vview_f *r);
void vsip_cvrandu_f(vsip_randstate *state, const vsip_cvview_f *r);
void vsip_vrandn_f(vsip_randstate *state, const vsip_vview_f *r);
void vsip_cvrandn_f(vsip_randstate *state, const vsip_cvview_f *r);

vsip_randstate *vsip_randcreate(vsip_index seed, vsip_index numprocs, vsip_index id, vsip_rng portable);
int vsip_randdestroy(vsip_randstate *state);


void vsip_vmag_i(const vsip_vview_i * A, const vsip_vview_i * R);
void vsip_vclip_i(const vsip_vview_i *a, vsip_scalar_i t1, vsip_scalar_i t2, vsip_scalar_i c1, vsip_scalar_i c2, const vsip_vview_i *r);
void vsip_vneg_i(const vsip_vview_i *a, const vsip_vview_i *r);
void vsip_vinvclip_i(const vsip_vview_i *a, vsip_scalar_i t1, vsip_scalar_i t2, vsip_scalar_i t3, vsip_scalar_i c1, vsip_scalar_i c2, const vsip_vview_i *r);
void vsip_vadd_i(const vsip_vview_i *a, const vsip_vview_i *b, const vsip_vview_i *r);
void vsip_svadd_i(vsip_scalar_i alpha, const vsip_vview_i *b, const vsip_vview_i *r);
void vsip_vmul_i(const vsip_vview_i *a, const vsip_vview_i *b, const vsip_vview_i *r);
void vsip_svmul_i(const vsip_scalar_i a, const vsip_vview_i * B, const vsip_vview_i * R);
void vsip_vsub_i(const vsip_vview_i *a, const vsip_vview_i *b, const vsip_vview_i *r);
void vsip_svsub_i(vsip_scalar_i alpha, const vsip_vview_i *b, const vsip_vview_i *r);

void vsip_vand_i(const vsip_vview_i *a, const vsip_vview_i *b, const vsip_vview_i *r);
void vsip_vnot_i(const vsip_vview_i *a, const vsip_vview_i *r);
void vsip_vor_i(const vsip_vview_i *a, const vsip_vview_i *b, const vsip_vview_i *r);
void vsip_vxor_i(const vsip_vview_i *a, const vsip_vview_i *b, const vsip_vview_i *r);

void vsip_vfill_i(vsip_scalar_i alpha, const vsip_vview_i *r);
void vsip_vramp_i(vsip_scalar_i alpha, vsip_scalar_i beta, const vsip_vview_i *r);
void vsip_vgather_i(const vsip_vview_i *x, const vsip_vview_vi *index, vsip_vview_i *y); 
void vsip_vscatter_i(const vsip_vview_i *x, const vsip_vview_i *y, const vsip_vview_vi *index);

vsip_scalar_vi vsip_vsumval_bl(const vsip_vview_bl *a);
void vsip_vand_bl(const vsip_vview_bl *a, const vsip_vview_bl *b, const vsip_vview_bl *r);
void vsip_vnot_bl(const vsip_vview_bl *a, const vsip_vview_bl *r);
void vsip_vor_bl(const vsip_vview_bl *a, const vsip_vview_bl *b, const vsip_vview_bl *r);
void vsip_vxor_bl(const vsip_vview_bl *a, const vsip_vview_bl *b, const vsip_vview_bl *r);
vsip_scalar_bl vsip_valltrue_bl(const vsip_vview_bl *a);
vsip_scalar_bl vsip_vanytrue_bl(const vsip_vview_bl *a);
vsip_length vsip_vindexbool(const vsip_vview_bl *b, vsip_vview_vi *index);

void vsip_ccfftop_f(const vsip_fft_f *fft, const vsip_cvview_f *x, const vsip_cvview_f *y);
void vsip_ccfftip_f(const vsip_fft_f *fft, const vsip_cvview_f *xy);
void vsip_rcfftop_f(const vsip_fft_f *fft, const vsip_vview_f *x, const vsip_cvview_f *y);
void vsip_crfftop_f(const vsip_fft_f *fft, const vsip_cvview_f *x, const vsip_vview_f *y);
vsip_fft_f *vsip_ccfftip_create_f(vsip_length N, vsip_scalar_f scale, vsip_fft_dir dir, vsip_length ntimes, vsip_alg_hint hint);
vsip_fft_f *vsip_ccfftop_create_f(vsip_length N, vsip_scalar_f scale, vsip_fft_dir dir, vsip_length ntimes, vsip_alg_hint hint);
vsip_fft_f *vsip_rcfftop_create_f(vsip_length N, vsip_scalar_f scale, vsip_length ntimes, vsip_alg_hint hint);
vsip_fft_f *vsip_crfftop_create_f(vsip_length N, vsip_scalar_f scale, vsip_length ntimes, vsip_alg_hint hint);
void vsip_fft_getattr_f(const vsip_fft_f *fft, vsip_fft_attr_f *attr);
int vsip_fft_destroy_f(vsip_fft_f *fft);

void vsip_ccfftmop_f(const vsip_fftm_f *fft, const vsip_cmview_f *X, const vsip_cmview_f *Y);
void vsip_ccfftmip_f(const vsip_fftm_f *fft, const vsip_cmview_f *XY);
void vsip_rcfftmop_f(const vsip_fftm_f *fft, const vsip_mview_f *x, const vsip_cmview_f *y);
void vsip_crfftmop_f(const vsip_fftm_f *fft, const vsip_cmview_f *X, const vsip_mview_f *Y);
vsip_fftm_f *vsip_ccfftmop_create_f(vsip_length M, vsip_length N, vsip_scalar_f scale, vsip_fft_dir dir, vsip_major major,
				vsip_length ntimes, vsip_alg_hint hint);
vsip_fftm_f *vsip_ccfftmip_create_f(vsip_length M, vsip_length N, vsip_scalar_f scale, vsip_fft_dir dir, vsip_major major, 
				vsip_length ntimes, vsip_alg_hint hint);
vsip_fftm_f *vsip_rcfftmop_create_f(vsip_length M, vsip_length N, vsip_scalar_f scale, vsip_major major, vsip_length ntimes, vsip_alg_hint hint);
vsip_fftm_f *vsip_crfftmop_create_f(vsip_length M, vsip_length N, vsip_scalar_f scale, vsip_major major, vsip_length ntimes, vsip_alg_hint hint);
void vsip_fftm_getattr_f(const vsip_fftm_f *fft, vsip_fftm_attr_f *attr);
int vsip_fftm_destroy_f(vsip_fftm_f *fft);

vsip_vview_f *vsip_vcreate_hanning_f(vsip_length N, vsip_memory_hint hint);
vsip_vview_f *vsip_vcreate_blackman_f(vsip_length N, vsip_memory_hint hint);
vsip_vview_f *vsip_vcreate_kaiser_f(vsip_length N, vsip_scalar_f beta, vsip_memory_hint hint);
vsip_vview_f *vsip_vcreate_cheby_f(vsip_length N, vsip_scalar_f ripple, vsip_memory_hint hint);


vsip_fir_f *vsip_fir_create_f(const vsip_vview_f *kernel, vsip_symmetry symm, vsip_length N, vsip_length D,
				vsip_obj_state state, vsip_length ntimes, vsip_alg_hint hint);
vsip_cfir_f *vsip_cfir_create_f(const vsip_cvview_f *kernel, vsip_symmetry symm, vsip_length N, vsip_length D,
				vsip_obj_state state,  vsip_length ntimes, vsip_alg_hint hint);
int vsip_fir_destroy_f(vsip_fir_f *filt);
int vsip_cfir_destroy_f(vsip_cfir_f *filt);
void vsip_fir_getattr_f(const vsip_fir_f *filt, vsip_fir_attr_f *attr);
void vsip_cfir_getattr_f(const vsip_cfir_f *filt, vsip_cfir_attr_f *attr);				
int vsip_firflt_f(vsip_fir_f *filt, const vsip_vview_f *x, const vsip_vview_f *y);
int vsip_cfirflt_f(vsip_cfir_f *filt, const vsip_cvview_f *x, const vsip_cvview_f *y);				
				
				
vsip_conv1d_f *vsip_conv1d_create_f(const vsip_vview_f *h, vsip_symmetry symm, vsip_length N, vsip_length D,
					vsip_support_region support, vsip_length ntimes, vsip_alg_hint hint);				
vsip_length vsip_conv1d_destroy_f(vsip_conv1d_f *conv1d);							
void vsip_conv1d_getattr_f(const vsip_conv1d_f *conv1d, vsip_conv1d_attr_f *attr);				
void vsip_convolve1d_f(const vsip_conv1d_f *conv1d, const vsip_vview_f *x, const vsip_vview_f *y);				
				
				
vsip_corr1d_f *vsip_corr1d_create_f(vsip_length M, vsip_length N, vsip_support_region support, vsip_length ntimes, vsip_alg_hint hint);
vsip_ccorr1d_f *vsip_ccorr1d_create_f(vsip_length M, vsip_length N, vsip_support_region support, vsip_length ntimes, vsip_alg_hint hint);
int vsip_corr1d_destroy_f(vsip_corr1d_f *corr1d);
int vsip_ccorr1d_destroy_f(vsip_ccorr1d_f *corr1d);
void vsip_corr1d_getattr_f(const vsip_corr1d_f *corr1d, vsip_corr1d_attr_f *attr);
void vsip_ccorr1d_getattr_f(const vsip_ccorr1d_f *corr1d, vsip_ccorr1d_attr_f *attr);
void vsip_correlate1d_f(const vsip_corr1d_f *corr1d, vsip_bias bias, const vsip_vview_f *ref, const vsip_vview_f *x, const vsip_vview_f *y);
void vsip_ccorrelate1d_f(const vsip_ccorr1d_f *corr1d, vsip_bias bias, const vsip_cvview_f *ref, const vsip_cvview_f *x, const vsip_cvview_f *y);

void vsip_vhisto_f(const vsip_vview_f *src, vsip_scalar_f min_bin, vsip_scalar_f max_bin, vsip_hist_opt opt, const vsip_vview_f *hist);


void vsip_vmprod_f(const vsip_vview_f *x, const vsip_mview_f *A, const vsip_vview_f *y);
void vsip_mprodt_f(const vsip_mview_f *A, const vsip_mview_f *B, const vsip_mview_f *C);
void vsip_cmprodt_f(const vsip_cmview_f *A, const vsip_cmview_f *B, const vsip_cmview_f *C);
void vsip_cvmprod_f(const vsip_cvview_f *x, const vsip_cmview_f *A, const vsip_cvview_f *y);
void vsip_cmprodh_f(const vsip_cmview_f *A, const vsip_cmview_f *B, const vsip_cmview_f *C);

void vsip_mvprod_f(const vsip_mview_f *A, const vsip_vview_f *x, const vsip_vview_f *y);
void vsip_cmvprod_f(const vsip_cmview_f *A, const vsip_cvview_f *x, const vsip_cvview_f *y);
void vsip_mprod_f(const vsip_mview_f *A, const vsip_mview_f *B, const vsip_mview_f *C);
void vsip_cmprod_f(const vsip_cmview_f *A, const vsip_cmview_f *B, const vsip_cmview_f *C);
void vsip_cmprodj_f(const vsip_cmview_f *A, const vsip_cmview_f *B, const vsip_cmview_f *C);

void vsip_mtrans_f(const vsip_mview_f *A, const vsip_mview_f *C);
void vsip_cmtrans_f(const vsip_cmview_f *A, const vsip_cmview_f *C);
void vsip_cmherm_f(const vsip_cmview_f *A, const vsip_cmview_f *C);


void vsip_gemp_f(vsip_scalar_f alpha, const vsip_mview_f *A, vsip_mat_op OpA, 
		const vsip_mview_f *B, vsip_mat_op OpB, vsip_scalar_f beta, const vsip_mview_f *C);
void vsip_cgemp_f(vsip_cscalar_f alpha, const vsip_cmview_f *A, vsip_mat_op OpA,
		const vsip_cmview_f *B, vsip_mat_op OpB, vsip_cscalar_f beta, const vsip_cmview_f *C);
void vsip_gems_f(vsip_scalar_f alpha, const vsip_mview_f *A, vsip_mat_op OpA, vsip_scalar_f beta, const vsip_mview_f *C);
void vsip_cgems_f(vsip_cscalar_f alpha, const vsip_cmview_f *A, vsip_mat_op OpA, vsip_cscalar_f beta, const vsip_cmview_f *C);
void vsip_vouter_f(vsip_scalar_f alpha, const vsip_vview_f *x, const vsip_vview_f *y, const vsip_mview_f *C);
void vsip_cvouter_f(vsip_cscalar_f alpha, const vsip_cvview_f *x, const vsip_cvview_f *y, const vsip_cmview_f *C);

vsip_lu_f *vsip_lud_create_f(vsip_length N);
vsip_clu_f *vsip_clud_create_f(vsip_length N);
int vsip_lud_f(vsip_lu_f *lud, const vsip_mview_f *A);
int vsip_clud_f(vsip_clu_f *lud, const vsip_cmview_f *A);
void vsip_lud_getattr_f(const vsip_lu_f *lud, vsip_lu_attr_f *attr);
void vsip_clud_getattr_f(const vsip_clu_f *lud, vsip_clu_attr_f *attr);
int vsip_lusol_f(const vsip_lu_f *lud, vsip_mat_op OpA, const vsip_mview_f *XB);
int vsip_clusol_f(const vsip_clu_f *lud, vsip_mat_op OpA, const vsip_cmview_f *XB);
int vsip_lud_destroy_f(vsip_lu_f *lud);
int vsip_clud_destroy_f(vsip_clu_f *lud);

int vsip_chold_f(vsip_chol_f *chold, const vsip_mview_f *A);
int vsip_cchold_f(vsip_cchol_f *chold, const vsip_cmview_f *A);
vsip_chol_f *vsip_chold_create_f(vsip_mat_uplo uplo, vsip_length N);
vsip_cchol_f *vsip_cchold_create_f(vsip_mat_uplo uplo, vsip_length N);
int vsip_chold_destroy_f(vsip_chol_f *chold);
int vsip_cchold_destroy_f(vsip_cchol_f *chold);
void vsip_chold_getattr_f(const vsip_chol_f *chold, vsip_chol_attr_f *attr);
void vsip_cchold_getattr_f(const vsip_cchol_f *chold, vsip_cchol_attr_f *attr);
int vsip_cholsol_f(const vsip_chol_f *chold, const vsip_mview_f *XB);
int vsip_ccholsol_f(const vsip_cchol_f *chold, const vsip_cmview_f *XB);

int vsip_qrd_f(vsip_qr_f *qrd, const vsip_mview_f *A);
int vsip_cqrd_f(vsip_cqr_f *qrd, const vsip_cmview_f *A);
vsip_qr_f *vsip_qrd_create_f(vsip_length M, vsip_length N, vsip_qrd_qopt qopt);
vsip_cqr_f *vsip_cqrd_create_f(vsip_length M, vsip_length N, vsip_qrd_qopt qopt);
int vsip_qrsol_f(const vsip_qr_f *qrd, vsip_qrd_prob prob, const vsip_mview_f *XB);
int vsip_cqrsol_f(const vsip_cqr_f *qrd, vsip_qrd_prob prob, const vsip_cmview_f *XB);
int vsip_qrdprodq_f(const vsip_qr_f *qrd, vsip_mat_op opQ, vsip_mat_side apQ, const vsip_mview_f *C);
int vsip_cqrdprodq_f(const vsip_cqr_f *qrd, vsip_mat_op opQ, vsip_mat_side apQ, const vsip_cmview_f *C);
int vsip_qrdsolr_f(const vsip_qr_f *qrd, vsip_mat_op OpR, vsip_scalar_f alpha, const vsip_mview_f *XB);
int vsip_cqrdsolr_f(const vsip_cqr_f *qrd, vsip_mat_op OpR, vsip_cscalar_f alpha, const vsip_cmview_f *XB);
void vsip_qrd_getattr_f(const vsip_qr_f *qrd, vsip_qr_attr_f *attr);
void vsip_cqrd_getattr_f(const vsip_cqr_f *qrd, vsip_cqr_attr_f *attr);
int vsip_qrd_destroy_f(vsip_qr_f *qrd);
int vsip_cqrd_destroy_f(vsip_cqr_f *qrd);

int vsip_toepsol_f(const vsip_vview_f *t, const vsip_vview_f *b, const vsip_vview_f *w, const vsip_vview_f *x);
int vsip_ctoepsol_f(const vsip_cvview_f *t, const vsip_cvview_f *b, const vsip_cvview_f *w, const vsip_cvview_f *x);
int vsip_covsol_f(const vsip_mview_f *A, const vsip_mview_f *XB);
int vsip_ccovsol_f(const vsip_cmview_f *A, const vsip_cmview_f *XB);
int vsip_llsqsol_f(const vsip_mview_f *A, const vsip_mview_f *XB);
int vsip_cllsqsol_f(const vsip_cmview_f *A, const vsip_cmview_f *XB);


//2023-12-7
typedef unsigned int vsip_scalar_ue32;
void vsip_vunbiasfull_f(const vsip_corr1d_f *cor,const vsip_vview_f *x,const vsip_vview_f *y);
//typedef enum { VSIP_CCFFTOP, VSIP_CCFFTIP, VSIP_RCFFTOP, VSIP_CRFFTOP }vsip_ffttype;
void vsip_cinterleaved(vsip_length N, vsip_cblock_f *b);
void (vsip_csplit)(vsip_length N, vsip_cblock_f *b);
//extend
void vsip_cvgemul_f(vsip_scalar_f a,const vsip_cvview_f *b,const vsip_cvview_f *r);
void (vsip_nadds_f)( vsip_vview_i const *a,  vsip_scalar_f beta,  vsip_vview_f const *r);
void (vsip_naddsmuls_f)(vsip_vview_i const *a, vsip_scalar_f c, vsip_scalar_f d,vsip_vview_f const *r);
void vsip_ssubnmuls_f (vsip_vview_i const *a, vsip_scalar_f c, vsip_scalar_f d, vsip_vview_f const *r);
void (vsip_vabs_f)(const vsip_vview_f *a,const vsip_vview_f *r);
void (vsip_varg_f)(const vsip_cvview_f* a,const vsip_vview_f* r);
void vsip_vceil_f(vsip_vview_f const *a,vsip_vview_f const *r);
void vsip_vcopy_f_si(const vsip_vview_f *a,const vsip_vview_si *r);
void vsip_vcopy_si_f(const vsip_vview_si *a,const vsip_vview_f *r);
void vsip_vcumsum_f(const vsip_vview_f *a,const vsip_vview_f *r);
void vsip_vdiff_f(const vsip_vview_f *a,const vsip_vview_f *r);
void vsip_vfloor_f(const vsip_vview_f* a, const vsip_vview_f* r);
void vsip_vneg_f(const vsip_vview_f *a, const vsip_vview_f *r);
void vsip_vsdiv_f(const vsip_vview_f *a, vsip_scalar_f beta,const vsip_vview_f *r);
void vsip_vsinh_f(vsip_vview_f const *a,vsip_vview_f const *r);
void (vsip_vsmax_f)( const vsip_vview_f* A,const vsip_scalar_f  b,const vsip_vview_f* R);
void vsip_vtanh_f(vsip_vview_f const *a,vsip_vview_f const *r);
//2023-12-15
vsip_scalar_f (vsip_msumsqval_f)(const vsip_mview_f *r);
void (vsip_cmsub_f)(const vsip_cmview_f *a,const vsip_cmview_f *b,const vsip_cmview_f *r);
vsip_scalar_f (vsip_cmmeansqval_f)(const vsip_cmview_f *a);
//2023.12.18
void (vsip_msub_f)(const vsip_mview_f *a,const vsip_mview_f *b,const vsip_mview_f *r);
vsip_cvview_f* vsip_cvmcopy_f( vsip_cmview_f* v, vsip_index i, vsip_cvview_f *a);
vsip_scalar_f (vsip_mmeansqval_f)(const vsip_mview_f *r);

//2024.04.24
typedef enum
{VSIP_SORT_BYVALUE = 0,VSIP_SORT_BYMAGNITUDE = 1} vsip_sort_mode;
typedef enum
{VSIP_SORT_ASCENDING = 0,VSIP_SORT_DESCENDING = 1} vsip_sort_dir;
void vsip_vsortip_f(const vsip_vview_f *in_out,vsip_sort_mode mode,
                  vsip_sort_dir dir,vsip_bool fill,const vsip_vview_vi *vi0);
void (vsip_vcopyfrom_user_f)(vsip_scalar_f* const r,const vsip_vview_f* a);
//void sort_f(void);
void vsip_mcmaxmgsq_f(const vsip_cmview_f *a, const vsip_cmview_f *b, const vsip_cmview_f *r);
//2024.6.7
vsip_scalar_f vsip_mmeanval_f(const vsip_mview_f *r);

//2024.7.25
vsip_scalar_vi  (vsip_mcolindex)( vsip_scalar_mi mi);
void (vsip_MATINDEX)(vsip_scalar_vi r,vsip_scalar_vi c,vsip_scalar_mi *mi);
vsip_scalar_mi (vsip_matindex)( vsip_scalar_vi r, vsip_scalar_vi c);
vsip_scalar_vi  (vsip_mrowindex)( vsip_scalar_mi mi);

#ifdef _cplusplus
}
#endif

#endif

