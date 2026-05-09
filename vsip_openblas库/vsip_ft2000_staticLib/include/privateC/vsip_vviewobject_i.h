#ifndef _vsip_vviewobject_i_h
#define _vsip_vviewobject_i_h 1
/*
 * vsip_vviewobject_d.h
 *
 * This file contains the PRIVATE vsip_vviewattributes_d definition.
*/

#include <vsip_blockobject_i.h>
#include "vsip.h"
#include "XDU.h"

struct vsip_vviewobject_i {
  vsip_block_i*	block;		/* memory data block object	*/
  vsip_offset		offset;
  vsip_stride		stride;
  vsip_length		length;
  int			markings;	/* valid|destoyed vview object	*/
  };
#endif /* _vsip_vviewobject_i_h */
