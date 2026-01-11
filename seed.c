#include <string.h>
#include "bwt.h"
#include "minibwa.h"
#include "kalloc.h"

static void mb_bwt_extend_eq(const mb_bwt_t *bwt, int32_t len, const uint8_t *q, mb_sai_t *p)
{
	uint32_t st = p->info>>32, en = (uint32_t)p->info;
	int32_t j, c;
	mb_sai_t ok[4];
	for (j = en; j < len; ++j) { // extend forwardly
		if (q[j] > 3) break;
		c = 3 - q[j];
		mb_bwt_extend(bwt, p, ok, 0);
		if (ok[c].size < p->size) break;
		*p = ok[c];
	}
	en = j;
	for (j = st - 1; j >= 0; --j) { // extend backwardly
		if (q[j] > 3) break;
		c = q[j];
		mb_bwt_extend(bwt, p, ok, 1);
		if (ok[c].size < p->size) break;
		*p = ok[c];
	}
	st = j + 1;
	p->info = (uint64_t)st<<32 | en;
}

static int32_t mb_bwt_seed_greedy(const mb_bwt_t *bwt, int32_t len, const uint8_t *q, int32_t x, int32_t min_len, int32_t max_intv, mb_sai_t *p)
{ // inspired by PMID:21209072 but more crude
	int32_t i, c;
	mb_sai_t ik, ok[4];
	memset(p, 0, sizeof(*p));
	if (q[x] > 3) return x + 1;
	mb_bwt_set_intv(bwt, q[x], &ik); // the initial interval of a single base
	for (i = x + 1; i < len; ++i) { // forward search
		if (q[i] < 4) { // an A/C/G/T base
			c = 3 - q[i]; // complement of q[i]
			mb_bwt_extend(bwt, &ik, ok, 0);
			if (ok[c].size < max_intv && i - x >= min_len) {
				*p = ok[c];
				p->info = (uint64_t)x<<32 | (i + 1);
				return i + 1;
			}
			ik = ok[c];
		} else return i + 1;
	}
	return len;
}

void mb_seed_intv(void *km, const mb_bwt_t *bwt, int32_t len, const uint8_t *seq, int32_t min_len, int32_t max_sub_occ, mb_sai_v *v)
{
	int64_t x = 0, i, n_a0;
	mb_sai_t p;

	v->n = 0;
	do {
		x = mb_bwt_smem(bwt, len, seq, x, min_len, 1, 1, &p);
		if (p.size > 0) {
			Kgrow(km, mb_sai_t, v->a, v->n, v->m);
			v->a[v->n++] = p;
		}
	} while (x < len);

	n_a0 = v->n;
	for (i = 0; i < n_a0; ++i) {
		uint32_t st = v->a[i].info>>32, en = (uint32_t)v->a[i].info;
		if (en - st < min_len * 2 || v->a[i].size > max_sub_occ)
			continue;
		x = st;
		do {
			x = mb_bwt_seed_greedy(bwt, en, seq, x, min_len, max_sub_occ * 2, &p);
			if (p.size > v->a[i].size) {
				int32_t to_add = 1;
				if (v->n > 0 && p.size == v->a[v->n-1].size && (uint32_t)p.info <= (uint32_t)v->a[v->n-1].info)
					to_add = 0;
				if (to_add) {
					mb_bwt_extend_eq(bwt, len, seq, &p);
					Kgrow(km, mb_sai_t, v->a, v->n, v->m);
					v->a[v->n++] = p;
				}
			}
		} while (x < en);
	}
}
