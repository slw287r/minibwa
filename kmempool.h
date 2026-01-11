#ifndef AC_KMEMPOOL_H
#define AC_KMEMPOOL_H

#ifdef __cplusplus
extern "C" {
#endif

void *kmp_init(unsigned sz);
void *kmp_init2(unsigned sz, unsigned chunk_size);
void *kmp_init3(void *km, unsigned sz, unsigned chunk_size);
void kmp_destroy(void *mp);
void *kmp_alloc(void *mp);
void kmp_free(void *mp, void *p);

#ifdef __cplusplus
}
#endif

#endif
