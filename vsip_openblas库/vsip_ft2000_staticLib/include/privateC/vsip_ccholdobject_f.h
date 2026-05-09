#ifndef XDU_CCHOLDOBJECT_F_H
#define XDU_CCHOLDOBJECT_F_H

#include <vsip_cmviewobject_f.h>
#include "vsip.h"

struct vsip_ccholdobject_f{
       const vsip_cmview_f* matrix;
       vsip_length N;
       vsip_mat_uplo uplo;
       };

#endif /* XDU_CCHOLDOBJECT_F_H */
