#ifndef _XDU_H
#define _XDU_H 1

#ifdef _cplusplus
extern "C" {
#endif

#include "vsip.h"
#include "vsip_cblockobject_f.h"
#include "vsip_cvviewobject_f.h"

#ifndef __VSIP_SCALARS_H_
#define __VSIP_SCALARS_H_

#define VSIP_SINH_F(x) ((vsip_scalar_f) sinh(x))
#define VSIP_SINH_D(x) ((vsip_scalar_d) sinh(x))
#define VSIP_SIN_F(x) ((vsip_scalar_f) sin(x))
#define VSIP_SIN_D(x) ((vsip_scalar_d) sin(x))
#define VSIP_ASIN_F(x) ((vsip_scalar_f) asin(x))
#define VSIP_ASIN_D(x) ((vsip_scalar_d) asin(x))
#define VSIP_COSH_F(x) ((vsip_scalar_f) cosh(x))
#define VSIP_COSH_D(x) ((vsip_scalar_d) cosh(x))
#define VSIP_COS_F(x) ((vsip_scalar_f) cos(x))
#define VSIP_COS_D(x) ((vsip_scalar_d) cos(x))
#define VSIP_ACOS_F(x) ((vsip_scalar_f) acos(x))
#define VSIP_ACOS_D(x) ((vsip_scalar_d) acos(x))
#define VSIP_EXP10_F(x)  ((vsip_scalar_f) pow(10.0 , (x)))
#define VSIP_EXP10_D(x)  ((vsip_scalar_d) pow(10.0 , (x)))
#define VSIP_EXP_F(x)  ((vsip_scalar_f) exp(x))
#define VSIP_EXP_D(x)  ((vsip_scalar_d) exp(x))
#define VSIP_FLOOR_F(x) ((vsip_scalar_f) floor(x))
#define VSIP_FLOOR_D(x) ((vsip_scalar_d) floor(x))
#define VSIP_CEIL_F(x) ((vsip_scalar_f) ceil(x))

#endif //__VSIP_SCALARS_H_

#ifndef _XDU_H
#define _XDU_H 1
#define vsip_valid_structure_object (0x5555)
#define vsip_freed_structure_object (0xAAAA)
#endif /* _XDU__h */
#ifndef VSIP_XDU_H
#define VSIP_XDU_H 1

#define vsip_valid_structure_object (0x5555)
#define VSIP_VALID_STRUCTURE_OBJECT (0x5555)
#define vsip_freed_structure_object (0xAAAA)
#define VSIP_FREED_STRUCTURE_OBJECT (0xAAAA)
#define VSIP_RELEASED_BLOCK (0)
#define VSIP_ADMITTED_BLOCK (1)
#define VSIP_VSIPL_BLOCK    (0)
#define VSIP_USER_BLOCK     (1)
#define VSIP_DERIVED_BLOCK  (2)

#endif

#ifndef __VSIP_SCALARS_H
#define __VSIP_SCALARS_H

#define VSIP_SINH_F(x) ((vsip_scalar_f) sinh(x))
#define VSIP_SINH_D(x) ((vsip_scalar_d) sinh(x))
#define VSIP_SIN_F(x) ((vsip_scalar_f) sin(x))
#define VSIP_SIN_D(x) ((vsip_scalar_d) sin(x))
#define VSIP_ASIN_F(x) ((vsip_scalar_f) asin(x))
#define VSIP_ASIN_D(x) ((vsip_scalar_d) asin(x))
#define VSIP_COSH_F(x) ((vsip_scalar_f) cosh(x))
#define VSIP_COSH_D(x) ((vsip_scalar_d) cosh(x))
#define VSIP_COS_F(x) ((vsip_scalar_f) cos(x))
#define VSIP_COS_D(x) ((vsip_scalar_d) cos(x))
#define VSIP_ACOS_F(x) ((vsip_scalar_f) acos(x))
#define VSIP_ACOS_D(x) ((vsip_scalar_d) acos(x))
#define VSIP_EXP10_F(x)  ((vsip_scalar_f) pow(10.0 , (x)))
#define VSIP_EXP10_D(x)  ((vsip_scalar_d) pow(10.0 , (x)))
#define VSIP_EXP_F(x)  ((vsip_scalar_f) exp(x))
#define VSIP_EXP_D(x)  ((vsip_scalar_d) exp(x))
#define VSIP_FLOOR_F(x) ((vsip_scalar_f) floor(x))
#define VSIP_FLOOR_D(x) ((vsip_scalar_d) floor(x))
#define VSIP_CEIL_F(x) ((vsip_scalar_f) ceil(x))
#define VSIP_CEIL_D(x) ((vsip_scalar_d) ceil(x))

#endif /*vsip_scalars_h*/
/*
 *
 * *********************************函数声明***************************
 *
 */
#ifndef _hs_h
#define _hs_h 1

//vsip_fft_f* vsip_ccfftip_create_f(vsip_length N,vsip_scalar_f scale, vsip_fft_dir dir, unsigned int ntimes,vsip_alg_hint hint);
void init_fft_f(vsip_cvview_f *x,vsip_fft_f* fft);
void  XDU_ccfftip_f(const vsip_fft_f* fft, const vsip_cvview_f *x) ;
//vsip_fftm_f* vsip_ccfftmip_create_f(vsip_length M, vsip_length N, vsip_scalar_f scale, vsip_fft_dir dir, vsip_major major,unsigned int ntimes,vsip_alg_hint hint);
//void vsip_ccfftmip_f(const vsip_fftm_f *Offt, const vsip_cmview_f *y);
//vsip_fftm_f* vsip_ccfftmop_create_f(vsip_length M, vsip_length N, vsip_scalar_f scale, vsip_fft_dir dir, vsip_major major, unsigned int ntimes, vsip_alg_hint hint);
//void vsip_ccfftmop_f(const vsip_fftm_f *Offt, const vsip_cmview_f *x, const vsip_cmview_f *y);
//void vsip_ccfftop_f(const vsip_fft_f *Offt, const vsip_cvview_f *x, const vsip_cvview_f *y);
vsip_cvview_f* XDU_cmcolview_f( const vsip_cmview_f* v,  vsip_index j,  vsip_cvview_f *a) ;
void XDU_cmcopy_f_f(const vsip_cmview_f *a,const vsip_cmview_f *r);
void XDU_cmfill_f(vsip_cscalar_f alpha, const vsip_cmview_f *r);
vsip_cvview_f* XDU_cmrowview_f( const vsip_cmview_f* v,  vsip_index i, vsip_cvview_f *a);
vsip_cmview_f*XDU_cmtransview_f(const vsip_cmview_f* v, vsip_cmview_f * a);
//vsip_fftm_f* vsip_crfftmop_create_f(vsip_length M, vsip_length N,vsip_scalar_f scale, vsip_major major, unsigned int ntimes, vsip_alg_hint hint);
//void vsip_crfftmop_f(const vsip_fftm_f  *Offt, const vsip_cmview_f *Z, const vsip_mview_f  *X);
void XDU_csvmul_f(vsip_cscalar_f alpha, const vsip_cvview_f *b,const vsip_cvview_f *r);
void XDU_cvadd_f(const vsip_cvview_f *a, const vsip_cvview_f *b,const vsip_cvview_f *r);
void XDU_cvcopy_f_f(const vsip_cvview_f* a, const vsip_cvview_f* r);
vsip_cblock_f* XDU_cvdestroy_f(vsip_cvview_f* v);
void XDU_cvfill_f(vsip_cscalar_f alpha, const vsip_cvview_f* r);

void XDU_cvsqrt_f(const vsip_cvview_f *a,const vsip_cvview_f *r);

vsip_scalar_vi  XDU_jofk(vsip_scalar_vi k,  vsip_scalar_vi *pn,vsip_scalar_vi *p0,vsip_scalar_vi pF, vsip_length fn);
vsip_vview_f* XDU_mcolview_f(const vsip_mview_f* v,vsip_index i, vsip_vview_f *a);
void XDU_mcopy_f_f(const vsip_mview_f *a, const vsip_mview_f *r);
vsip_mview_f* XDU_mimagview_f( const vsip_cmview_f* X, vsip_mview_f* Y) ;
vsip_mview_f* XDU_mrealview_f( const vsip_cmview_f* X,  vsip_mview_f* Y);
vsip_vview_f*  XDU_mrowview_f(const vsip_mview_f* v,vsip_index i,vsip_vview_f *a);
vsip_mview_f *XDU_mtransview_f(const vsip_mview_f* v, vsip_mview_f * a) ;
vsip_length XDU_nuV(vsip_length N, vsip_scalar_vi *pn,vsip_scalar_vi *p0,vsip_scalar_vi *pF);
vsip_fftm_f* vsip_rcfftmop_create_f(vsip_length M,vsip_length N, vsip_scalar_f scale,vsip_major major,  unsigned int ntimes,vsip_alg_hint hint);
//void vsip_rcfftmop_f(const vsip_fftm_f *Offt,const vsip_mview_f  *X,const vsip_cmview_f *Z);
void XDU_rcvadd_f(const vsip_vview_f *a,const vsip_cvview_f *b,const vsip_cvview_f *r) ;
void XDU_rcvmul_f(const vsip_vview_f* a, const vsip_cvview_f* b,const vsip_cvview_f* r);
void XDU_rscvmul_f(vsip_scalar_f alpha,  const vsip_cvview_f *b, const vsip_cvview_f *r) ;
void XDU_svmul_f(vsip_scalar_f alpha,const vsip_vview_f *b,const vsip_vview_f *r);
void XDU_svsub_f(vsip_scalar_f alpha,  const vsip_vview_f *b, const vsip_vview_f *r);
void XDU_vacos_f(const vsip_vview_f *a,const vsip_vview_f *r);
void XDU_vclip_f(const vsip_vview_f* a,vsip_scalar_f t1,vsip_scalar_f t2,vsip_scalar_f c1,vsip_scalar_f c2, const vsip_vview_f* r);
void XDU_vcopy_f_f(const vsip_vview_f* a,const vsip_vview_f* r);
void XDU_vcos_f(const vsip_vview_f *a, const vsip_vview_f *r);
vsip_block_f* XDU_vdestroy_f(vsip_vview_f* v);
void XDU_veuler_f(const vsip_vview_f* a,const vsip_cvview_f* r);
void XDU_vfill_f(vsip_scalar_f alpha,const vsip_vview_f* r);
vsip_vview_f* XDU_vimagview_f(const vsip_cvview_f* v, vsip_vview_f*  a);
vsip_vview_f* XDU_vrealview_f(const vsip_cvview_f* v, vsip_vview_f*  a);
vsip_cscalar_f XDU_cvjjdot_f(const vsip_cvview_f *a,const vsip_cvview_f *b);

#endif //_hs_h


#ifndef _XD_H
#define _XD_H 1

void XD_cmprintm_f(char s[], vsip_cmview_f *X);
void XD_cvprintm_d(char s[], vsip_cvview_d *X);
void XD_cvprintm_f(char s[], vsip_cvview_f *X);
void XD_mprintm_f(char s[], vsip_mview_f *X);
void XD_vprintm_f(char s[], vsip_vview_f *X);
void XD_vprintm_i(char s[], vsip_vview_i *X);
void XD_vprintm_mi(char s[], vsip_vview_mi *X);
void XD_vprintm_si(char s[], vsip_vview_si *X);
vsip_length (vsip_vgetlength_si)(const vsip_vview_si* v);
vsip_scalar_si (vsip_vget_si)(const vsip_vview_si *v, vsip_index i);
void XD_vprintm_vi(char s[], vsip_vview_vi *X);

#endif //_XD_H


//#ifndef _XDU_H
//#define _XDU_H 1
#define vsip_valid_structure_object (0x5555)
#define vsip_freed_structure_object (0xAAAA)

#define vsip_valid_structure_object (0x5555)
#define VSIP_VALID_STRUCTURE_OBJECT (0x5555)
#define vsip_freed_structure_object (0xAAAA)
#define VSIP_FREED_STRUCTURE_OBJECT (0xAAAA)
#define VSIP_RELEASED_BLOCK (0)
#define VSIP_ADMITTED_BLOCK (1)
#define VSIP_VSIPL_BLOCK    (0)
#define VSIP_USER_BLOCK     (1)
#define VSIP_DERIVED_BLOCK  (2)
//#endif /* VSIP_XDU__h */

#ifndef XDU_CVGET_F_H
#define XDU_CVGET_F_H 1

/* get and store a value in the internal block scalar storage */
#define XDU_CVGETP_F(v, k) { vsip_offset o = (v)->block->cstride * ((v)->offset + (vsip_stride)(k) * (v)->stride); \
                            (v)->block->a_scalar.r = *((v)->block->Re->array + o); \
                            (v)->block->a_scalar.i = *((v)->block->Im->array + o);}

