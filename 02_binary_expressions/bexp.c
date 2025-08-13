#include "bexp.h"

int bexp_length(const expression * ex) {
    int len1, len2;
    switch (ex->op) {
    case AND:
    case OR:
    case XOR: 
	len1 = ex->value1.end - ex->value1.begin;
	len2 = ex->value2.end - ex->value2.begin;
	return (len1 > len2) ? len1 : len2;
    case PLUS:
	len1 = ex->value1.end - ex->value1.begin;
	len2 = ex->value2.end - ex->value2.begin;
	return ((len1 > len2) ? len1 : len2) + 1;
    case GREATER_THAN:
    case LESS_THAN:
    case NOT:
	return 1;
    case COMPLEMENT:
	return ex->value1.end - ex->value1.begin;
    default:
	return 0;
    }
}

static int is_binary_digit(char c) {
    return (c == '0' || c == '1');
}

static int bexp_and(value * result, const value * value1, const value * value2) {
    const char * v1 = value1->begin;
    const char * v2 = value2->begin;
    char * r = result->begin;
    while(v1 != value1->end && v2 != value2->end) {
	if (r == result->end)
	    return 0;
	if (!is_binary_digit(*v1) || !is_binary_digit(*v2))
	    return 0;
	if (*v1 == '1' && *v2 == '1')
	    *r = '1';
	else 
	    *r = '0';
	++r;
	++v1;
	++v2;
    }
    result->end = r;
    return 1;
}

static int bexp_or(value * result, const value * value1, const value * value2) {
    const char * v1 = value1->begin;
    const char * v2 = value2->begin;
    char * r = result->begin;
    while(v1 != value1->end && v2 != value2->end) {
	if (r == result->end)
	    return 0;
	if (!is_binary_digit(*v1) || !is_binary_digit(*v2))
	    return 0;
	if (*v1 == '0' && *v2 == '0')
	    *r = '0';
	else 
	    *r = '1';
	++r;
	++v1;
	++v2;
    }
    result->end = r;
    return 1;
}

static int bexp_xor(value * result, const value * value1, const value * value2) {
    const char * v1 = value1->begin;
    const char * v2 = value2->begin;
    char * r = result->begin;
    while(v1 != value1->end && v2 != value2->end) {
	if (r == result->end)
	    return 0;
	if (!is_binary_digit(*v1) || !is_binary_digit(*v2))
	    return 0;
	if (*v1 == *v2)
	    *r = '0';
	else 
	    *r = '1';
	++r;
	++v1;
	++v2;
    }
    result->end = r;
    return 1;
}

static void flip_string(char * begin, char * end) {
    while(begin != end) {
	if (--end == begin)
	    return;
	char c = *begin;
	*begin = *end;
	*end = c;
	++begin;
    }
}

#if 1
static int bexp_plus(value * result, const value * value1, const value * value2) {
    static unsigned int adder_table_v[] = { 0, 1, 1, 0, 1, 0, 0, 1 }; 
    static unsigned int adder_table_c[] = { 0, 0, 0, 1, 0, 1, 1, 1 };
    unsigned int c = 0;
    unsigned int b1, b2;

    const char * v1 = value1->end;
    const char * v2 = value2->end;
    char * r = result->begin;

    while(v1 != value1->begin || v2 != value2->begin) {
	if (r == result->end)
	    return 0;

	if (v1 != value1->begin) {
	    --v1;
	    if (!is_binary_digit(*v1))
		return 0;
	    b1 = *v1 - '0';
	} else 
	    b1 = 0;

	if (v2 != value2->begin) {
	    --v2;
	    if (!is_binary_digit(*v2))
		return 0;
	    b2 = *v2 - '0';
	} else
	    b2 = 0;

	*r++ = '0' + adder_table_v[b1 | (b2 << 1) | (c << 2)];
	c = adder_table_c[b1 | (b2 << 1) | (c << 2)];
    }
    if (c > 0) {
	if (r == result->end)
	    return 0;
	*r++ = '1';
    }
    result->end = r;
    flip_string(result->begin, result->end);
    return 1;
}
#else
static int bexp_plus(value * result, const value * value1, const value * value2) {
    int carry = 0;
    const char * v1 = value1->end;
    const char * v2 = value2->end;
    char * r = result->begin;

    while(v1 != value1->begin || v2 != value2->begin) {
	if (r == result->end)
	    return 0;

	if (v1 != value1->begin) {
	    --v1;
	    if (!is_binary_digit(*v1))
		return 0;
	    carry += *v1 - '0';
	}

	if (v2 != value2->begin) {
	    --v2;
	    if (!is_binary_digit(*v2))
		return 0;
	    carry += *v2 - '0';
	}

	if (carry > 1) {
	    *r = '0' + (carry - 2);
	    carry = 1;
	} else {
	    *r = '0' + carry;
	    carry = 0;
	}
	++r;
    }
    if (carry > 0) {
	if (r == result->end)
	    return 0;
	*r++ = '1';
    }
    result->end = r;
    flip_string(result->begin, result->end);
    return 1;
}
#endif

