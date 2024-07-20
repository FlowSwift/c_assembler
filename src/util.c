#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

char *add_file_extension(char *filename, char *extension)
{
    char *new_filename = malloc(sizeof(char) * MAX_FILE_NAME);
    if (new_filename == NULL)
    {
        printf("MEM ERROR\n");
        return NULL; /* WHAT TO RETURN? */
    }
    strcpy(new_filename, filename);
    strcat(new_filename, extension);
    return new_filename;
}