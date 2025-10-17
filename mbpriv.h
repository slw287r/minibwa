#ifndef MBPRIV_H
#define MBPRIV_H

#include "l2bit.h"
#include "bwt.h"

#ifdef __cplusplus
extern "C" {
#endif

// defined in bwtgen.c
void mb_bwtgen(const char *fn_pac, const char *fn_bwt, int block_size);

#ifdef __cplusplus
}
#endif

#endif
