#ifndef __LINES_H__
#define __LINES_H__
#define MAX_LINE_LENGTH 81   /* Max 80 chars + \n */

struct lines
{
    char line[MAX_LINE_LENGTH];
    struct lines *next;
};

struct lines *malloc_line(char *line);
void free_lines(struct lines *head);

#endif /* __LINES_H__ */