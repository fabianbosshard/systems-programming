#include <stdio.h>
#include <assert.h>

#include "cset.h"

static unsigned int good_primes[] = {
    2, 23, 37, 47, 53, 67, 79, 83, 89, 97,
    113, 127, 131, 157, 163, 167, 173, 211, 223, 233,
    251, 257, 263, 277, 293, 307, 317, 331, 337, 353,
    359, 367, 373, 379, 383, 389, 397, 401, 409, 439,
    443, 449, 457, 467, 479, 487, 491, 499, 503, 509,
    541, 547, 557, 563, 577, 587, 593, 607, 613, 631,
    647, 653, 673, 677, 683, 691, 701, 709, 719, 727,
    733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
    839, 853, 863, 877, 887, 907, 911, 919, 929, 937,
    941, 947, 953, 967, 971, 977, 983, 991, 997,
    0 };

static void test_basic(unsigned int size) {
    int i;
    cset * S1 = cset_new(size);
    cset * S2 = cset_new(size);

    assert(S1);
    assert(S2);

    assert(cset_is_empty(S1));
    assert(cset_is_empty(S2));
    assert(cset_is_equal(S1,S2));

    for(i = 0; i < size; ++i) {
	assert(!cset_find(S1, i));
	assert(!cset_find(S2, i));
    }

    cset_insert(S1, 3);
    assert(!cset_is_empty(S1));
    assert(cset_is_empty(S2));
    assert(!cset_is_equal(S1,S2));

    cset_insert(S1, 7);
    cset_insert(S1, 7);
    assert(!cset_is_empty(S1));
    assert(cset_find(S1, 7));
    cset_delete(S1, 7);
    assert(!cset_is_empty(S1));
    assert(!cset_find(S1, 7));

    cset_insert(S1, 7);
    cset_insert(S2, 7);
    cset_insert(S2, 3);

    assert(!cset_is_empty(S2));
    assert(cset_is_equal(S1,S2));

    cset_delete(S1, 7);
    assert(!cset_is_empty(S1));
    assert(!cset_is_equal(S1,S2));
    cset_insert(S1, 7);
    assert(cset_is_equal(S1,S2));
    for(i = 0; i < size; ++i) {
	assert(cset_find(S1, i) == cset_find(S2, i));
    }

    assert(cset_find(S1, 3));
    assert(cset_find(S1, 7));

    cset_delete(S1, 3);
    cset_delete(S1, 7);

    assert(!cset_find(S1, size - 1));
    cset_insert(S1, size - 1);
    assert(cset_find(S1, size - 1));

    assert(!cset_find(S1, 0));
    cset_insert(S1, 0);
    assert(cset_find(S1, 0));

    for(i = 0; i < size; ++i)
	cset_delete(S1, i);

    assert(cset_is_empty(S1));

    cset_insert(S1, 1);
    cset_insert(S1, 0);
    for(i = 2; i < size / 2; ++i) {
	if (!cset_find(S1, i))
	    for(int j = i + i; j < size; j += i)
		cset_insert(S1, j);
    }

    for(i = 0; good_primes[i] != 0 && good_primes[i] < size; ++i) {
	assert(!cset_find(S1, good_primes[i]));
	if (good_primes[i] + 2 < size)
	    assert(cset_find(S1, good_primes[i] + 2));
	if (good_primes[i] > 3)
	    assert(cset_find(S1, good_primes[i] - 2));
    }

    cset_destroy(S1);
    cset_destroy(S2);
}

static void test_with_good_primes(unsigned int size) {
    int i;
    cset * S1 = cset_new(size);

    cset_insert(S1, 1);
    cset_insert(S1, 0);
    for(i = 2; i < size / 2; ++i) {
	if (!cset_find(S1, i))
	    for(int j = i + i; j < size; j += i)
		cset_insert(S1, j);
    }

    for(i = 0; good_primes[i] != 0 && good_primes[i] < size; ++i) {
	assert(!cset_find(S1, good_primes[i]));
	if (good_primes[i] + 2 < size)
	    assert(cset_find(S1, good_primes[i] + 2));
	if (good_primes[i] > 3)
	    assert(cset_find(S1, good_primes[i] - 2));
    }

    cset_destroy(S1);
}

static void test_set_set(unsigned int size) {
    int i;
    cset * S1 = cset_new(size);
    cset * S2 = cset_new(size);

    assert(S1);
    assert(S2);

    for(i = 0; i < size; i += 2)
	cset_insert(S1, i);

    for(i = 1; i < size; i += 2)
	cset_insert(S2, i);

    cset_intersection(S1, S2);

    assert(cset_is_empty(S1));

    for(i = 0; i < size; i += 2)
	cset_insert(S1, i);

    cset_union(S1, S2);

    for(i = 0; i < size; ++i)
	assert(cset_find(S1, i));

    for(i = 0; i < size; ++i)
	cset_delete(S1, i);

    assert(cset_is_empty(S1));

    for(i = 0; good_primes[i] != 0 && good_primes[i] < size; ++i) {
        if (good_primes[i]  > 2)
	    cset_insert(S1, good_primes[i]);
    }

    cset * S3 = cset_new_copy(S1);
    assert(cset_is_equal(S1, S3));

    cset_intersection(S1, S2);

    assert(cset_is_equal(S1, S3));

    cset_destroy(S1);
    cset_destroy(S2);
    cset_destroy(S3);
}

int main() {
    unsigned int size;
    for (size = 10; size < 10000; size *= 2) {
	test_basic(size);
   	test_with_good_primes(size);
	test_set_set(size);
    }
    printf("Test passed.\n");
}
