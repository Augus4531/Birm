#ifndef _vsip_vviewobject_mi_h
#define _vsip_vviewobject_mi_h 1
/*
 *  vsip_vviewattributes_mi.h
 *
 * This file contains the PRIVATE vsip_vviewattributes_mi definition.
*/

#include <vsip_blockobject_mi.h>
#include "vsip.h"
#include "XDU.h"

struct vsip_vviewobject_mi {
  vsip_block_mi*	block;		/* memory data block object	*/
  vsip_offset		offset;
  vsip_stride		stride;
  vsip_length		length;
  int			markings;	/* valid|destoyed vview object	*/
  };
#endif /* _vsip_vviewobject_mi_h */
