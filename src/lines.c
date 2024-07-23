#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lines.h"
#include "error_handler.h"

/* 
    Allocates memory for a new line node and initializes it with the given line content.
    Returns a pointer to the new line node.
*/
struct lines *malloc_line(char *line)
{
    struct lines *new_line = malloc(sizeof(struct lines));
    if (new_line == NULL)
    {
        handle_error(ERROR_MEMORY_ALLOCATION_FAILED, 0);
        return NULL;
    }
    strcpy(new_line->line, line);
    new_line->next = NULL;
    return new_line;
}

/* Frees all the line nodes in the linked list */
void free_lines(struct lines *head)
{
    struct lines *current = head;
    struct lines *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}