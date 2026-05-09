#ifndef _vsip_vviewobject_bl_h
#define _vsip_vviewobject_bl_h 1

/*
 * vsip_vviewattributes_bl.h
 *
 * This file contains the PRIVATE vsip_vviewattributes_bl definition.
 */

#include <vsip_blockobject_bl.h>
#include "vsip.h"

struct vsip_vviewobject_bl {
  vsip_block_bl* block;        /* memory data block object */
  vsip_offset    offset;
  vsip_stride    stride;
  vsip_length    length;
  int            markings;     /* valid|destoyed vview object */
  };
#endif /* _vsip_vviewobject_bl_h */
