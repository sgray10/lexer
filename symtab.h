#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <stdlib.h>

struct symbol_t {
    char *key;
    void *value;
    struct symbol_t *next;
};

struct symbol_table_t {
    size_t size;
    struct symbol_t **table;
};

struct symbol_table_t *symbol_table_new(size_t size);
void symbol_table_free(struct symbol_table_t *st);
void *symbol_table_lookup(struct symbol_table_t *st, char *key);
void symbol_table_insert(struct symbol_table_t *st, char *key, void *value);
void symbol_table_delete(struct symbol_table_t *st, char *key);

#endif