/* retrieve the value stored in the internal block scalar storage */
#define XDU_CVGET_F(v) ((v)->block->a_scalar)
#endif /* XDU_CVGET_F_H */

#ifndef XDU_CVGET_F_H
#define XDU_CVGET_F_H 1

#include "vsip.h"
#include"vsip_cblockobject_f.h"
#include"vsip_cvviewobject_f.h"

/* get and store a value in the internal block scalar storage */
#define XDU_CVGETP_F(v, k) { vsip_offset o = (v)->block->cstride * ((v)->offset + (vsip_stride)(k) * (v)->stride); \
                            (v)->block->a_scalar.r = *((v)->block->Re->array + o); \
                            (v)->block->a_scalar.i = *((v)->block->Im->array + o);}

/* retrieve the value stored in the internal block scalar storage */
#define XDU_CVGET_F(v) ((v)->block->a_scalar)

#endif /* XDU_CVGET_F_H */

#ifndef XDU_VRAMP_F_H
#define XDU_VRAMP_F_H

#define XDU_vramp_f(x,y,r) { \
   vsip_length N = r->length,i; \
   register vsip_scalar_f start = x; register vsip_scalar_f inc = y;\
   vsip_stride rst = r->stride * r->block->rstride;\
   vsip_scalar_f *rp    = (r->block->array) + r->offset * r->block->rstride;\
   *rp = start;\
   for(i=1; i<N; i++){\
       rp += rst;\
       *rp = start + (vsip_scalar_d)i * inc;\
   }}
