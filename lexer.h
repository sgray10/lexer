#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdlib.h>

#define MAX_LEXICAL_VALUE 256
#define NUM_TOKENS        87

/* kinds of tokens */
enum token_t {
    T_OP_LBRACE,
    T_OP_RBRACE,
    T_OP_LBRACKET,
    T_OP_RBRACKET,
    T_OP_LPAREN,
    T_OP_RPAREN,
    T_OP_PERIOD,
    T_OP_ARROW,
    T_OP_INCREMENT,
    T_OP_DECREMENT,
    T_OP_MINUS,
    T_OP_PLUS,
    T_OP_MULTIPLY,
    T_OP_DIVIDE,
    T_OP_MODULUS,
    T_OP_BITAND,
    T_OP_BITOR,
    T_OP_BITCOMP,
    T_OP_BITSHIFTL,
    T_OP_BITSHIFTR,
    T_OP_BITXOR,
    T_OP_NOT,
    T_OP_LT,
    T_OP_LTE,
    T_OP_GT,
    T_OP_GTE,
    T_OP_EQUALS,
    T_OP_NEQUALS,
    T_OP_AND,
    T_OP_OR,
    T_OP_QUESTION,
    T_OP_ASSIGN,
    T_OP_ASSIGN_MULTIPLY,
    T_OP_ASSIGN_DIVIDE,
    T_OP_ASSIGN_MODULUS,
    T_OP_ASSIGN_PLUS,
    T_OP_ASSIGN_MINUS,
    T_OP_ASSIGN_BITSHIFTL,
    T_OP_ASSIGN_BITSHIFTR,
    T_OP_ASSIGN_BITAND,
    T_OP_ASSIGN_BITXOR,
    T_OP_ASSIGN_BITOR,
    T_OP_COMMA,
    T_OP_HASH,
    T_OP_DHASH,
    T_OP_SEMICOLON,
    T_OP_COLON,
    T_OP_ELLIPSIS,
    T_LIT_CHAR,
    T_LIT_INT,
    T_LIT_FLOAT,
    T_LIT_STR,
    T_LIT_ID,
    T_KW_AUTO,
    T_KW_BREAK,
    T_KW_CASE,
    T_KW_CHAR,
    T_KW_CONST,
    T_KW_CONTINUE,
    T_KW_DEFAULT,
    T_KW_DO,
    T_KW_DOUBLE,
    T_KW_ELSE,
    T_KW_ENUM,
    T_KW_EXTERN,
    T_KW_FLOAT,
    T_KW_FOR,
    T_KW_GOTO,
    T_KW_IF,
    T_KW_INT,
    T_KW_LONG,
    T_KW_REGISTER,
    T_KW_RETURN,
    T_KW_SHORT,
    T_KW_SIGNED,
    T_KW_SIZEOF,
    T_KW_STATIC,
    T_KW_STRUCT,
    T_KW_SWITCH,
    T_KW_TYPEDEF,
    T_KW_UNION,
    T_KW_UNSIGNED,
    T_KW_VOID,
    T_KW_VOLATILE,
    T_KW_WHILE,
    T_EOF,
    T_UNINITIALIZED,
};


struct token_hash_t {
    enum token_t tok;
    char value[MAX_LEXICAL_VALUE];
};

/* break input stream into tokens */
struct lexical_env_t {
    size_t ntokens;                /* number of tokens */
    size_t lineno, lpos;           /* current line number, position in line */
    int cur, prev;                 /* current and previous character */
    enum token_t tok;              /* the current token type */
    char value[MAX_LEXICAL_VALUE]; /* the current lexical value */
    struct file_t *file;           /* our representation of the file */
    struct symbol_table_t *st;     /* identifiers, keywords, literals */
};

struct lexical_env_t *lexical_env_new(void);
void lexical_env_free(struct lexical_env_t *l);
void gettoken(struct lexical_env_t *l);

#endif
