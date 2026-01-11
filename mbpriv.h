#ifndef MBPRIV_H
#define MBPRIV_H

#include "minibwa.h"
#include "l2bit.h"
#include "bwt.h"

#define MB_DBG_NO_KALLOC    (0x1LL)

struct mb_idx_s {
	l2b_t *l2b;
	mb_bwt_t *bwt;
};

#ifdef __cplusplus
extern "C" {
#endif

// defined in bwtgen.c
void mb_bwtgen(const char *fn_pac, const char *fn_bwt, int block_size);

// defined in base-algo.c
void mb_seed_intv(void *km, const mb_bwt_t *bwt, int32_t len, const uint8_t *seq, int32_t min_len, int32_t max_sub_occ, mb_sai_v *v);

#ifdef __cplusplus
}
#endif

#endif
