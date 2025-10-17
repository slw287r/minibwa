#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void mb_panic(const char *fn, const char *msg)
{
	fprintf(stderr, "[E::%s] %s ABORT!\n", fn, msg);
	abort();
}

uint64_t mb_read_huge(FILE *fp, uint64_t size, void *ptr)
{ // Mac/Darwin has a bug when reading data longer than 2GB. This function fixes this issue by reading data in small chunks
	const int bufsize = 0x1000000; // 16M block
	uint64_t offset = 0;
	uint8_t *a = (uint8_t*)ptr;
	while (size) {
		int x = bufsize < size? bufsize : size;
		if ((x = fread(a + offset, 1, x, fp)) == 0) break;
		size -= x; offset += x;
	}
	return offset;
}
