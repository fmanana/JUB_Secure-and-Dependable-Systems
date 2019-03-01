/*
 * rpn.h --
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "rpn-stack.h"
#include "rpn.h"

/** RPN evaluation was successful. */
#define RPN_OK 0
/** RPN evaluation failed due to an invalid token. */
#define RPN_INVALID_TOKEN -1
/** RPN evaluation failed due to a missing operand. */
#define RPN_MISSING_OPERAND -2
/** RPN evaluation failed due to a missing operator. */
#define RPN_MISSING_OPERATOR -3
/** RPN evaluation failed due to an arithmetic error. */
#define RPN_ARITHMETIC_ERROR -4

/**
 * \Computes result of intermediate calculations in stack
 * 
 * \param first The first operand
 * \param second The second operand
 * \param operator The operation to be performed
 */
int rpn_compute(int first, int second, char *operator)
{
    if(strcmp(operator, "+") == 0)
        return (second + first);
    else if(strcmp(operator, "-") == 0)
        return (second - first);
    else if(strcmp(operator,"*") == 0)
        return (second * first);
    else if(strcmp(operator, "/") == 0)
    {
        if (first == 0) // dividing by zero
        {
            perror("Arithmetics error");
            exit(RPN_ARITHMETIC_ERROR);
        }
        else
            return (second / first);
    }
    else
    {
        fprintf(stderr, "invalid token: %s\n", operator);
        exit(RPN_INVALID_TOKEN);
    }
}

/**
 * \brief Evaluate an expression in reverse polish notation.
 *
 * \param token Array of tokens making up the expression.
 * \param result Pointer to the string which will hold the result (malloced).
 * \result One of the error codes defined above.
 */
int rpn_eval_token(char *token[], char **result)
{
    int size = *(int *)token; 
    char *cursor = NULL;
    cursor = malloc(sizeof(char) * size);
    int i = 1;
    // check for invlaid tokens
    for(i = 1; i <= size; i++)
    {
        strcpy(cursor, token[i]);
        if((strcmp(cursor, "+") == 0) || (strcmp(cursor, "-") == 0) || (strcmp(cursor, "*") == 0) || (strcmp(cursor, "/") == 0))
        {
            continue;
        }
        else if(atoi(cursor) == 0 && strcmp(cursor, "0") != 0)
        { // invalid operator
            fprintf(stderr, "invalid token: %s\n", cursor);
            exit(RPN_INVALID_TOKEN);
        }
    }

    rpn_stack_t *s = rpn_stack_new();
    i = 0;
    int j = 1, value[size], intermediate[size];
    void *pop_1, *pop_2;
    while(j <= size)
    {
        strcpy(cursor, token[j]);
        value[i] = atoi(cursor);
        if(value[i] > 0 || strcmp(cursor, "0") == 0)
        {
            rpn_stack_push(s, &value[i]);
            i++;
        }
        else
        {
            pop_1 = rpn_stack_pop(s);
            if(pop_1 == NULL) // no operands in stack
            {
                perror("missing operand(s)");
                exit(RPN_MISSING_OPERAND);
            }
            pop_2 = rpn_stack_pop(s);
            if(pop_2 == NULL)
            {
                perror("missing operand(s)");
                exit(RPN_MISSING_OPERAND);
            }
            // save intermediate calculation in intermediate and push to stack
            intermediate[j - 1] = rpn_compute(*(int *)pop_1, *(int *)pop_2, cursor);
            rpn_stack_push(s, (void *) &intermediate[j - 1]);
        }
        j++;
    }
    result = (void *) rpn_stack_pop(s);
    // check peek when empty
    
    if(!rpn_stack_empty(s))
    {
        perror("missing operator(s)");
        exit(RPN_MISSING_OPERATOR);
    }
    printf("%d\n", *(int *) result);
    if(!rpn_stack_empty(s))
    {
        perror("missing operator(s)");
        exit(RPN_MISSING_OPERATOR);
    }

    rpn_stack_del(s);
    free(cursor);
    return RPN_OK;
}

/**
 * \brief Evaluate an expression in reverse polish notation.
 *
 * \param expr The expression (whitespace separated numbers and operators)
 * \param result Pointer to the string which will hold the result (malloced).
 * \result One of the error codes defined above.
 */

int rpn_eval_expr(const char *expr, char **result)
{
    char *temp = (char *) malloc(sizeof(strlen(expr) + 1));
    char *temp_ptr;
    // save pointer to later restore
    temp_ptr = temp;
    const char *delim = " \t";
    char *current_token, **tokens;
    if(!strcpy(temp, expr))
    {
        perror("in rpn_eval(): error copying expr");
        exit(EXIT_SUCCESS);
    }
    // Keep track of the number of tokens in expr
    int count = 1;
    while((temp = strpbrk(temp, delim)) != NULL)
    {
        count++;
        temp++;
    }
    // move temp to 
    temp = temp_ptr;
    current_token = strtok(temp, delim);
    // allocate tokens
    tokens = (char **) malloc((sizeof(char *) * count) + 1);
    /*
        reserve tokens[0] to store the number of tokens to parse.
        This saves time counting tokens later.
    */
    tokens[0] = (char*)(intptr_t)count;
    if(!strcpy(temp, expr))
    {
        perror("in rpn_eval(): error copying expr");
        exit(EXIT_SUCCESS);
    }
    count = 1;
    current_token = strtok(temp, delim);
    while(current_token != NULL)
    {
        tokens[count] = current_token;
        current_token = strtok(NULL, delim);
        count++;
    }

    int ret = rpn_eval_token(tokens, result);
    free(temp);
    return ret;
}