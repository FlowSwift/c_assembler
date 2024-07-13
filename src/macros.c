#include <stdlib.h>
#include <string.h>
#include "macros.h"

struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head)
{
    struct macros *head = *ptr_to_head;
    struct macros *new_macro = malloc(sizeof(struct macros));
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
