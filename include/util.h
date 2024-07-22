#ifndef __UTIL_H__
#define __UTIL_H__
#define MAX_FILE_NAME 100    /* Assuming file name no bigger than 100 chars */

char *add_file_extension(char *filename, char *extension);
int isEmptyLine(char *line);
int isCommentedLine(char *line);
int is_valid_integer(char *operand);
int count_occurrences(const char *str, char ch);
char *strdup(char *src_string);
int is_valid_string(char* str);


#endif