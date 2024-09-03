#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "error_handler.h"
#include "constant.h"
#include "first_pass.h"

/*
    Adds a file extension to the given filename.
    Returns a new string with the filename and extension.
*/
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

/*
    Checks if a line is empty (only contains whitespace).
    Returns 0 if the line is empty, 1 otherwise.
*/
int is_empty_line(char *line)
{
    while (*line != '\0')
    {
        if (!isspace(*line))
        {
            return 1; /* Not empty */
        }
        line++;
    }
    return 0;
}

/* Checks if a line is a commented line (starts with ';') */
int is_commented_line(char *line)
{
    if (*line == ';')
    {
        return 0; /* is commented line*/
    }
    return 1;
}

/*
    Validates if the operand is a valid integer
    Returns 0 if the operand is a valid integer, error code otherwise.
*/
int is_valid_integer(char *operand)
{
    ErrorCode error_flag = 0; /*assume success*/
    if ((operand == NULL) || (*operand == '\0'))
    { /* Operand is null or empty */
        error_flag = ERROR_OPERAND_IS_EMPTY;
        return error_flag;
    }
    if ((isdigit(*operand)) || (*operand != '+') || (*operand != '-'))
    { /* Ensure the first character is a digit or a sign ('+' or '-') */
        if (!isdigit(*operand) && strlen(operand) == 1)
        {
            error_flag = ERROR_NOT_VALID_INTEGER; /*so first char was only + or - */
            return error_flag;
        }
        operand++; /*move past first char*/
        while (*operand != '\0')
        {
            if (!isdigit(*operand))
            {
                error_flag = ERROR_NOT_VALID_INTEGER; /*found not integer char */
                return error_flag;
            }
            operand++; /*move to next char*/
        }
    }
    else
    {                                         /*if first char not in allowed chars*/
        error_flag = ERROR_NOT_VALID_INTEGER; /*founs not integer char */
    }
    return error_flag; /* 0 - > Success. Operand is a valid integer */
}

/*
    Validates if the value is a valid register name (r0 to r7).
    Returns 0 if the value is a valid register name, 1 otherwise.
*/
int valid_reg_name(char *value)
{
    if (strlen(value) == 2 && value[0] == 'r' && value[1] >= '0' && value[1] <= '7')
    {
        return 0; /*SUCCESS*/
    }
    return ERROR_WRONG_OPERAND_SYNTAX;
}

/*
    Check if a string is a opcode.
    Returns 1 if the string is an opcode, 0 otherwise.
*/
int check_if_opcode(char *name)
{
    int j = 0;
    while (j++ < OP_CODE_ARRAY_SIZE)
    {
        if (strcmp(name, OPCODES[j - 1].name) == 0)
        {
            return 1; /*found name in opcode table*/
        }
    }
    return 0; /*not opcode*/
}

/*
    Counts the occurrences of a character in a string.
    Returns the number of occurrences.
*/
int count_occurrences(const char *str, char ch)
{
    int count = 0;
    while (*str != '\0')
    {
        if (*str == ch)
        {
            count++;
        }
        str++;
    }
    return count;
}

/*
    Duplicates a string to a new allocated memory location.
*/
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
    if (duplicate_string == NULL)
    {
        handle_error(ERROR_MEMORY_ALLOCATION_FAILED, 0);
        return NULL;
    }
    memcpy(duplicate_string, src_string, len);
    return duplicate_string;
}

/*
    Checks if a string is valid (starts and ends with double quotes).
    Returns 0 if the string is valid, 1 otherwise.
*/
int is_valid_string(char *str)
{
    /* starts and ends with "" */
    if ((str[0] != '"') || (str[strlen(str) - 1] != '"') || (strlen(str) <= 2))
    {
        return ERROR_INVALID_STRING_SYNTAX;
    }
    return 0; /*success*/
}

char *trim_whitespace(char *str)
{
    /*function to move past whitespace values*/
    char *end = NULL;
    end = str + strlen(str) - 1;
    while (isspace((unsigned char)*str))
        str++; /*move past space in starting string*/
    /*Trim trailing space*/
    while (end > str && isspace((unsigned char)*end))
        end--;
    /*Write new null terminator character*/
    end[1] = '\0';
    return str; /*return pointer for the start of not space string*/
}

int calculate_L(int srcType, int dstType)
{
    int L = 1; /* at least 1, the base word */
    if (srcType == -1 && dstType == -1)
    {
        return L;
    }
    if ((srcType == 2 || srcType == 3) && (dstType == 2 || dstType == 3))
    {
        return L + 1;
    }
    if (srcType != -1)
    {
        L = L + 1;
    }
    if (dstType != -1)
    {
        L = L + 1;
    }
    return L;
}

void free_all(struct macros **macro_head, SymbolTable **symbolTable, BinaryLine **binary_table)
{
    if ((*macro_head) != NULL)
    {
        free_macros((*macro_head));
        (*macro_head) = NULL;
    }
    if ((*symbolTable) != NULL)
    {
        freeSymbolTable((*symbolTable));
        (*symbolTable) = NULL;
    }
    if ((*binary_table) != NULL)
    {
        free_BinaryLine((*binary_table));
        (*binary_table) = NULL;
    }
}