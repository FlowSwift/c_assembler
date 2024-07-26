#ifndef __LINES_H__
#define __LINES_H__
#define MAX_LINE_LENGTH 82 /* Max 80 chars + \n + \0 */

struct lines
{
    char line[MAX_LINE_LENGTH];
    struct lines *next;
};

struct lines *malloc_line(char *line);
void free_lines(struct lines *head);

#endif /* __LINES_H__ */