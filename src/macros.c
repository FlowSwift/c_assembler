#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "error_handler.h"

/* 
    Creates a new macro node and adds it to the end of the linked list of macros.
    Returns a pointer to the new macro node.
*/
struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head)
{
    struct macros *head = *ptr_to_head;
    struct macros *new_macro = malloc(sizeof(struct macros));
    if (new_macro == NULL)
    {
        handle_error(ERROR_MEMORY_ALLOCATION_FAILED, 0);
        return NULL;
    }
    strcpy(new_macro->name, macro_name);
    new_macro->lines = NULL;
    new_macro->next = NULL;
    if (*ptr_to_head == NULL)
    {
        *ptr_to_head = new_macro;
        return new_macro;
    }
    while (head->next != NULL)
    {
        head = head->next;
    }
    head->next = new_macro;
    return new_macro;
}

/* 
    Checks if a macro with the given name exists in the linked list of macros.
    Returns a pointer to the macro node if it exists, NULL otherwise.
*/
struct macros *is_existing_macro(struct macros *head, char *line)
{
    struct macros *current = head;
    char macro_name[MAX_LINE_LENGTH];
    strcpy(macro_name, line);
    macro_name[strlen(macro_name) - 1] = '\0'; /* Remove newline for comparison */
    while (current != NULL)
    {
        if (strcmp(current->name, macro_name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* 
    Frees all the macro nodes and their associated lines in the linked list
*/
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
