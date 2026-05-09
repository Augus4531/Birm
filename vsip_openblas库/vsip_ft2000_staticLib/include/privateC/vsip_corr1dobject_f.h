#ifndef _vsip_corr1dobject_f_h
#define vsip_corr1dobject_f_h 1

#include "vsip.h"


struct vsip_corr1dobject_f{
     vsip_cvview_f  *h;
     vsip_cvview_f  *x;
     vsip_fft_f     *fft;
     vsip_length n;
     vsip_length m;
     vsip_length mn;
     vsip_length N;
     vsip_length lag_len;
     int ntimes;
     vsip_support_region support;
     vsip_alg_hint hint;
};

#endif /* _vsip_corr1dobject_f */
