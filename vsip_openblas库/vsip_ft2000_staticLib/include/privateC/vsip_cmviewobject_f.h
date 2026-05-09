#ifndef _vsip_cmviewobject_f_h
#define _vsip_cmviewobject_f_h 1
/*
 *  vsip_cmviewattributes_f.h
 *
 * This file contains the PRIVATE vsip_cmviewattributes_f
// definition.
*/

#include <vsip_cblockobject_f.h>
#include "vsip.h"


struct vsip_cmviewobject_f {
  vsip_cblock_f* block;        /* memory data block object */
  vsip_offset    offset;
  vsip_stride    row_stride;
  vsip_length    row_length;
  vsip_stride    col_stride;
  vsip_length    col_length;
  int            markings;     /* valid|destoyed mview object */
  };
#endif /* _vsip_cmviewobject_f_h */
