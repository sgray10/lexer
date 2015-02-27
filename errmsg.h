#ifndef __ERRMSG_H__

struct error_msg_t {
    int lineno, pos;          /* line number and character position */
    int errno;                /* error msg number */
    struct error_msg_t *next; /* the next error msg in the file */
};

struct error_msg_t *error_msg_new(void);
void error_msg_free(struct error_msg_t *errmsg);

#define __ERRMSG_H__
#endif
