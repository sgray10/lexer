#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filebuf.h"
#include "lexer.h"
#include "symtab.h"

int lexer_nextch(struct lexical_env_t *env);
int lexer_prevch(struct lexical_env_t *env);
int lexer_peekch(struct lexical_env_t *env);
int lexer_readtoch(struct lexical_env_t* env, int match);
int lexer_match(struct lexical_env_t* env, char *substring);
void lexer_make_token_literal(struct lexical_env_t *e, char *k, enum token_t t);
void lexer_keywords(struct lexical_env_t *env);
int token_match_op(struct lexical_env_t *env, char *s, enum token_t t);
void token_read_operator_multich(struct lexical_env_t *env);
void token_read_operator(struct lexical_env_t *env);
void token_read_number_literal(struct lexical_env_t *env);
void token_read_identifier_literal_or_keyword(struct lexical_env_t *env);
void token_read_string_literal(struct lexical_env_t *env);
void token_read_literal(struct lexical_env_t *env);

struct lexical_env_t *lexical_env_new(void)
{
    struct lexical_env_t *env = NULL;
    env = (struct lexical_env_t *) malloc(sizeof(struct lexical_env_t));
    if (env != NULL){
        env->ntokens = 0;
        env->lineno = 1;
        env->lpos = 0;
        env->cur = env->prev = EOF;
        env->tok = T_UNINITIALIZED;
        env->value[0] = '\0';
        env->file = file_new();
        env->st = symbol_table_new(65536);
        lexer_keywords(env);
    }
    return env;
}

void lexical_env_free(struct lexical_env_t *env)
{
    if (env != NULL) {
        file_free(env->file);
        symbol_table_free(env->st);
        free(env);
    }
}

void lexer_make_token_literal(struct lexical_env_t *e, char *k, enum token_t t)
{
    struct token_hash_t *th = NULL;
    th = (struct token_hash_t *) symbol_table_lookup(e->st, k);
    if (th == NULL) {
        th = (struct token_hash_t *) malloc(sizeof(struct token_hash_t));
        if (th != NULL) {
            strcpy(th->value, k);
            th->tok = t;
            symbol_table_insert(e->st, k, (void *) th);
        }
    }
    else {
        e->tok = th->tok;
        strcpy(e->value, th->value);
    }
}


void lexer_keywords(struct lexical_env_t *env)
{
    lexer_make_token_literal(env, "auto", T_KW_AUTO);
    lexer_make_token_literal(env, "break", T_KW_BREAK);
    lexer_make_token_literal(env, "case", T_KW_CASE);
    lexer_make_token_literal(env, "char", T_KW_CHAR);
    lexer_make_token_literal(env, "const", T_KW_CONST);
    lexer_make_token_literal(env, "continue", T_KW_CONTINUE);
    lexer_make_token_literal(env, "default", T_KW_DEFAULT);
    lexer_make_token_literal(env, "do", T_KW_DO);
    lexer_make_token_literal(env, "double", T_KW_DOUBLE);
    lexer_make_token_literal(env, "else", T_KW_ELSE);
    lexer_make_token_literal(env, "enum", T_KW_ENUM);
    lexer_make_token_literal(env, "extern", T_KW_EXTERN);
    lexer_make_token_literal(env, "float", T_KW_FLOAT);
    lexer_make_token_literal(env, "for", T_KW_FOR);
    lexer_make_token_literal(env, "goto", T_KW_GOTO);
    lexer_make_token_literal(env, "if", T_KW_IF);
    lexer_make_token_literal(env, "int", T_KW_INT);
    lexer_make_token_literal(env, "long", T_KW_LONG);
    lexer_make_token_literal(env, "register", T_KW_REGISTER);
    lexer_make_token_literal(env, "return", T_KW_RETURN);
    lexer_make_token_literal(env, "short", T_KW_SHORT);
    lexer_make_token_literal(env, "signed", T_KW_SIGNED);
    lexer_make_token_literal(env, "sizeof", T_KW_SIZEOF);
    lexer_make_token_literal(env, "static", T_KW_STATIC);
    lexer_make_token_literal(env, "struct", T_KW_STRUCT);
    lexer_make_token_literal(env, "switch", T_KW_SWITCH);
    lexer_make_token_literal(env, "typedef", T_KW_TYPEDEF);
    lexer_make_token_literal(env, "union", T_KW_UNION);
    lexer_make_token_literal(env, "unsigned", T_KW_UNSIGNED);
    lexer_make_token_literal(env, "void", T_KW_VOID);
    lexer_make_token_literal(env, "volatile", T_KW_VOLATILE);
    lexer_make_token_literal(env, "while", T_KW_WHILE);
}

