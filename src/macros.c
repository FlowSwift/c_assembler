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
    if (new_macro == NULL) /*if allocation failed.*/
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
    char temp_name[MAX_LINE_LENGTH];
    strcpy(temp_name, name);
    if (temp_name[strlen(temp_name) - 1] == '\n')
    {
        temp_name[strlen(temp_name) - 1] = '\0'; /*remove newline character*/
    }
    while (current != NULL)
    {
        if (strcmp(current->name, temp_name) == 0)
        {
            return current; /*returns a pointer to the existing macro.*/
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