static int bexp_compare(const value * value1, const value * value2) {
    int p1 = value1->end - value1->begin;
    int p2 = value2->end - value2->begin;

    while (p1 > 0 || p2 > 0) {
	if (p1 > p2) {
	    if (value1->begin[p1 - 1] == '0')
		--p1;
	    else 
		return 1;
	} else if (p1 < p2) {
	    if (value2->begin[p2 - 1] == '0')
		--p2;
	    else 
		return -1;
	} else {
	    if (value1->begin[p1 - 1] == value2->begin[p2 - 1]) {
		--p1;
		--p2;
	    } else if (value1->begin[p1 - 1] > value2->begin[p2 - 1]) {
		return 1;
	    } else {
		return -1;
	    }
	}
    }
    return 0;
}

static int bexp_gt(value * result, const value * value1, const value * value2) {
    if (bexp_compare(value1, value2) == 1) {
	result->begin[0] = '1';
    } else {
	result->begin[0] = '0';
    }
    result->end = result->begin + 1;
    return 1;
}

static int bexp_lt(value * result, const value * value1, const value * value2) {
    if (bexp_compare(value1, value2) == -1) {
	result->begin[0] = '1';
    } else {
	result->begin[0] = '0';
    }
    result->end = result->begin + 1;
    return 1;
}

static int bexp_complement(value * result, const value * value) {
    const char * v;
    char * r = result->begin;
    for(v = value->begin; v != value->end; ++v) {
	if (r == result->end)
	    return 0;
	switch(*v) {
	case '0': *r++ = '1'; break;
	case '1': *r++ = '0'; break;
	default:
	    return 0;
	}
    }
    result->end = r;
    return 1;
}

static int bexp_not(value * result, const value * value) {
    const char * v;
    for(v = value->begin; v != value->end; ++v) {
	if (*v == '1') {
	    result->begin[0] = '0';
	    result->end = result->begin + 1;
	    return 1;
	}
    }
    result->begin[0] = '1';
    result->end = result->begin + 1;
    return 1;
}

int bexp_evaluate(value * result, expression * ex) {
    switch (ex->op) {
    case AND: return bexp_and(result, &(ex->value1), &(ex->value2));
    case OR: return bexp_or(result, &(ex->value1), &(ex->value2));
    case XOR: return bexp_xor(result, &(ex->value1), &(ex->value2));
    case PLUS: return bexp_plus(result, &(ex->value1), &(ex->value2));
    case GREATER_THAN: return bexp_gt(result, &(ex->value1), &(ex->value2)); 
    case LESS_THAN: return bexp_lt(result, &(ex->value1), &(ex->value2));
    case NOT: return bexp_not(result, &(ex->value1));
    case COMPLEMENT: return bexp_complement(result, &(ex->value1));
	;
    }
    return 0;
}