#endif //XDU_VRAMP_F_H

#ifndef XDU_VPUT_F_H
#define XDU_VPUT_F_H 1

#include"vsip.h"
#include"vsip_vviewobject_f.h"

#define XDU_VPUT_F(v,i,s) {*(v->block->array + (v->offset + (vsip_stride)(i) * v->stride) * v->block->rstride) = (s);}
#endif /* XDU_VPUT_F_H */

#ifndef XDU_VGET_F_H
#define XDU_VGET_F_H 1

#include "vsip.h"
#include"vsip_blockobject_f.h"
#include"vsip_vviewobject_f.h"

#define XDU_VGET_F(v,i) (*(v->block->array + (v->offset + (vsip_stride)(i) * v->stride) * v->block->rstride))
#endif /* XDU_VGET_F_H */

#ifndef _XDU_functions_f_h
#define _XDU_functions_f_h 1
vsip_vview_f* XDU_vrealview_f(const vsip_cvview_f*, vsip_vview_f*);
vsip_mview_f* XDU_mrealview_f(const vsip_cmview_f*, vsip_mview_f*);
vsip_vview_f* XDU_vimagview_f(const vsip_cvview_f*, vsip_vview_f*);
vsip_mview_f* XDU_mimagXDUew_f(const vsip_cmview_f*, vsip_mview_f*);
vsip_cvview_f* XDU_cmrowview_f(const vsip_cmview_f*,vsip_index,vsip_cvview_f*);
vsip_cvview_f* XDU_cmcolXDUew_f(const vsip_cmview_f*,vsip_index,vsip_cvview_f*);
vsip_vview_f* XDU_mrowview_f(const vsip_mview_f*,vsip_index,vsip_vview_f*);
vsip_vview_f* XDU_mcolview_f(const vsip_mview_f*,vsip_index,vsip_vview_f*);
vsip_mview_f* XDU_mtransview_f(const vsip_mview_f*,vsip_mview_f*);
vsip_cmview_f* XDU_cmtransview_f(const vsip_cmview_f*,vsip_cmview_f*);
//void XDU_rcvacosh_f(vsip_vview_f*,vsip_cvview_f*);
//vsip_scalar_f XDU_acosh_f(vsip_scalar_f);
//void XDU_cvcosh_f(vsip_cvview_f*,vsip_cvview_f*);
//void XDU_vfreqswap_f(vsip_vview_f*);
vsip_cscalar_f XDU_cvjjdot_f(const vsip_cvview_f*,const vsip_cvview_f*);
#endif /* _XDU_functions_f_h */

