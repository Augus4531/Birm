#ifndef _vsip_mviewobject_f_h
#define _vsip_mviewobject_f_h 1

/*
 * vsip_mviewattributes_f.h
 *
 * This file contains the PRIVATE vsip_mviewattributes_f definition.
*/
#include "vsip.h"
#include <vsip_blockobject_f.h>

struct vsip_mviewobject_f {
  vsip_block_f* block;        /* memory data block object   */
  vsip_offset   offset;
  vsip_stride   row_stride;
  vsip_length   row_length;
  vsip_stride   col_stride;
  vsip_length   col_length;
  int           markings;     /* valid|destoyed mview object */
  };
#endif /* _vsip_mviewobject_f_h */
