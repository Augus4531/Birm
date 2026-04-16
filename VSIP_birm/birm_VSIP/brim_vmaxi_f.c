#include "birm_arm_float_User.h"
#include <vsip.h>

int birm_vmaxi_f(const float *x, const int nx, int *id, float *y)
{
    if (!x || !id || !y)
        return birmParamNullError;
    if (nx <= 0)
        return birmParamLengthInvalidError;

    vsip_block_f *blk_x = vsip_blockbind_f((float *)x, nx, VSIP_MEM_NONE);
    vsip_vview_f *vw_x = vsip_vbind_f(blk_x, 0, 1, nx);

    vsip_blockadmit_f(blk_x, VSIP_TRUE);

    vsip_index idx;
    // vmaxtval_f 返回最大值，同时将索引写入 idx 指针位置
    *y = vsip_vmaxval_f(vw_x, &idx);
    *id = (int)idx;

    vsip_vdestroy_f(vw_x);
    vsip_blockrelease_f(blk_x, VSIP_TRUE);
    vsip_blockdestroy_f(blk_x);

    return birmSuccess;
}