#ifndef XDU_ftm_f_h
#define XDU_ftm_f_h 1

#define XDU_ftm_f_2PI 6.2831853071795864769252866
#define XDU_ftm_f_PI  3.1415926535897932384626433


/* building blocks */
void XDU_dftm_f(void *tfft);
void XDU_fft2m_f(void *tfft);
void XDU_ifft2m_f(void *tfft);
void XDU_fft3m_f(void *tfft);
void XDU_ifft3m_f(void *tfft);
void XDU_fft4m_f(void *tfft);
void XDU_ifft4m_f(void *tfft);
void XDU_fft5m_f(void *tfft);
void XDU_ifft5m_f(void *tfft);
void XDU_fft7m_f(void *tfft);
void XDU_ifft7m_f(void *tfft);
void XDU_fft8m_f(void *tfft);
void XDU_ifft8m_f(void *tfft);
vsip_fftm_f *XDU_modm_type(vsip_fftm_f* , vsip_fftmtype);
//void XDU_p0pFm_f(vsip_fftm_f*);
void XDU_p0pFm_f(void *tfft);


/* Utility Function to determine If length is a Power of 2, 3, 4 */
/* and return -1 if not and power if true */
/* nu(length) */
vsip_length XDU_nuV_f(vsip_length, vsip_scalar_vi*,vsip_scalar_vi*,vsip_scalar_vi*);

