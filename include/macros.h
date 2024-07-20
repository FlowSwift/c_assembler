#ifndef __MACROS_H__
#define __MACROS_H__
#define MACRO_START "macr"   /* Macro start keyword */
#define MACRO_END "end_macr" /* Macro end keyword */
#include "lines.h"

struct macros
{
    char name[MAX_LINE_LENGTH];
    struct lines *lines;
    struct macros *next;
};

struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head);
struct macros *is_existing_macro(struct macros *head, char *line);
void free_macros(struct macros *head);

#endif /* __MACROS_H__ */