#ifndef _vsip_cblockobject_f_h
#define _vsip_cblockobject_f_h 1
/*
 * vsip_blockattributes_f.h
 *
 * This file contains the PRIVATE vsip_blockattributes_f definition.
 */

#include <vsip_blockobject_f.h>
#include "vsip.h"


struct vsip_cblockobject_f {
  vsip_block_f *Re;
  vsip_block_f *Im;            /* blocks holding data arrays      */
  int           kind;         /* 0 ==> private, 1 ==> public     */
  int           admit;        /* 0 ==> No,      1 ==> Yes        */
  vsip_stride   cstride;      /* 1 for split, 2 for interleaved  */
  vsip_length   size;         /* block size in elements          */
  int           bindings;     /* reference counter               */
  int           markings;     /* valid|destoyed block object     */
  vsip_memory_hint hint;       /* not used in this implementation */
  #if defined(VSIP_ALWAYS_INTERLEAVED)
     vsip_scalar_f *data; /* created data                              */
     vsip_scalar_f *Rp;   /* pointer to user data; real or interleaved */
     vsip_scalar_f *Ip;   /* pointer to split imaginary user data      */
  #elif defined(VSIP_ALWAYS_SPLIT)
     vsip_scalar_f *r_data; /* created data                              */
     vsip_scalar_f *i_data; /* created data                              */
     vsip_scalar_f *Rp;     /* pointer to user data; real or interleaved */
     vsip_scalar_f *Ip;     /* pointer to split imaginary user data      */
  #endif
  vsip_cscalar_f a_scalar;
  vsip_cscalar_f a_zero;
  vsip_cscalar_f a_one;
  vsip_cscalar_f a_imag_one;
  vsip_scalar_bl update;          /* a place to store update flag */
  };
#endif /* _vsip_cblockobject_f_h */

