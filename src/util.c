#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "error_handler.h"

char *add_file_extension(char *filename, char *extension)
{
    char *new_filename = malloc(sizeof(char) * MAX_FILE_NAME);
    if (new_filename == NULL)
    {
        handle_error(ERROR_MEMORY_ALLOCATION_FAILED, 0);
        return NULL;
    }
    strcpy(new_filename, filename);
    strcat(new_filename, extension);
    return new_filename;
}


int is_empty_line(char *line)
{
    while (*line != '\0') {
        if (!isspace(*line)) {
            return 1;  /* Not empty */
        }
        line++; 
    }
    return 0;
}

int is_commented_line(char *line)
{
    if (*line == ';') {
        return 0; /* is commented line*/
    }
    return 1;
}

int is_valid_integer(char *operand) {
    ErrorCode error_flag = 0; /*assume success*/
    if (operand == NULL || *operand == '\0') {
        error_flag = ERROR_OPERAND_IS_EMPTY;
        return error_flag; /* Operand is null or empty */
    }
    if((isdigit(*operand)) || (*operand != '+') || (*operand != '-')) {/* Ensure the first character is a digit or a sign ('+' or '-') */
        operand++; /*move past first char*/
        while (*operand != '\0') {
            if (!isdigit(*operand)) {
                error_flag = ERROR_NOT_VALID_INTEGER; /*founs not integer char */
                return error_flag;
            }
            operand++; /*move to nest char*/
        }
    }
    else{ /*if first char not in allowed chars*/
        error_flag = ERROR_NOT_VALID_INTEGER; /*founs not integer char */
    }
    return error_flag; /* 0 - > Success. Operand is a valid integer */
}

int valid_reg_name(char *value){
    if (strlen(value) == 2 && value[0] == 'r' && value[1] >= '0' && value[1] <= '7'){
        return 0; /*SUCCESS*/
    }
    return 1;

}

int count_occurrences(const char *str, char ch) {
    int count = 0;
    while (*str!='\0') {
        if (*str == ch) {
            count++;
        }
        str++;
    }
    return count;
}

char *strdup1(char *src_string)
{
    size_t len;
    char *duplicate_string = NULL;
    if (src_string == NULL)
    {
        return NULL;
    }

    len = strlen(src_string) + 1;
    duplicate_string = malloc(len);

    if (duplicate_string != NULL)
    {
        memcpy(duplicate_string, src_string, len);
    }
    return duplicate_string;
}

int is_valid_string(char* str){
    /*starts and ends with ""*/
    if ((str[0] != '"') || (str[strlen(str) - 1] != '"') || (strlen(str) <=2)){
        return 1; /*not string "{some value}" */
    }
    return 0; /*success*/
}