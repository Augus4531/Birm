#ifndef _vsip_cblockobject_d_h
#define _vsip_cblockobject_d_h 1


#include"vsip.h"
#include"vsip_blockobject_d.h"

struct vsip_cblockobject_d {
  vsip_block_d *Re,
               *Im;            /* blocks holding data array                */
  int           kind;         /* 0 ==> private, 1 ==> public              */
  int           admit;        /* 0 ==> No,      1 ==> Yes                 */
  vsip_stride   cstride;      /* 1 for split, 2 for interleaved           */
  vsip_length   size;         /* block size in elements                   */
  int           bindings;     /* reference counter                        */
  int           markings;     /* valid|destoyed block object              */
  vsip_memory_hint hint;       /* not used in this implementation */
  #if defined(VSIP_ALWAYS_INTERLEAVED) 
     vsip_scalar_d *data;  /* created data                              */
     vsip_scalar_d *Rp;    /* pointer to user data; real or interleaved */
     vsip_scalar_d *Ip;    /* pointer to split imaginary user data      */
  #elif defined(VSIP_ALWAYS_SPLIT)
     vsip_scalar_d *r_data;  /* created data                              */
     vsip_scalar_d *i_data;  /* created data                              */
     vsip_scalar_d *Rp;      /* pointer to user data; real or interleaved */
     vsip_scalar_d *Ip;      /* pointer to split imaginary user data      */
  #endif
  vsip_cscalar_d a_scalar;
  vsip_cscalar_d a_zero;
  vsip_cscalar_d a_one;
  vsip_cscalar_d a_imag_one;
  vsip_scalar_bl update;          /* a place to store update flag */
  };
#endif /* _vsip_cblockobject_d_h */

