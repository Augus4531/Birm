#include <string.h>
#include "birm_arm_float_User.h"

int birm_vmov_sf(const float * __restrict x, const int sa, const int nx, float * __restrict y, const int sb) {
    if (!x || !y) return -1;
    if (nx <= 0 || sa <= 0 || sb <= 0) return -2;

    // Case 1: Pure memcpy
    if (sa == 1 && sb == 1) {
        memcpy(y, x, nx * sizeof(float));
        return 0;
    }

    // Case 2: Contiguous Input (Gather read not needed, just Scatter write)
    if (sa == 1) {
        for (int i = 0; i < nx; i++) {
            y[i * sb] = x[i];
        }
        return 0;
    }

    // Case 3: Contiguous Output (Gather read, Sequential write)
    if (sb == 1) {
        for (int i = 0; i < nx; i++) {
            y[i] = x[i * sa];
        }
        return 0;
    }

    // Case 4: General Strided Copy
    // We use pointers to avoid complex indexing math (i * sb) inside the loop.
    const float *src = x;
    float *dst = y;
    int n = nx;

    // Prefetch distances (tunable)
    // const int pld_dist = 64; 

    // Unroll 4 times is usually the sweet spot for instruction cache vs throughput
    // on irregular strides, as it balances register pressure.
    while (n >= 4) {
        float v0 = *src; src += sa;
        float v1 = *src; src += sa;
        float v2 = *src; src += sa;
        float v3 = *src; src += sa;

        *dst = v0; dst += sb;
        *dst = v1; dst += sb;
        *dst = v2; dst += sb;
        *dst = v3; dst += sb;

        n -= 4;
    }

    while (n > 0) {
        *dst = *src;
        src += sa;
        dst += sb;
        n--;
    }

    return 0;
}
