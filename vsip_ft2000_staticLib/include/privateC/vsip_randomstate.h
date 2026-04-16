#ifndef _vsip_randomstate_h
#define _vsip_randomstate_h 1

#include "vsip.h"

struct vsip_randomstate{
     vsip_scalar_ue32  a; /* multiplier in LCG */
     vsip_scalar_ue32  c; /* adder in LCG */
     vsip_scalar_ue32  a1;
     vsip_scalar_ue32  c1;
     vsip_scalar_ue32  X; /* Last or initial X */
     vsip_scalar_ue32  X1;
     vsip_scalar_ue32  X2;
     
     int               type;
     double out_d;
     	 void *stream; /*mkl*/
};
#endif /* _vsip_randomstate_h */
