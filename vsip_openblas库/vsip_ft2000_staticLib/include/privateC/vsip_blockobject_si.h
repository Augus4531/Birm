#ifndef _vsip_blockobject_si_h
#define _vsip_blockobject_si_h 1
/*
 * vsip_blockattributes_d.h
 *
 * This file contains the PRIVATE vsip_blockattributes_d definition.
*/

#include "vsip.h"


struct vsip_blockobject_si {
  vsip_scalar_si*  array;       /* external data array         */
  int              kind;        /* 0 ==> private, 1 ==> public */
  int              admit;       /* 0 ==> No,      1 ==> Yes    */
  vsip_length      size;        /* block size in elements      */
  int              bindings;    /* reference counter           */
  int              markings;    /* valid|destoyed block object */
  vsip_memory_hint hint;        /* Not used in this implementation */
  vsip_scalar_bl update;          /* a place to store update flag */
  };
#endif /* _vsip_blockobject_si_h */
