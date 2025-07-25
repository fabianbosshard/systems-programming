#include "bexp.h"


/*  Return the length (in characters) of the result of the given
 *  expression, without necessarily computing that expression.  This
 *  can be used to allocate the space necessary to store the result.
 *  Return -1 in case of error, for example if the given expression
 *  contains an unknown operator.
 */
int bexp_length(const expression * ex) {
    if (ex->value1.end - ex->value1.begin > ex->value2.end - ex->value2.begin) {
        return ex->value1.end - ex->value1.begin;
    } else {
        return ex->value2.end - ex->value2.begin;
    }
}

/*  Evaluates a given expression.  The space in which to store the
 *  result must be allocated by the caller.  Thus result->begin must
 *  point to the beginning of the allocated space while result->end
 *  points to one-past the end.  Return 1 upon success, or 0 upon
 *  failure, for example if the given expression is not valid or if
 *  the space provided by the caller is insufficient to store the
 *  result.  If the allocated space is larged than necessary, this
 *  function must adjust the length of the result sequence.
 */
int bexp_evaluate(value * result, expression * ex) {
    if (ex->op < 0 || ex->op > 7) {
        return 0; // expression not valid
    }
    if (bexp_length(ex) > result->end - result->begin) {
        return 0; // space insufficient to store result
    }
    switch (ex->op) {
        case AND:
        
    }
}
