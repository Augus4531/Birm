#ifndef _vsip_vviewobject_f_h
#define _vsip_vviewobject_f_h 1

/*
 * vsip_vviewattributes_f.h
 *
 * This file contains the PRIVATE vsip_vviewattributes_f definition.
 */

#include <vsip_blockobject_f.h>
#include "vsip.h"

struct vsip_vviewobject_f {
  vsip_block_f* block;        /* memory data block object */
  vsip_offset   offset;
  vsip_stride   stride;
  vsip_length   length;
  int           markings;     /* valid|destoyed vview object */
  };
#endif /* _vsip_vviewobject_f_h */
