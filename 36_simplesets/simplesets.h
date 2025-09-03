#ifndef SIMPLESETS_H_INCLUDED
#define SIMPLESETS_H_INCLUDED

/* Represents a set of objects. */
struct ss;

/* Create a set containing a single object. */
extern struct ss * ss_create_singleton();

/* Destroy all previously created sets. */
extern void ss_destroy_all();

/* Merge two sets into a single set.
 *
 * As a result, all the elements in X and Y belong to the same set,
 * and both X and Y now refer to the same set.
 */
extern void ss_merge(struct ss * X, struct ss * Y);

/* Test whether two sets are disjoint. */
extern int ss_disjoint(struct ss * X, struct ss * Y);

#endif