/* Utility Functions for unsorting */
vsip_scalar_vi XDU_jofk_f(vsip_scalar_vi, vsip_scalar_vi *, vsip_scalar_vi *, vsip_scalar_vi,vsip_length);
void XDU_sortm_copy_f(void *tfft);
//void XDU_fft_wtm_f(vsip_fftm_f*);
void XDU_fft_wtm_f(void *tfft);
void XDU_cvgather_f(const vsip_cvview_f*,const vsip_scalar_vi*,const vsip_cvview_f*);
void XDU_ccfftmip_f(const vsip_fftm_f*, const vsip_cmview_f*);
#endif /*XDU_ftm_f.h*/

#ifndef XDU_ft_f_h
#define XDU_ft_f_h 1

#define XDU_ft_f_2PI 6.2831853071795864769252866
#define XDU_ft_f_PI  3.1415926535897932384626433


/* building blocks */
void XDU_dft_f(void *tfft);
void XDU_fft2_f(void *tfft);
void XDU_ifft2_f(void *tfft);
void XDU_fft3_f(void *tfft);
void XDU_ifft3_f(void *tfft);
void XDU_fft4_f(void *tfft);
void XDU_ifft4_f(void *tfft);
void XDU_fft5_f(void *tfft);
void XDU_ifft5_f(void *tfft);
void XDU_fft7_f(void *tfft);
void XDU_ifft7_f(void *tfft);
void XDU_fft8_f(void *tfft);
void XDU_ifft8_f(void *tfft);
vsip_fft_f *XDU_mod_type(vsip_fft_f* , vsip_ffttype);
void XDU_p0pF_f(void *tfft);

