#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lines.h"

/* ASSUMES CURRENT LINE */
struct lines *malloc_line(char *line)
{
    struct lines *new_line = malloc(sizeof(struct lines));
    if (new_line == NULL)
    {
        printf("MEM ERROR\n");
        return NULL; /* WHAT TO RETURN? */
    }
    strcpy(new_line->line, line);
    new_line->next = NULL;
    return new_line;
}