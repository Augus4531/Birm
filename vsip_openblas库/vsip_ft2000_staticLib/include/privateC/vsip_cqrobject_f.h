
/* Created RJudd August 28, 1999 */
/* SPAWARSYSCEN D881 */
/* private attributes for real qrd */
#ifndef _vsip_cqrobject_f_h
#define _vsip_cqrobject_f_h 1

#include<vsip_cmviewobject_f.h>
#include<vsip_cvviewobject_f.h>
#include<vsip_mviewobject_f.h>
#include<vsip_vviewobject_f.h>



struct vsip_cqrobject_f{

     vsip_qrd_qopt qopt;
     vsip_length M;
     vsip_length N;
     vsip_cmview_f *A;
     vsip_cmview_f AA;
     vsip_cvview_f *v;
     vsip_cvview_f *w;
     vsip_cvview_f *cI;
     vsip_scalar_f *beta;
};

#endif /*_vsip_cqrobject_f_h */