/* Utility Function to determine If length is a Power of 2, 3, 4 */
/* and return -1 if not and power if true */
/* nu(length) */
//vsip_length XDU_nuV_f(vsip_length, vsip_scalar_vi*,vsip_scalar_vi*,vsip_scalar_vi*);

/* Utility Functions for unsorting */
//vsip_scalar_vi XDU_jofk_f(vsip_scalar_vi, vsip_scalar_vi *, vsip_scalar_vi *, vsip_scalar_vi,vsip_length);
//void XDU_sort_copy_f(vsip_fft_f*);
void  XDU_sort_copy_f(void* tfft);
void XDU_fft_wt_f(void *tfft);
void XDU_cvgather_f(const vsip_cvview_f*,const vsip_scalar_vi*,const vsip_cvview_f*);
//void XDU_ccfftip_f(const vsip_fft_f*, const vsip_cvview_f*);
#endif /*XDU_ft_f.h*/

//#ifndef _XDU_FFTW_OBJ_H
//#define _XDU_FFTW_OBJ_H
//
//vsipl_fftw_obj *vsipl_fftwobj_create();
//int vsipl_fftwobj_init(vsipl_fftw_obj* *obj, fftw_direction dir, int n, int flags);
//void vsipl_fftwobj_fin(vsipl_fftw_obj *obj);
//void vsipl_cvcopy_to_fftw_f(const vsip_cvview_f *v, vsipl_fftw_obj *obj);
//void vsipl_cvcopy_from_fftw_f(vsipl_fftw_obj *obj,const vsip_cvview_f *v);
//void vsipl_cvcopy_to_fftw_d(const vsip_cvview_d *v, vsipl_fftw_obj *obj);
//void vsipl_cvcopy_from_fftw_d( vsipl_fftw_obj *obj,const vsip_cvview_d *v);
//#endif _XDU_FFTW_OBJ_H

//2024.5.23
void XDU_cvjsubvmprodIP_f(vsip_cvview_f *a,vsip_cmview_f *B,vsip_cvview_f *r);
void XDU_cvsubvmprodIP_f(vsip_cvview_f *a,vsip_cmview_f *B,vsip_cvview_f *r);
void XDU_csolve_ut_a_f(vsip_cmview_f *R,vsip_cscalar_f alpha,vsip_cmview_f *B);
void XDU_csolve_lt_a_f(vsip_cmview_f *R, vsip_cscalar_f alpha,vsip_cmview_f *B);
void XDU_csolve_ut_f(vsip_cmview_f *R,vsip_cmview_f *B);
void XDU_csolve_lt_f(vsip_cmview_f *R,vsip_cmview_f *B);
void XDU_cqrdsolr_f(vsip_cmview_f *R0,vsip_mat_op OpR,vsip_cmview_f *X0);
void XDU_cgivens_f(vsip_cscalar_f a,vsip_cscalar_f b,vsip_cscalar_f *c,vsip_cscalar_f *s,vsip_cscalar_f  *r);
void XDU_cgivens_llsq_f( vsip_cmview_f *A,vsip_cmview_f *B);
void XDU_cgivens_r_f( vsip_cmview_f *A);




#ifdef _cplusplus
}
#endif

#endif /* _XDU_H */
