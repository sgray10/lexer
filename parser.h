#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>

#include "lexer.h"

struct parse_env_t {
    size_t nerrors;              /* number of syntax errors */
    struct lexical_env_t *lenv;  /* current lexical environment */
    struct error_msg_t *errors;  /* syntax errors list */
};

struct parse_env_t *parse_env_new(void);
void parse_env_free(struct parse_env_t *env);
void parse(struct parse_env_t *env);

#endif