int lexer_nextch(struct lexical_env_t *env)
{
    env->prev = env->cur;
    env->cur = file_nextch(env->file);
    //env->lpos++;
    /* if (env->cur == '\n') { */
    /*     env->lineno++; */
    /*     env->lpos = 0; */
    /* } */
    return env->cur;
}

int lexer_prevch(struct lexical_env_t *env)
{
    size_t cursor = env->file->cursor;
    char *fbuf = env->file->buf;
    if (fbuf != NULL && cursor > 0) {
        env->cur = fbuf[cursor-1];
        env->prev = (cursor > 1) ? fbuf[cursor-2] : EOF;
        env->file->cursor--;
        return env->cur;
    }
    env->cur = env->prev = EOF;
    return env->cur;
}

int lexer_peekch(struct lexical_env_t *env)
{
    int c = EOF;
    if ((c = file_nextch(env->file)) != EOF)
        env->file->cursor--;
    return c;
}

int lexer_readtoch(struct lexical_env_t *env, int match)
{
    int c;
    while ((c = lexer_nextch(env)) != EOF && c != match);
    return c;

}

int lexer_match(struct lexical_env_t *env, char *s)
{
    int cur = env->cur;
    size_t i = 0;
    int old_prev = env->prev, old_cur = env->cur;
    int old_cursor = env->file->cursor;
    for (i = 0; s[i] != '\0'; i++) {
        if (cur != s[i] || cur == EOF) {
            env->prev = old_prev;
            env->cur = old_cur;
            env->file->cursor = old_cursor;
            return 0;
        }
        cur = lexer_nextch(env);
    }
    cur = lexer_prevch(env);
    return 1;
}

int token_match_op(struct lexical_env_t *env, char *s, enum token_t t)
{
    if (lexer_match(env, s) != 0) {
        env->tok = t;
        return 1;
    }
    return 0;
}

void token_read_operator_multich(struct lexical_env_t *env)
{
    int c = env->cur;
    switch (c) {
    case '.':
        if (token_match_op(env, "...", T_OP_ELLIPSIS)) return;
        if (token_match_op(env, ".", T_OP_PERIOD)) return;
        break;
    case '-':

        if (token_match_op(env, "--", T_OP_DECREMENT)) return;
        if (token_match_op(env, "->", T_OP_ARROW)) return;
        if (token_match_op(env, "-=", T_OP_ASSIGN_MINUS)) return;
        if (token_match_op(env, "-", T_OP_MINUS)) return;
        break;
    case '+':
        if (token_match_op(env, "++", T_OP_INCREMENT)) return;
        if (token_match_op(env, "+=", T_OP_ASSIGN_PLUS)) return;
        if (token_match_op(env, "+", T_OP_PLUS)) return;
        break;
    case '*':
        if (token_match_op(env, "*=", T_OP_ASSIGN_MULTIPLY)) return;
        if (token_match_op(env, "*", T_OP_MULTIPLY)) return;
        break;
    case '/':
        if (token_match_op(env, "/=", T_OP_ASSIGN_DIVIDE)) return;
        if (token_match_op(env, "/", T_OP_DIVIDE)) return;
        break;
    case '%':
        if (token_match_op(env, "%=", T_OP_ASSIGN_MODULUS)) return;
        if (token_match_op(env, "%", T_OP_MODULUS)) return;
        break;
    case '&':
        if (token_match_op(env, "&=", T_OP_ASSIGN_BITAND)) return;
        if (token_match_op(env, "&&", T_OP_AND)) return;
        if (token_match_op(env, "&", T_OP_BITAND)) return;
        break;
    case '!':
        if (token_match_op(env, "!=", T_OP_NEQUALS)) return;
        if (token_match_op(env, "!", T_OP_NOT)) return;
        break;
    case '=':
        if (token_match_op(env, "==", T_OP_EQUALS)) return;
        if (token_match_op(env, "=", T_OP_ASSIGN)) return;
        break;
    case '<':
        if (token_match_op(env, "<<=", T_OP_ASSIGN_BITSHIFTL)) return;
        if (token_match_op(env, "<=", T_OP_LTE)) return;
        if (token_match_op(env, "<<", T_OP_BITSHIFTL)) return;
        if (token_match_op(env, "<", T_OP_LT)) return;
        break;
    case '>':
        if (token_match_op(env, ">>=", T_OP_ASSIGN_BITSHIFTR)) return;
        if (token_match_op(env, ">=", T_OP_GTE)) return;
        if (token_match_op(env, ">>", T_OP_BITSHIFTR)) return;
        if (token_match_op(env, ">", T_OP_GT)) return;
        break;
    case '^':
        if (token_match_op(env, "^=", T_OP_ASSIGN_BITXOR)) return;
        if (token_match_op(env, "^", T_OP_BITXOR)) return;
        break;
    case '|':
        if (token_match_op(env, "||", T_OP_OR)) return;
        if (token_match_op(env, "|=", T_OP_ASSIGN_BITOR)) return;
        if (token_match_op(env, "|", T_OP_BITOR)) return;
        break;
    default: break;
    }
}

