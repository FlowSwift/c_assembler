#ifndef __PREPROCESSOR_H__
#define __PREPROCESSOR_H__
#include <stdio.h>
#include "macros.h"

FILE *pre_process(char *file_name);
int strip_file(char *filename, char *temp_file_name);
void strip_line(char *dest, char *source);
FILE *process_macros(char *filename, char *temp_file_name);
int validate_macro_name(char *macr_ptr, char *line);
void add_macro(FILE *file, FILE *processed_file, struct macros **ptr_to_head, char *macro_ptr);
void write_macro(struct macros *macro, FILE *file);

#endif /* __PREPROCESSOR_H__ */