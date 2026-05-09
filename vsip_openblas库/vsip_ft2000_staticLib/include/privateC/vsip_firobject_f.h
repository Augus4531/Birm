#ifndef _vsip_firobject_f_h
#define _vsip_firobject_f_h 1

#include "vsip.h"


struct vsip_firobject_f{
     vsip_vview_f  *h;
     vsip_vview_f  *s;
     vsip_length N;
     vsip_length M;
     vsip_length p;
     vsip_length D;
     int ntimes;
     vsip_symmetry symm;
     vsip_alg_hint hint;
     vsip_obj_state state;
};
#endif /* _vsip_firobject_f_h */
