#ifndef __UTIL_H__
#define __UTIL_H__
#define MAX_FILE_NAME 100    /* Assuming file name no bigger than 100 chars */

char *add_file_extension(char *filename, char *extension);
int is_empty_line(char *line);
int is_commented_line(char *line);
int is_valid_integer(char *operand);
int valid_reg_name(char *value);
int count_occurrences(const char *str, char ch);
char *strdup1(char *src_string);
int is_valid_string(char* str);


#endif