#include <stdio.h>

#include "filebuf.h"

#define MAX_READ_SIZE 8192

struct file_t *file_new(void)
{
    struct file_t *file = NULL;
    file = (struct file_t *) malloc(sizeof(struct file_t));
    if (file != NULL) {
        file->cursor = file->sz = 0;
        file->buf = NULL;
        return file;
    }
    return file;
}

void file_free(struct file_t *fp)
{
    if (fp != NULL) {
        if (fp->buf != NULL)
            free(fp->buf);
        free(fp);
    }
}

void file_read(struct file_t *file)
{
    int i = 0, c = EOF;
    file->buf = (char *) malloc(MAX_READ_SIZE*sizeof(char));
    for (i = 0; i < MAX_READ_SIZE; i++) {
        c = getchar();
        if (c == EOF)
            return;
        file->buf[file->sz] = (char) c;
        file->sz++;
        if (i == (MAX_READ_SIZE - 1)) {
            file->buf = (char *) realloc(file->buf, (file->sz+MAX_READ_SIZE)*sizeof(char));
            i = 0;
        }
    }
}

int file_nextch(struct file_t *file)
{
    if (file->buf == NULL)
        file_read(file);
    if (file->cursor < file->sz)
         return file->buf[file->cursor++];
    return EOF;
}
