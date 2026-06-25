#ifndef iZLIB_H
#define iZLIB_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <igzip_lib.h>

// exported from zlib.h
#ifndef Z_OK
#define Z_OK            0
#endif

#ifndef Z_ERRNO
#define Z_ERRNO        (-1)
#endif

#ifndef Z_STREAM_ERROR
#define Z_STREAM_ERROR (-2)
#endif

#ifndef UNIX
#define UNIX 3
#endif

#ifndef BUF_SIZE
#define BUF_SIZE (1<<22)
#endif

#ifndef HDR_SIZE
#define HDR_SIZE (1<<16)
#endif

#ifndef MIN_COM_LVL
#define MIN_COM_LVL 0
#endif

#ifndef MAX_COM_LVL
#define MAX_COM_LVL 3
#endif

#ifndef COM_LVL_DEFAULT
#define COM_LVL_DEFAULT 3 // was 2
#endif

typedef struct
{
	FILE *fp;
	int fd;
	char *mode;
	int is_plain;
	struct isal_gzip_header *gzip_header;
	struct inflate_state *state;
	struct isal_zstream *zstream;
	uint8_t *buf_in;
	size_t buf_in_size;
	uint8_t *buf_out;
	size_t buf_out_size;
	char *buf_get;
	size_t buf_get_size;
	int64_t buf_get_len;
	int64_t buf_get_out;
} gzFile_t;

typedef gzFile_t* gzFile;

#ifdef __cplusplus
extern "C" {
#endif
int is_gz(FILE* fp);
int is_plain(FILE* fp);
uint32_t get_posix_filetime(FILE* fp);
int ingest_gzip_header(gzFile fp);
gzFile gzopen(const char *in, const char *mode);
gzFile gzdopen(int fd, const char *mode);
int gzread(gzFile fp, void *buf, size_t len);
char* gzgets(gzFile fp, char *buf, int len);
int gzwrite(gzFile fp, const void *buf, size_t len);
int gzprintf(gzFile fp, const char *format, ...);
int gzputc(gzFile fp, int c);
int gzputs(gzFile fp, const char *s);
int gzeof(gzFile fp);
int64_t gzoffset(gzFile fp);
int set_compress_level(gzFile fp, int level);
int gzclose(gzFile fp);
#ifdef __cplusplus
}
#endif
#endif
