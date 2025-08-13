#ifndef BEXP_H_INCLUDED
#define BEXP_H_INCLUDED

/*  A binary value expressed as a sequence of '0' and '1' characters,
 *  the sequence starts at the character pointed to by begin, and ends
 *  at the character right before the one pointed to by end.  Thus a
 *  "null" value can be represented as an empty sequence where
 *  begin==end.  The first character, pointed to by begin, represents
 *  the least-significant bit.  All the bits following the most
 *  significant one specified in the sequence are assumed to be 0.  A
 *  valid value must contain only '0' and '1' characters.
 */
typedef struct {
    char * begin;		/* pointer to least significant bit */
    char * end;			/* pointer to one-past most significant bit */
} value;

/* Identifiers for the operators we can use on binary values
 */ 
typedef enum {
    /* Binary operators: */
    AND,			/* & */
    OR,				/* | */
    XOR,			/* ^ */
    PLUS,			/* + */
    GREATER_THAN,		/* > */
    LESS_THAN,			/* < */
    /* Unary operators: */
    NOT,			/* ! logical negation: value==0 => 1, otherwise 0 */
    COMPLEMENT,			/* ~ complement: bitwise not */
} operator_t;

typedef struct {
    operator_t op;
    value value1;		/* first operand */
    value value2;		/* second operand, 
				   ignored if op is a unary operator. */
} expression;

/*  Return the length (in characters) of the result of the given
 *  expression, without necessarily computing that expression.  This
 *  can be used to allocate the space necessary to store the result.
 *  Return -1 in case of error, for example if the given expression
 *  contains an unknown operator.
 */
extern int bexp_length(const expression * ex);

/*  Evaluates a given expression.  The space in which to store the
 *  result must be allocated by the caller.  Thus result->begin must
 *  point to the beginning of the allocated space while result->end
 *  points to one-past the end.  Return 1 upon success, or 0 upon
 *  failure, for example if the given expression is not valid or if
 *  the space provided by the caller is insufficient to store the
 *  result.  If the allocated space is larged than necessary, this
 *  function must adjust the length of the result sequence.
 */
extern int bexp_evaluate(value * result, expression * ex);

#endif
