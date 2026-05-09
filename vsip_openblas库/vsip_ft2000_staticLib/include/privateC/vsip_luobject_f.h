
#ifndef _vsip_luobject_f_h
#define _vsip_luobject_f_h 1
#include <vsip_mviewobject_f.h>
#include <vsip_vviewobject_f.h>
#include <vsip_vviewobject_vi.h>
#include"vsip.h"

struct vsip_luobject_f
{
	vsip_mview_f* LU;
	vsip_mview_f  LLU;
	vsip_index* P;
	vsip_length N;

};

#endif /*_vsip_luobject_f_h */
