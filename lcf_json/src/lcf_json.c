//
// Created by nim on 2022/3/26.
//

#include "lcf_json.h"
#include <assert.h> /* assert() */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */
#include <stdlib.h> /* NULL, strtod(), malloc(), free() */

#define EXPECT(ctx, ch)                                                                            \
    do {                                                                                           \
        assert(*(ctx)->json == (ch));                                                              \
        (ctx)->json++;                                                                             \
    } while (0)

#define IS_DECIMAL_DIGIT(ch)  ((ch) >= '0' && (ch) <= '9')
#define IS_NON_ZERO_DIGIT(ch) ((ch) >= '1' && (ch) <= '9')

static void
lj_parse_whitespace(lj_context_t *ctx);

static int
lj_parse_number(lj_context_t *ctx, lj_value_t *val);

static int
lj_parse_value(lj_context_t *ctx, lj_value_t *val);

static int
lj_parse_literal(lj_context_t *ctx, lj_value_t *val, const char *literal, lj_type_t type);

// Parse Whitespace
// type:
//  - ' '   space
//  - '\t'  horizontal tab
//  - '\n'  carriage return
//  - '\r'  linefeed
static void
lj_parse_whitespace(lj_context_t *ctx)
{
    const char *p = ctx->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    ctx->json = p;
}

static int
lj_parse_literal(lj_context_t *ctx, lj_value_t *val, const char *literal, lj_type_t type)
{
    EXPECT(ctx, literal[0]);
    size_t i;
    for (i = 0; literal[i + 1]; i++) {
        if (ctx->json[i] != literal[i + 1]) {
            return LJ_PARSE_INVALID_VALUE;
        }
    }
    ctx->json += i;
    val->type = type;
    return LJ_PARSE_OK;
}

static int
lj_parse_number(lj_context_t *ctx, lj_value_t *val)
{
    const char *p = ctx->json;
    if (*p == '-') {
        p++;
    }
    if (*p == '0') {
        p++;
    } else {
        if (!IS_NON_ZERO_DIGIT(*p)) {
            return LJ_PARSE_INVALID_VALUE;
        }
        do {
            p++;
        } while (IS_DECIMAL_DIGIT(*p));
    }
    if (*p == '.') {
        p++;
        if (!IS_DECIMAL_DIGIT(*p)) {
            return LJ_PARSE_INVALID_VALUE;
        }
        do {
            p++;
        } while (IS_DECIMAL_DIGIT(*p));
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') {
            p++;
        }
        if (!IS_DECIMAL_DIGIT(*p)) {
            return LJ_PARSE_INVALID_VALUE;
        }
        do {
            p++;
        } while (IS_DECIMAL_DIGIT(*p));
    }
    errno  = 0;
    val->n = strtod(ctx->json, NULL);
    if (errno == ERANGE && (val->n == HUGE_VAL || val->n == -HUGE_VAL)) {
        return LJ_PARSE_NUMBER_TOO_BIG;
    }
    val->type = LJ_NUMBER;
    ctx->json = p;
    return LJ_PARSE_OK;
}

static int
lj_parse_value(lj_context_t *ctx, lj_value_t *val)
{
    switch (*ctx->json) {
        case 'n':
            return lj_parse_literal(ctx, val, "null", LJ_NULL);
        case 't':
            return lj_parse_literal(ctx, val, "true", LJ_TRUE);
        case 'f':
            return lj_parse_literal(ctx, val, "false", LJ_FALSE);
        default:
            return lj_parse_number(ctx, val);
        case '\0':
            return LJ_PARSE_EXPECT_VALUE;
    }
}

int
lj_parse(lj_value_t *val, const char *json)
{
    assert(val != NULL);

    int          ret;
    lj_context_t ctx;
    ctx.json  = json;
    ctx.stack = NULL;
    ctx.size  = 0;
    ctx.top   = 0;
    lj_init(val);

    lj_parse_whitespace(&ctx);
    ret = lj_parse_value(&ctx, val);

    if (ret == LJ_PARSE_OK) {
        lj_parse_whitespace(&ctx);
        if (ctx.json[0] != '\0') {
            ret = LJ_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    assert(ctx.top == 0);
    free(ctx.stack);

    return ret;
}

void
lj_free(lj_value_t *val)
{
    assert(val != NULL);
    if (val->type == LJ_STRING) {
        free(val->str.data);
    }
    val->type = LJ_NULL;
}

lj_type_t
lj_get_type(const lj_value_t *val)
{
    assert(val != NULL);
    return val->type;
}

lj_bool_t
lj_get_boolean(const lj_value_t *val)
{
    // TODO
    return false;
}

void
lj_set_boolean(const lj_value_t *val, lj_bool_t b)
{
    // TODO
}

double
lj_get_number(const lj_value_t *val)
{
    assert(val != NULL && val->type == LJ_NUMBER);
    return val->n;
}

void
lj_set_number(const lj_value_t *val, double n)
{
    // TODO
}

lj_string_t
lj_get_string(const lj_value_t *val)
{
    assert(val != NULL && val->type == LJ_STRING);
    return val->str;
}

void
lj_set_string(lj_value_t *val, const char *str, size_t len)
{
    assert(val != NULL && (str != NULL || len != 0));

    lj_free(val);
    val->str.data = lj_malloc(u_char *, len);
    lj_memcpy(val->str.data, str, len);

    val->str.data[len] = '\0';
    val->str.len       = len;
    val->type          = LJ_STRING;
}
