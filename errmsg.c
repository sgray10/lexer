#include <stdlib.h>

#include "errmsg.h"

struct error_msg_t *error_msg_new(void)
{
    struct error_msg_t *err = NULL;
    err = (struct error_msg_t *) malloc(sizeof(struct error_msg_t));
    if (err != NULL) {
        err->lineno = err->pos = err->errno = 0;
        err->next = NULL;
        return err;
    }
    exit(1);
}

void error_msg_free(struct error_msg_t *err)
{
    struct error_msg_t *cur = NULL, *prev = NULL;
    if (err != NULL) {
        cur = prev = err;
        while (cur->next != NULL) {
            cur = cur->next;
            free(prev);
            prev = cur;
        }
        free(cur);
    }
}
