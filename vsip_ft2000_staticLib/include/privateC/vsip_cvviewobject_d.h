#ifndef _vsip_cvviewobject_d_h
#define _vsip_cvviewobject_d_h 1

#include "vsip_cblockobject_d.h"
#include "vsip.h"

struct vsip_cvviewobject_d
{
  vsip_cblock_d*	block;		/* memory data block object	*/
  vsip_offset		offset;
  vsip_stride		stride;
  vsip_length		length;
  int			markings;	/* valid|destoyed vview object	*/
};
#endif /* _vsip_cvviewobject_d_h */
