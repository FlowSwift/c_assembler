#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


int isEmptyLine(char *line)
{
    while (*line != '\0') {
        if (!isspace(*line)) {
            return 0;  /* Not empty */
        }
        line++; 
    }
    return 1;
}

int isCommentedLine(char *line)
{
    if (*line == ';') {
        return 1; /* Commented line*/
    }
    return 0;
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

char *strdup(char *src_string)
{
    size_t len;
    char *duplicate_string = NULL;

    if (src_string == NULL)
    {
        return NULL;
    }

    len = strlen(src_string) + 1;
    duplicate_string = malloc(len);

    if (duplicate != NULL)
    {
        memcpy(duplicate_string, src_string, len);
    }
    return duplicate_string;
}