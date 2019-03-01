#include "rpn-stack.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Prefixing all global symbols with rpn_ to avoid clashes with
 * symbols defined in the C library (e.g., signal stack definitions)
 * since we use this stack for building an RPN calculator.
 */
struct _rpn_stack
{
    void *data;
    struct _rpn_stack *prev;
};

typedef struct _rpn_stack rpn_stack_t;

/**
 * \brief Create a new empty stack.
 *
 * \return A pointer acting as a handle for the new stack.
 */

rpn_stack_t *
rpn_stack_new()
{
    rpn_stack_t *s = malloc(sizeof(rpn_stack_t));
    if (!s)
    {
        perror("rpn_stack_new: allocation failed");
        EXIT_FAILURE;
    }
    s->data = NULL; //data is not used
    s->prev = NULL; //NULL identifies that the stack is empty
    return s;
}

/**
 * \brief Push data (allocated by the caller) on the stack.
 *
 * \param s The stack to push data on.
 * \param data The pointer to data to be pushed on the stack.
 */

void rpn_stack_push(rpn_stack_t *s, void *data)
{
    rpn_stack_t *node;
    node = malloc(sizeof(rpn_stack_t));
    if (!s)
    {
        perror("rpn_stack_new: allocation failed");
        EXIT_FAILURE;
    }
    node->data = data;
    node->prev = s->prev;
    s->prev = node;
}

/**
 * \brief Pop data from the top of the stack.
 *
 * \param s The stack to pop data from.
 * \return The data or NULL if the stack is empty.
 */

void *
rpn_stack_pop(rpn_stack_t *s)
{
    rpn_stack_t *tmp;
    void *data;
    tmp = s->prev;
    data = tmp->data;
    s->prev = tmp->prev;
    tmp->prev = NULL;
    free(tmp);
    return data;
}

/**
 * \brief Peek on the data at the top of the stack.
 *
 * \param s The stack to peek on.
 * \return The data or NULL if the stack is empty.
 */

void *
rpn_stack_peek(rpn_stack_t *s)
{
    void *result;
    if (!rpn_stack_empty(s))
    {
        rpn_stack_t *current;
        current = s->prev;
        return current->data;
    }
    else
    {
        return NULL;
    }
}

/**
 * \brief Test whether a stack is empty.
 *
 * \param s The stack to test.
 * \return A non-zero value if the stack is empty, 0 otherwise.
 */

int rpn_stack_empty(rpn_stack_t *s)
{
    return s->prev == NULL ? 1 : 0;
}

/**
 * \brief Delete a stack.
 *
 * \param s The stack to delete.
 */

void rpn_stack_del(rpn_stack_t *s)
{
    while (!rpn_stack_empty(s))
    {
        rpn_stack_pop(s);
    }
}
