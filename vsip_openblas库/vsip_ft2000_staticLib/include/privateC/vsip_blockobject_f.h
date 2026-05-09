#ifndef _vsip_blockobject_f_h
#define _vsip_blockobject_f_h 1
/* for real block stride                     */
/*
 * vsip_blockattributes_f.h
 *
 *  This file contains the PRIVATE vsip_blockattributes_f definition.
 */
#include "vsip.h"

struct vsip_blockobject_f {
  vsip_cblock_f*   parent;     /* if derived point to parent                             */
  vsip_scalar_f*   array;      /* external data array                                    */
  int              kind;       /* 0 ==> private, 1 ==> public, 2==> derived              */
  int              admit;      /* 0 ==> No,      1 ==> Yes                               */
  vsip_stride      rstride;    /* real block stride; stride = view_stride * block_stride */
  vsip_length      size;       /* block size in elements                                 */
  int              bindings;   /* reference counter                                      */
  int              markings;   /* valid|destoyed block object                            */
  vsip_memory_hint hint;       /* Not used in this implementation                        */
  vsip_scalar_bl   update;          /* a place to store update flag */
  };
#endif /* _vsip_blockobject_f_h */

