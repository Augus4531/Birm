#ifndef _vsip_blockattributes_vi_h
#define _vsip_blockattributes_vi_h 1

/*
 * vsip_blockattributes_vi.h
 *
 * This file contains the PRIVATE vsip_blockattributes_vi definition.
 */

#include "vsip.h"

struct vsip_blockobject_vi {
  vsip_scalar_vi*  array;       /* external data array              */
  int              kind;        /* 0 ==> private, 1 ==> public      */
  int              admit;       /* 0 ==> No,      1 ==> Yes         */
  vsip_length      size;        /* block size in elements           */
  int              bindings;    /* reference counter                */
  int              markings;    /* valid|destoyed block object      */
  vsip_memory_hint hint;        /* not used in this implementation  */
  vsip_scalar_bl update;          /* a place to store update flag */
  };
#endif /* _vsip_blockattributes_vi_h */

