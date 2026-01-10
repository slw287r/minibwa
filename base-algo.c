#include <string.h>
#include "bwt.h"
#include "minibwa.h"
#include "kalloc.h"

static int32_t mb_bwt_seed_last(const mb_bwt_t *bwt, int32_t len, const uint8_t *q, int32_t x, int32_t min_len, int32_t max_intv, mb_sai_t *p)
{
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

void mb_seed_intv(void *km, const mb_seedopt_t *opt, const mb_bwt_t *bwt, int32_t len, const uint8_t *seq, mb_sai_v *v)
{
	int64_t x = 0, i, n_a0;
	mb_sai_t p;

	v->n = 0;
	do {
		x = mb_bwt_smem(bwt, len, seq, x, opt->min_len, 1, 1, &p);
		if (p.size > 0) {
			Kgrow(km, mb_sai_t, v->a, v->n, v->m);
			v->a[v->n++] = p;
		}
	} while (x < len);

	n_a0 = v->n;
	for (i = 0; i < n_a0; ++i) {
		uint32_t st = v->a[i].info>>32, en = (uint32_t)v->a[i].info;
		int32_t min_len;
		if (en - st < opt->min_len * 2 || v->a[i].size > opt->max_sub_occ)
			continue;
		x = st;
		min_len = (en - st + 2) / 3;
		if (min_len < opt->min_len) min_len = opt->min_len;
		do {
			#if 1
			x = mb_bwt_seed_last(bwt, en, seq, x, opt->min_len, opt->max_sub_occ * 2, &p);
			#else
			x = mb_bwt_smem(bwt, en, seq, x, min_len, v->a[i].size + 1, opt->max_sub_occ * 2, &p);
			#endif
			if (p.size > v->a[i].size) {
				Kgrow(km, mb_sai_t, v->a, v->n, v->m);
				v->a[v->n++] = p;
			}
		} while (x < en);
	}
}
