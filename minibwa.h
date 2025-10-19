#ifndef MINIBWA_H
#define MINIBWA_H

#define MB_VERSION "0.0"

struct mb_idx_s;
typedef struct mb_idx_s mb_idx_t;

#ifdef __cplusplus
extern "C" {
#endif

mb_idx_t *mb_idx_load(const char *prefix);
void mb_idx_destroy(mb_idx_t *idx);

#ifdef __cplusplus
}
#endif

#endif
