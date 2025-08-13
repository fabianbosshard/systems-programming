// test_bexp.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "bexp.h"

/* ---- tiny helpers -------------------------------------------------- */

/* Make a value from an MSB-first C string like "1011".
   It writes LSB-first into buf and sets out->begin/out->end.
   Returns 1 on success, 0 on bad chars or too small buf. */
static int value_from_msb(const char *msb, char *buf, size_t bufsize, value *out) {
    size_t n = strlen(msb);
    if (n > bufsize) return 0;
    for (size_t i = 0; i < n; ++i) {
        char c = msb[n - 1 - i];            // reverse (LSB first)
        if (c != '0' && c != '1') return 0;
        buf[i] = c;
    }
    out->begin = buf;
    out->end   = buf + n;
    return 1;
}

/* Convert a value (LSB-first) into an MSB-first C string.
   out must have room for (len+1). Returns pointer to out. */
static char *value_to_msb(const value *v, char *out, size_t outsize) {
    size_t n = (size_t)(v->end - v->begin);
    if (outsize < n + 1) { if (outsize) out[0] = '\0'; return out; }
    for (size_t i = 0; i < n; ++i) out[i] = v->begin[n - 1 - i];
    out[n] = '\0';
    return out;
}

/* Assert the value equals the expected MSB string. */
static int assert_value_eq(const char *expected_msb, const value *got, const char *label) {
    char tmp[256];
    value_to_msb(got, tmp, sizeof tmp);
    int ok = (strcmp(tmp, expected_msb) == 0);
    if (!ok) {
        fprintf(stderr, "[FAIL] %s: expected \"%s\", got \"%s\"\n", label, expected_msb, tmp);
    }
    return ok;
}

/* Assert the length equals expected. */
static int assert_length_eq(int expected, const expression *ex, const char *label) {
    int L = bexp_length(ex);
    if (L != expected) {
        fprintf(stderr, "[FAIL] %s: expected length %d, got %d\n", label, expected, L);
        return 0;
    }
    return 1;
}

/* Run one evaluation test:
   - prepares result buffer with capacity res_cap
   - checks return code, used length, and content (MSB string) */
static int eval_case(const char *label,
                     const char *op1_msb,
                     operator_t op,
                     const char *op2_msb,            // ignored for unary
                     const char *expect_msb,
                     int expect_rc,
                     size_t res_cap)
{
    char b1[256], b2[256], rbuf[256];
    value v1 = {0}, v2 = {0}, res = {0};
    expression ex = {0};

    if (!value_from_msb(op1_msb, b1, sizeof b1, &v1)) { fprintf(stderr, "bad op1\n"); return 0; }
    if (op == NOT || op == COMPLEMENT) {
        (void)op2_msb; // ignored
        v2.begin = v2.end = NULL;
    } else {
        if (!value_from_msb(op2_msb ? op2_msb : "", b2, sizeof b2, &v2)) { fprintf(stderr, "bad op2\n"); return 0; }
    }

    ex.op = op;
    ex.value1 = v1;
    ex.value2 = v2;

    if (res_cap > sizeof rbuf) res_cap = sizeof rbuf;
    res.begin = rbuf;
    res.end   = rbuf + res_cap;

    int rc = bexp_evaluate(&res, &ex);

    size_t used = (size_t)(res.end - res.begin);
    int ok = 1;

    if (rc != expect_rc) {
        fprintf(stderr, "[FAIL] %s: expected rc=%d, got %d\n", label, expect_rc, rc);
        ok = 0;
    }
    if (rc == 1) {
        /* When success was expected, check content and length matches expect_msb */
        char tmp[256];
        value_to_msb(&res, tmp, sizeof tmp);
        size_t exp_len = strlen(expect_msb);
        if (used != exp_len) {
            fprintf(stderr, "[FAIL] %s: expected used_len=%zu, got %zu\n", label, exp_len, used);
            ok = 0;
        }
        ok &= assert_value_eq(expect_msb, &res, label);
    }
    if (ok) fprintf(stdout, "[ OK ] %s\n", label);
    return ok;
}

/* ---- tests ---------------------------------------------------------- */

