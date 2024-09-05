#ifndef __MACROS_H__
#define __MACROS_H__
#define MACRO_START "macr"   /* Macro start keyword */
#define MACRO_END "end_macr" /* Macro end keyword */
#include "lines.h"


/**
 * @struct macros
 * @brief A structure representing a macro definition in the assembly language.
 *
 * This structure is used to store the name and associated lines of a macro, as well as a pointer to the next macro in a linked list.
 * @var macros::name
 * The name of the macro, stored as a character array with a maximum length defined by `MAX_LINE_LENGTH`.
 *
 * @var macros::lines
 * A pointer to the first node in a linked list of lines associated with the macro. Each node contains a line of text that defines the macro's content.
 *
 * @var macros::next
 * A pointer to the next macro in the linked list of macros.
 */
struct macros
{
    char name[MAX_LINE_LENGTH];
    struct lines *lines;
    struct macros *next;
};

/**
 * @brief Creates a new macro and adds it to the end of the linked list of macros.
 * @param macro_name The name of the macro to be added.
 * @param ptr_to_head A pointer to the head of the macros linked list.
 *
 * @return A pointer to the new macro node, or NULL if memory allocation fails.
 */
struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head);

/**
 * @brief Checks if a macro with the same given name exists in the linked list of macros.
 * @param head A pointer to the head of the macros linked list.
 * @param name The name of the macro to search for.
 *
 * @return A pointer to the macro node if it exists, NULL otherwise.
 */
struct macros *is_existing_macro(struct macros *head, char *name);

/**
 * @brief Frees all macro nodes and their associated lines is program files and at the end.
 *
 * @param head A pointer to the head of the macros linked list.
 */
void free_macros(struct macros *head);

#endif /* __MACROS_H__ */