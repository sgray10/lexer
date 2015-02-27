#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
struct token_str_t {
    char name[32];
    char value[32];
};

struct token_str_t token_strings[NUM_TOKENS] = {
    { .name = "T_OP_LBRACE", .value = "{" },
    { .name = "T_OP_RBRACE", .value = "}" },
    { .name = "T_OP_LBRACKET", .value = "[" },
    { .name = "T_OP_RBRACKET", .value = "]" },
    { .name = "T_OP_LPAREN", .value = "(" },
    { .name = "T_OP_RPAREN", .value = ")" },
    { .name = "T_OP_PERIOD", .value = "." },
    { .name = "T_OP_ARROW", .value = "->" },
    { .name = "T_OP_INCREMENT", .value = "++" },
    { .name = "T_OP_DECREMENT", .value = "--" },
    { .name = "T_OP_MINUS", .value = "-" },
    { .name = "T_OP_PLUS", .value = "+" },
    { .name = "T_OP_MULTIPLY", .value = "*" },
    { .name = "T_OP_DIVIDE", .value = "/" },
    { .name = "T_OP_MODULUS", .value = "%" },
    { .name = "T_OP_BITAND", .value = "&" },
    { .name = "T_OP_BITOR", .value = "|" },
    { .name = "T_OP_BITCOMP", .value = "~" },
    { .name = "T_OP_BITSHIFTL", .value = "<<" },
    { .name = "T_OP_BITSHIFTR", .value = ">>" },
    { .name = "T_OP_BITXOR", .value = "^" },
    { .name = "T_OP_NOT", .value = "!" },
    { .name = "T_OP_LT", .value = "<" },
    { .name = "T_OP_LTE", .value = "<=" },
    { .name = "T_OP_GT", .value = ">" },
    { .name = "T_OP_GTE", .value = ">=" },
    { .name = "T_OP_EQUALS", .value = "==" },
    { .name = "T_OP_NEQUALS", .value = "!=" },
    { .name = "T_OP_AND", .value = "&&" },
    { .name = "T_OP_OR", .value = "||" },
    { .name = "T_OP_QUESTION", .value = "?" },
    { .name = "T_OP_ASSIGN", .value = "=" },
    { .name = "T_OP_ASSIGN_MULTIPLY", .value = "*=" },
    { .name = "T_OP_ASSIGN_DIVIDE", .value = "/=" },
    { .name = "T_OP_ASSIGN_MODULUS", .value = "%=" },
    { .name = "T_OP_ASSIGN_PLUS", .value = "+=" },
    { .name = "T_OP_ASSIGN_MINUS", .value = "-=" },
    { .name = "T_OP_ASSIGN_BITSHIFTL", .value = "<<=" },
    { .name = "T_OP_ASSIGN_BITSHIFTR", .value = ">>=" },
    { .name = "T_OP_ASSIGN_BITAND", .value = "&" },
    { .name = "T_OP_ASSIGN_BITXOR", .value = "^=" },
    { .name = "T_OP_ASSIGN_BITOR", .value = "|=" },
    { .name = "T_OP_COMMA", .value = "," },
    { .name = "T_OP_HASH", .value = "#" },
    { .name = "T_OP_DHASH", .value = "##" },
    { .name = "T_OP_SEMICOLON", .value = ";" },
    { .name = "T_OP_COLON", .value = ":" },
    { .name = "T_OP_ELLIPSIS", .value = "..." },
    { .name = "T_LIT_CHAR", .value = "" },
    { .name = "T_LIT_INT", .value = "" },
    { .name = "T_LIT_FLOAT", .value = "" },
    { .name = "T_LIT_STR", .value = "" },
    { .name = "T_LIT_ID", .value = "" },
    { .name = "T_KW_AUTO", .value = "auto" },
    { .name = "T_KW_BREAK", .value = "break" },
    { .name = "T_KW_CASE", .value = "case" },
    { .name = "T_KW_CHAR", .value = "char" },
    { .name = "T_KW_CONST", .value = "const" },
    { .name = "T_KW_CONTINUE", .value = "continue" },
    { .name = "T_KW_DEFAULT", .value = "default" },
    { .name = "T_KW_DO", .value = "do" },
    { .name = "T_KW_DOUBLE", .value = "double" },
    { .name = "T_KW_ELSE", .value = "else" },
    { .name = "T_KW_ENUM", .value = "enum" },
    { .name = "T_KW_EXTERN", .value = "extern" },
    { .name = "T_KW_FLOAT", .value = "float" },
    { .name = "T_KW_FOR", .value = "for" },
    { .name = "T_KW_GOTO", .value = "goto" },
    { .name = "T_KW_IF", .value = "if" },
    { .name = "T_KW_INT", .value = "int" },
    { .name = "T_KW_LONG", .value = "long" },
    { .name = "T_KW_REGISTER", .value = "register" },
    { .name = "T_KW_RETURN", .value = "return" },
    { .name = "T_KW_SHORT", .value = "short" },
    { .name = "T_KW_SIGNED", .value = "signed" },
    { .name = "T_KW_SIZEOF", .value = "sizeof" },
    { .name = "T_KW_STATIC", .value = "static" },
    { .name = "T_KW_STRUCT", .value = "struct" },
    { .name = "T_KW_SWITCH", .value = "switch" },
    { .name = "T_KW_TYPEDEF", .value = "typedef" },
    { .name = "T_KW_UNION", .value = "union" },
    { .name = "T_KW_UNSIGNED", .value = "unsigned" },
    { .name = "T_KW_VOID", .value = "void" },
    { .name = "T_KW_VOLATILE", .value = "volatile" },
    { .name = "T_KW_WHILE",. value = "while" },
    { .name = "T_EOF", .value = "-1" },
    { .name = "T_UNINITIALIZED", .value = "-2" }
};

int main(int argc, char **argv)
{
    struct lexical_env_t *e = lexical_env_new();
    size_t i = e->tok;
    printf("%2d: %22s: %s\n", (int) i, token_strings[i].name,
           token_strings[i].value);
    while (e->tok != T_EOF) {
        gettoken(e);
        i = (size_t) e->tok;
        printf("%2d: %22s: ", (int) i, token_strings[i].name);
        if (i >= T_LIT_CHAR && i <= T_KW_WHILE)
            printf("%s\n", e->value);
        else
            printf("%s\n", token_strings[i].value);
    }
    lexical_env_free(e);
    return 0;
}
