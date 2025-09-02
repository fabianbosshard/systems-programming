#ifndef CSET_H_INCLUDED
#define CSET_H_INCLUDED

/*  An extremely compact dynamic set capable of representing sets out
 *  of the universe of integers {0,1,2,...,n-1}.  n is a parameter of
 *  the constructor (see below).
 */
struct cset;
typedef struct cset cset;

extern cset * cset_new(unsigned int n);                                       /* complexity: O(n) */
extern cset * cset_new_copy(const cset * s);                                  /* complexity: O(n) */
extern void cset_destroy(cset * s);                                           /* complexity: O(1) */

extern void cset_insert(cset * s, unsigned int elem);                         /* complexity: O(1) */
extern void cset_delete(cset * s, unsigned int elem);                         /* complexity: O(1) */
extern int  cset_find(const cset * s, unsigned int elem);                     /* complexity: O(1) */

extern void cset_union(cset * a, const cset * b);                             /* complexity: O(n) */
extern void cset_intersection(cset * a, const cset * b);                      /* complexity: O(n) */

extern int  cset_is_empty(const cset * s);                                    /* complexity: O(n) */
extern int  cset_is_equal(const cset * a, const cset * b);                    /* complexity: O(n) */

#endif