void token_read_operator(struct lexical_env_t *env)
{
    int cur = env->cur;
    switch(cur) {
    case '{': env->tok = T_OP_LBRACE; break;
    case '}': env->tok = T_OP_RBRACE; break;
    case '[': env->tok = T_OP_LBRACKET; break;
    case ']': env->tok = T_OP_RBRACKET; break;
    case '(': env->tok = T_OP_LPAREN; break;
    case ')': env->tok = T_OP_RPAREN; break;
    case '~': env->tok = T_OP_BITCOMP; break;
    case '?': env->tok = T_OP_QUESTION; break;
    case ':': env->tok = T_OP_COLON; break;
    case ',': env->tok = T_OP_COMMA; break;
    case ';': env->tok = T_OP_SEMICOLON; break;
    default: return token_read_operator_multich(env);
    }
}

void token_read_number_literal(struct lexical_env_t *env)
{
    int c = env->cur;
    size_t i = 0;

    env->tok = T_LIT_INT;
    for (i = 0; c != EOF && isdigit(c); i++) {
        env->value[i] = c;
        c = lexer_nextch(env);
    }
    if (c == '.' && isdigit(lexer_peekch(env))) {
        env->value[i++] = c;
        while ((c = lexer_nextch(env)) != EOF && isdigit(c))
            env->value[i++] = c;
        env->tok = T_LIT_FLOAT;
    }
    env->value[i] = '\0';
    env->file->cursor--;
    lexer_make_token_literal(env, env->value, env->tok);
}

void token_read_identifier_literal_or_keyword(struct lexical_env_t *env)
{
    size_t c, i;;
    env->value[0] = env->cur;
    for(i = 1; (c = lexer_nextch(env))!=EOF && (isalpha(c) || c == '_'); i++)
        env->value[i] = c;
    lexer_prevch(env);
    env->value[i] = '\0';
    env->tok = T_LIT_ID;
    lexer_make_token_literal(env, env->value, env->tok);
}

void token_read_char_literal(struct lexical_env_t *env)
{
    env->value[0] = lexer_nextch(env);
    env->value[1] = '\0';
    lexer_nextch(env);
    env->tok = T_LIT_CHAR;
}

void token_read_string_literal(struct lexical_env_t *env)
{
    int c = EOF;
    size_t i = 0;
    for (i = 0; (c = lexer_nextch(env)) != EOF && c != '\"'; i++) {
        if (env->cur == '\\') {
            switch(lexer_peekch(env)) {
            case '\\': env->value[i] = '\\'; break;
            case '\'': env->value[i] = '\''; break;
            case '\"': env->value[i] = '\"'; break;
            case 'n': env->value[i] = '\n'; break;
            case 'r': env->value[i] = '\r'; break;
            case 't': env->value[i] = '\t'; break;
            default: break;
            }
        }
        else
            env->value[i] = c;
    }
    env->value[i] = '\0';
    env->tok = T_LIT_STR;
    lexer_make_token_literal(env, env->value, env->tok);
}

void gettoken(struct lexical_env_t *env)
{
    int c;
    env->tok = T_UNINITIALIZED;
    while ((c = lexer_nextch(env)) != EOF) {
        if (isalpha(c) || c == '_')
            return token_read_identifier_literal_or_keyword(env);
        else if (c == '\"')
            return token_read_string_literal(env);
        else if (c == '\'')
            return token_read_char_literal(env);
        else if (isdigit(c))
            return token_read_number_literal(env);
        else {
            switch (c) {
            case '\'': return token_read_char_literal(env);
            case '\"': return token_read_string_literal(env);
            case ' ': case '\t':  case '\n': break;
            case '#': lexer_readtoch(env, '\n'); break;
            default: return token_read_operator(env);
            }
        }
    }
    env->tok = T_EOF;
}
