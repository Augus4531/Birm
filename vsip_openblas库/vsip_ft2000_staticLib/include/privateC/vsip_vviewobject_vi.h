#ifndef _vsip_vviewobject_vi_h
#define _vsip_vviewobject_vi_h 1

/*
 * vsip_vviewattributes_vi.h
 *
 * This file contains the PRIVATE vsip_vviewattributes_vi  definition.
*/

#include <vsip_blockobject_vi.h>
#include "vsip.h"
#include "XDU.h"

struct vsip_vviewobject_vi {
  vsip_block_vi*	block;		/* memory data block object	*/
  vsip_offset		offset;
  vsip_stride		stride;
  vsip_length		length;
  int			markings;	/* valid|destoyed vview object	*/
  };
#endif /* _vsip_vviewobject_vi_h */
