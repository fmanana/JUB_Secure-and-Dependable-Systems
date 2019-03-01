#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpn-stack.h"
#include "rpn.h"

int main(int argc, char **argv)
{
    char *result = malloc(sizeof(char) * argc);
    char *expr = malloc(sizeof(char));
    for(int i = 1; i < argc; i++)
    {
        expr = realloc(expr, strlen(expr) + strlen(argv[i]));
        strcat(expr, argv[i]);
        if(i != argc - 1)
        {
            expr = realloc(expr, strlen(expr) + 1);
            strcat(expr, " ");
        }
    }
    rpn_eval_expr(expr, &result);

    return EXIT_SUCCESS;
}