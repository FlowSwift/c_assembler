#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "error_handler.h"
#include "constant.h"

struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head)
{
    struct macros *head = (*ptr_to_head);
    struct macros *new_macro = malloc(sizeof(struct macros));
    if (new_macro == NULL)
    {
        handle_error(ERROR_MEMORY_ALLOCATION_FAILED, 0);
        return NULL;
    }
    strcpy(new_macro->name, macro_name);
    new_macro->lines = NULL;
    new_macro->next = NULL;
    if ((*ptr_to_head) == NULL)
    {
        (*ptr_to_head) = new_macro;
        return new_macro;
    }
    while (head->next != NULL)
    {
        head = head->next;
    }
    head->next = new_macro;
    return new_macro;
}

struct macros *is_existing_macro(struct macros *head, char *name)
{
    struct macros *current = head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_macros(struct macros *head)
{
    struct macros *current = head;
    struct macros *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free_lines(current->lines);
        free(current);
        current = next;
    }
}
