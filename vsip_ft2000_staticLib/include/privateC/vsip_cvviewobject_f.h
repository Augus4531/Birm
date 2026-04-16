#ifndef _vsip_cvviewobject_f_h
#define _vsip_cvviewobject_f_h 1
/*
 * vsip_cvviewattributes_f.h
 *
 * This file contains the PRIVATE vsip_cvviewattributes_f  definition.
*/

#include "vsip_cblockobject_f.h"
#include "vsip.h"

struct vsip_cvviewobject_f {
  vsip_cblock_f*	block;		/* memory data block object	*/
  vsip_offset		offset;
  vsip_stride		stride;
  vsip_length		length;
  int			markings;	/* valid|destoyed vview object	*/
  };
#endif /* _vsip_cvviewobject_f_h */
