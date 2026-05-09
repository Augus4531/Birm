#ifndef XDU_CHOLDOBJECT_F_H
#define XDU_CHOLDOBJECT_F_H

#include <vsip_mviewobject_f.h>
#include "vsip.h"

struct vsip_choldobject_f{
       const vsip_mview_f* matrix;
       vsip_length N;
       vsip_mat_uplo uplo;
       };

#endif /* XDU_CHOLDOBJECT_F_H */
