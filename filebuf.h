#ifndef __FILEBUF_H__
#define __FILEBUF_H__

#include <stdlib.h>

/* My own file, for fun and profit */
struct file_t {
    size_t cursor;            /* current position of cursor in file */
    size_t sz;                /* size of the file in bytes */
    char *buf;                /* the character buffer (file contents) */
};

struct file_t *file_new(void);
void file_free(struct file_t *f);
void file_read(struct file_t *f);
int file_getch(struct file_t *f);
int file_nextch(struct file_t *f);

#endif
