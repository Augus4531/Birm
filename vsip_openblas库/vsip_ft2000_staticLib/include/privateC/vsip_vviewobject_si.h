#ifndef _vsip_vviewobject_si_h
#define _vsip_vviewobject_si_h 1
/*
 * vsip_vviewattributes_si.h
 *
 * This file contains the PRIVATE vsip_vviewattributes_d definition.
*/

#include "vsip.h"
#include "XDU.h"
#include "vsip_blockobject_si.h"

struct vsip_vviewobject_si {
  vsip_block_si*	block;		/* memory data block object	*/
  vsip_offset		offset;
  vsip_stride		stride;
  vsip_length		length;
  int			markings;	/* valid|destoyed vview object	*/
  };
#endif /* _vsip_vviewobject_si_h */
