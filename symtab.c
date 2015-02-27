#include <string.h>

#include "symtab.h"

size_t symbol_table_hash(struct symbol_table_t *st, char *key);

struct symbol_table_t *symbol_table_new(size_t size)
{
    size_t i = 0;
    struct symbol_table_t *st = NULL;

    st = (struct symbol_table_t *) malloc(sizeof(struct symbol_table_t));
    if (st == NULL)
        return NULL;

    st->table = (struct symbol_t **) malloc(size*sizeof(struct symbol_t *));
    if (st->table == NULL)
        return NULL;

    for (i = 0; i < size; i++)
        st->table[i] = NULL;
    st->size = size;
    return st;
}

void symbol_table_free(struct symbol_table_t *st)
{
    size_t i = 0;
    if (st == NULL)
        return;
    if (st->table != NULL) {
        for (i = 0; i < st->size; i++) {
            if (st->table[i] != NULL) {
                st->table[i]->key = NULL;
                st->table[i]->value = NULL;
                free(st->table[i]);
            }
        }
        free(st->table);
    }
    free(st);
}

size_t symbol_table_hash(struct symbol_table_t *st, char *key)
{
    size_t hash = 0, i = 0;

    for (hash = i = 0; i < strlen(key); i++) {
        hash += key[i], hash += (hash << 10), hash ^= (hash >> 6);
    }
    hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);
    return hash % st->size;
}

void *symbol_table_lookup(struct symbol_table_t *st, char *key)
{
    size_t i = symbol_table_hash(st, key);
    struct symbol_t *sym = NULL;
    if (key == NULL || st == NULL || st->table == NULL)
        return NULL;
    for (sym = st->table[i]; sym != NULL && sym->key != NULL; sym = sym->next) {
        if (strcmp(key, sym->key) == 0)
            return sym->value;
    }
    return NULL;
}

void symbol_table_insert(struct symbol_table_t *st, char *key, void *value)
{
    size_t i = symbol_table_hash(st, key);
    struct symbol_t *cur = NULL, *prev = NULL, *sym = NULL;
    if (key == NULL || st == NULL || st->table == NULL)
        return;
    prev = cur = st->table[i];
    while (cur != NULL) {
        if (strcmp(key, cur->key) == 0) {
            cur->value = value;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    if ((sym = (struct symbol_t *) malloc(sizeof(struct symbol_t))) != NULL) {
        sym->key = strdup(key);
        sym->value = value;
        sym->next = NULL;
        if (cur == prev)
            st->table[i] = sym;
        else if (prev != NULL)
            prev->next = sym;
    }
}

void symbol_table_delete(struct symbol_table_t *st, char *key)
{
    size_t i = symbol_table_hash(st, key);
    struct symbol_t *cur, *prev = NULL;
    if (key == NULL || st == NULL || st->table == NULL)
        return;
    prev = cur = st->table[i];
    while (cur != NULL) {
        if (strcmp(key, cur->key) == 0) {
            free(cur->key);
            if (prev != cur)
                prev->next = (cur->next != NULL) ? cur->next : NULL;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}
