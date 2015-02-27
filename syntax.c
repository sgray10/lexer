#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char **argv);

int main(int argc, char **argv)
{
    struct parse_env_t *penv = parse_env_new();
    parse(penv);
    parse_env_free(penv);
    return 0;
}
