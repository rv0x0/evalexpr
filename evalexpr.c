#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* token type enum */
typedef enum {
    TOK_NUM,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_MOD,
    TOK_EOF,
    TOK_ERR
} token_type;

/* token structure */
typedef struct {
    token_type type;
    long long val;
} token;

/* lexer structure */
typedef struct {
    const char *src;
    size_t pos;
    token cur;
} lexer;

/* init lexer with source string */
static void lexer_init(lexer *L, const char *s)
{
    L->src = s;
    L->pos = 0;
    L->cur.type = TOK_EOF;
    L->cur.val = 0;
}

/* advance to next token */
static void lexer_next(lexer *L)
{
    /* skip whitespace */
    while (isspace((unsigned char)L->src[L->pos]))
        L->pos++;

    char c = L->src[L->pos];

    /* end of string */
    if (c == '\0') {
        L->cur.type = TOK_EOF;
        return;
    }

    /* parse number – only digits; unary minus is handled by the parser */
    if (isdigit((unsigned char)c)) {
        char *end;
        L->cur.val = strtol(L->src + L->pos, &end, 10);
        L->pos = end - L->src;
        L->cur.type = TOK_NUM;
        return;
    }

    /* single-char operators */
    L->pos++;
    switch (c) {
        case '+': L->cur.type = TOK_PLUS;  break;
        case '-': L->cur.type = TOK_MINUS; break;
        case '*': L->cur.type = TOK_MUL;   break;
        case '/': L->cur.type = TOK_DIV;   break;
        case '%': L->cur.type = TOK_MOD;   break;
        default:  L->cur.type = TOK_ERR;   break;
    }
}

static long parse_expr(lexer *L);

/* parse number or unary minus */
static long parse_factor(lexer *L)
{
    if (L->cur.type == TOK_NUM) {
        long v = L->cur.val;
        lexer_next(L);
        return v;
    }
    if (L->cur.type == TOK_MINUS) {
        lexer_next(L);
        return -parse_factor(L);   /* unary minus */
    }
    fprintf(stderr, "SyntaxError: expected number\n");
    exit(EXIT_FAILURE);
}

/* parse * / % (higher precedence) */
static long parse_term(lexer *L)
{
    long left = parse_factor(L);

    while (L->cur.type == TOK_MUL || L->cur.type == TOK_DIV || L->cur.type == TOK_MOD) {
        token_type op = L->cur.type;
        lexer_next(L);
        long right = parse_factor(L);

        if (op == TOK_MUL)
            left *= right;
        else if (op == TOK_DIV) {
            if (right == 0) {
                fprintf(stderr, "ZeroDivisionError: division by zero\n");
                exit(EXIT_FAILURE);
            }
            left /= right;
        } else { /* TOK_MOD */
            if (right == 0) {
                fprintf(stderr, "ZeroDivisionError: modulo by zero\n");
                exit(EXIT_FAILURE);
            }
            left %= right;
        }
    }
    return left;
}

/* parse + - (lower precedence) */
static long parse_expr(lexer *L)
{
    long left = parse_term(L);

    while (L->cur.type == TOK_PLUS || L->cur.type == TOK_MINUS) {
        token_type op = L->cur.type;
        lexer_next(L);
        long right = parse_term(L);

        if (op == TOK_PLUS)
            left += right;
        else
            left -= right;
    }
    return left;
}

/* evaluate full expression string */
long eval(const char *expr)
{
    lexer L;
    lexer_init(&L, expr);
    lexer_next(&L);               /* load first token */

    long result = parse_expr(&L);

    /* must consume entire input */
    if (L.cur.type != TOK_EOF) {
        fprintf(stderr, "SyntaxError: unexpected token\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

int main(void)
{
    char line[4096];

    while (1) {
        printf(">> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;                /* Ctrl+D / EOF */

        /* strip trailing newline */
        size_t len = strlen(line);
        if (len && line[len - 1] == '\n')
            line[--len] = '\0';

        /* ignore empty lines */
        if (len == 0)
            continue;

        long result = eval(line);
        printf("%ld\n", result);
    }

    putchar('\n');
    return 0;
}
