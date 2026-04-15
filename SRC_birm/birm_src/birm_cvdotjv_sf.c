#include "birm_arm_float_User.h"
#include "armpl.h"

int birm_cvdotjv_sf(const float *x1, const int sa, const float *x2, const int sb, const int nx, float *y)
{
    if (!x1 || !x2 || !y)
    {
        return birmParamNullError;
    }
    if (nx <= 0 || sa <= 0 || sb <= 0)
    {
        return birmParamLengthInvalidError;
    }

    armpl_singlecomplex_t *_y = (armpl_singlecomplex_t *)y;
    _y[0] = cdotc_(&nx, (armpl_singlecomplex_t *)x2, &sa, (armpl_singlecomplex_t *)x1, &sb);
    return birmSuccess;
}