static int tests(void) {
    int pass = 1;

    /* 1) PLUS with carry: 1 + 1 = 10 */
    pass &= eval_case("PLUS simple carry", "1", PLUS, "1", "10", 1, 8);

    /* 2) PLUS different widths: 1011 (11) + 1 = 1100 (12) */
    pass &= eval_case("PLUS width mismatch", "1011", PLUS, "1", "1100", 1, 8);

    /* 3) AND across lengths: 10110 & 111 = 110 (MSB) */
    pass &= eval_case("AND", "10110", AND, "111", "110", 1, 8);

    /* 4) OR: 1000 | 1 = 1001 */
    pass &= eval_case("OR", "1000", OR, "1", "1001", 1, 8);

    /* 5) XOR: 1010 ^ 0110 = 1100 */
    pass &= eval_case("XOR", "1010", XOR, "0110", "1100", 1, 8);

    /* 6) GREATER_THAN: 101 (5) > 11 (3) => 1 */
    pass &= eval_case(">", "101", GREATER_THAN, "11", "1", 1, 8);

    /* 7) LESS_THAN: 100 (4) < 0111 (7) => 1 */
    pass &= eval_case("<", "100", LESS_THAN, "0111", "1", 1, 8);

    /* 8) LESS_THAN false => 0 (single-bit zero -> empty string or "0"?)
       We expect canonical zero as empty sequence. */
    pass &= eval_case("< false becomes 0", "101", LESS_THAN, "1", "", 1, 8);

    /* 9) NOT (logical): NOT(0) = 1, NOT(nonzero)=0 */
    pass &= eval_case("NOT zero", "", NOT, NULL, "1", 1, 8);
    pass &= eval_case("NOT nonzero", "1", NOT, NULL, "", 1, 8);

    /* 10) COMPLEMENT (bitwise, fixed to operand width): ~"10110" = "01001" -> trims msb zeros -> "1001" */
    pass &= eval_case("COMPLEMENT", "10110", COMPLEMENT, NULL, "1001", 1, 8);

    /* 11) COMPLEMENT of single '1' -> zero -> empty sequence */
    pass &= eval_case("COMPLEMENT -> zero", "1", COMPLEMENT, NULL, "", 1, 8);

    /* 12) Insufficient buffer for PLUS: 111 + 1 = 1000 (needs 4), give cap=3 -> fail (rc=0) */
    pass &= eval_case("INSUFFICIENT BUFFER", "111", PLUS, "1", /*ignored*/"", 0, 3);

    /* 13) bexp_length checks (only a few, the rest implied by evaluate):
       We expect the implementer to compute lengths without evaluating fully. */
    {
        char b1[16], b2[16];
        value v1, v2;
        expression ex;

        value_from_msb("1", b1, sizeof b1, &v1);
        value_from_msb("1", b2, sizeof b2, &v2);
        ex.op = PLUS; ex.value1 = v1; ex.value2 = v2;
        pass &= assert_length_eq(2, &ex, "len PLUS 1+1==2");

        value_from_msb("10110", b1, sizeof b1, &v1);
        value_from_msb("111",   b2, sizeof b2, &v2);
        ex.op = AND; ex.value1 = v1; ex.value2 = v2;
        pass &= assert_length_eq(3, &ex, "len AND width=min(5,3)=3 (then trimmed)");

        ex.op = GREATER_THAN;
        pass &= assert_length_eq(1, &ex, "len > is 1");

        ex.op = NOT;
        ex.value2.begin = ex.value2.end = NULL;
        pass &= assert_length_eq(1, &ex, "len NOT is 1");

        ex.op = COMPLEMENT;
        pass &= assert_length_eq(5, &ex, "len ~ equals operand width (before trimming zero result)");
    }

    /* 14) Unknown operator: cast an out-of-range value */
    {
        char b1[8];
        value v1; value_from_msb("1", b1, sizeof b1, &v1);
        expression ex = { .op = (operator_t)999, .value1 = v1, .value2 = v1 };
        int L = bexp_length(&ex);
        if (L != -1) { fprintf(stderr, "[FAIL] unknown op length should be -1, got %d\n", L); pass = 0; }
        char r[4]; value res = { .begin = r, .end = r + sizeof r };
        int rc = bexp_evaluate(&res, &ex);
        if (rc != 0) { fprintf(stderr, "[FAIL] unknown op evaluate should be 0, got %d\n", rc); pass = 0; }
        else fprintf(stdout, "[ OK ] unknown operator handled\n");
    }

    return pass;
}

int main(void) {
    int ok = tests();
    if (ok) {
        puts("\nAll tests passed ✅");
        return 0;
    }
    puts("\nSome tests failed ❌");
    return 1;
}
