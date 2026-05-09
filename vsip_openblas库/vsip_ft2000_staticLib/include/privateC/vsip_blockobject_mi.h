#ifndef _vsip_blockobject_mi_h
#define _vsip_blockobject_mi_h 1
/* To add the matrix index block */
/*
 * vsip_blockattributes_mi.h
 *
 * This file contains the PRIVATE vsip_blockattributes_mi definition.
  */

#include "vsip.h"

struct vsip_blockobject_mi {
  vsip_scalar_vi*  array;       /* external data array             */
  int              kind;        /* 0 ==> private, 1 ==> public     */
  int              admit;       /* 0 ==> No,      1 ==> Yes        */
  vsip_length      size;        /* block size in elements          */
  int              bindings;    /* reference counter               */
  int              markings;    /* valid|destoyed block object     */
  vsip_memory_hint hint;        /* Not used in this implementation */
  vsip_scalar_bl update;          /* a place to store update flag */
  };
#endif /* _vsip_blockobject_mi_h */

