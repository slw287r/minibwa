#ifndef L2BIT_H
#define L2BIT_H

#include <stdint.h>

#define L2B_MAGIC "L2B\1"

typedef struct {
	char *name, *comm;
	uint64_t len, off;
} l2b_ctg_t;

typedef struct {
	uint64_t st, en;
} l2b_intv_t;

typedef struct {
	uint64_t tot_len;
	uint64_t n_ctg, m_ctg;
	l2b_ctg_t *ctg;
	uint64_t n_pac, m_pac;
	uint64_t n_ambi, m_ambi;
	uint64_t n_mask, m_mask;
	l2b_intv_t *ambi, *mask;
	uint64_t *pac;
	char *cat_name, *cat_comm;
} l2b_t;

#ifdef __cplusplus
extern "C" {
#endif

l2b_t *l2b_load(const char *fn);
void l2b_destroy(l2b_t *l2b);
int64_t l2b_intv2cid(const l2b_t *l2b, uint64_t st, uint64_t en, int64_t *cst, int *rev);
int64_t l2b_getseq(const l2b_t *l2b, int64_t tid, int64_t st, int64_t en, uint8_t *seq);
int64_t l2b_getambi(const l2b_t *l2b, int64_t tid, int64_t st, int64_t en, int32_t *n_ambi);

l2b_t *l2b_import(const char *fn, uint64_t seed);
int l2b_save(const char *fn, const l2b_t *l2b);
int l2b_save_pac(const char *fn, const l2b_t *l2b, int both_strand);

static inline int l2b_get0(const l2b_t *l2b, uint64_t i)
{
	return l2b->pac[i>>5] >> ((i&31)<<1) & 3;
}

static inline void l2b_seq_prefetch(const l2b_t *l2b, int64_t tid, int64_t st)
{
	if (tid >= 0 && tid < l2b->n_ctg && st >= 0 && st < l2b->ctg[tid].len)
		__builtin_prefetch(&l2b->pac[(st + l2b->ctg[tid].off) >> 5]);
}

#ifdef __cplusplus
}
#endif

#endif
