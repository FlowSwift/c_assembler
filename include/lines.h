#ifndef __LINES_H__
#define __LINES_H__
#define MAX_LINE_LENGTH 82 /* Max 80 chars + \n + \0 */


/**
 * @struct lines
 * @brief A structure representing a node in a linked list of lines.
 * This structure is used to store a single line of text and a pointer to the next line in the linked list.
 * Will be parsed to Assembly line.
 * @var lines::line
 * The content of the line, stored as a character array with a maximum length of `MAX_LINE_LENGTH`.
 *
 * @var lines::next
 * A pointer to the next node in the linked list of lines.
 */
struct lines
{
    char line[MAX_LINE_LENGTH];
    struct lines *next;
};


/**
 * @brief Allocates memory for a new line node and initializes it with the given line content.
 * This function creates a new node in the linked list of lines, copying the content of the provided line
 * into the node. It sets the `next` pointer to `NULL` by default.
 *
 * @param line The content of the line to store in the new node.
 *
 * @return A pointer to the newly allocated line node, or `NULL` if memory allocation fails.
 */
struct lines *malloc_line(char *line);

/**
 * @brief Frees all the line nodes in the linked list.
 *
 * This function deallocates memory for all nodes in a linked list of lines, starting from the head.
 *
 * @param head A pointer to the first node in the linked list of lines.
 */
void free_lines(struct lines *head);

#endif /* __LINES_H__ */