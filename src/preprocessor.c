#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lines.h"
#include "macros.h"
#include "util.h"
#include "preprocessor.h"
#include "error_handler.h"

/*
    TODO:
        Ignore comments
        Add comments
        Add start macro name check
*/

/*
    To do:
        Check macros name
*/

/* CHECK HOW IT RETURNS INT */
int pre_process(char *file_name)
{
    char *full_file_name; /* File name including extension */
    char *temp_file_name; /* Temp file */
    ErrorCode error_flag = 0;
    /* IMPROVE ERRORS */
    full_file_name = add_file_extension(file_name, ".as");
    temp_file_name = add_file_extension(file_name, ".tmp");
    if (strip_file(full_file_name, temp_file_name) != 0)
    {
        free(full_file_name);
        free(temp_file_name);
        error_flag = ERROR_COULD_NOT_STRIP_FILE;
        handle_error(error_flag, 0);
        return error_flag;
    }
    if (process_macros(file_name, temp_file_name) != 0)
    {
        free(full_file_name);
        free(temp_file_name);
        error_flag = ERROR_COULD_NOT_PROCESS_MACROS;
        handle_error(error_flag, 0);
        return error_flag;
    }
    free(full_file_name);
    free(temp_file_name);
    return 0;
}

/*
    Strips file of extra whitespace
    Returns 0 if successful
    Returns error number if error
*/
int strip_file(char *filename, char *stripped_file_name)
{
    FILE *stripped_file = NULL;
    FILE *file = NULL;
    char line[MAX_LINE_LENGTH];
    char stripped_line[MAX_LINE_LENGTH];
    int line_counter = 0;
    ErrorCode error_flag = 0;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        error_flag = ERROR_FILE_NOT_FOUND;
        handle_error(error_flag, 0);
        return error_flag;
    }
    stripped_file = fopen(stripped_file_name, "w");
    if (stripped_file == NULL)
    {
        fclose(file);
        error_flag = ERROR_CANT_WRITE_FILE;
        handle_error(error_flag, 0);
        return error_flag; /* IMPROVE ERRORS*/
    }
    /* Strip lines assuming the line length can't be over MAX_LINE_LENGTH including the extra whitespace*/
    while (fgets(line, MAX_LINE_LENGTH + 1, file) != NULL)
    {
        line_counter++;
        if (line[strlen(line) - 1] != '\n')
        {
            error_flag = ERROR_LINE_TOO_LONG;
            handle_error(error_flag, line_counter);
            continue;
        }
        else
        {
            strip_line(stripped_line, line);
            fprintf(stripped_file, "%s", stripped_line);
        }
    }
    fclose(file);
    fclose(stripped_file);
    if (error_flag != 0)
    {
        return error_flag;
    }
    return 0;
}

void strip_line(char *dest, char *source)
{
    int i = 0, j = 0;
    long len = strlen(source);
    if (source[i] == '\n')
    {
        return;
    }
    while (isspace(source[i]))
        i++;
    for (; i < len; i++)
    {
        if (isspace(source[i]) && isspace(source[i + 1]))
            continue;
        dest[j++] = source[i];
    }
    dest[j] = '\0';
    return;
}

int process_macros(char *filename, char *temp_file_name)
{
    FILE *file = NULL, *processed_file = NULL;
    char *am_file_name = NULL;
    char line[MAX_LINE_LENGTH];
    struct macros *head = NULL;
    struct macros *macro = NULL;
    int line_counter = 0;
    char *macr_pos = NULL;
    ErrorCode error_flag = 0;
    file = fopen(temp_file_name, "r");
    if (file == NULL)
    {
        error_flag = ERROR_FILE_NOT_FOUND;
        handle_error(error_flag, 0);
        return error_flag;
    }
    am_file_name = add_file_extension(filename, ".am");
    processed_file = fopen(am_file_name, "w");
    free(am_file_name);
    if (processed_file == NULL)
    {
        fclose(file);
        error_flag = ERROR_CANT_WRITE_FILE;
        handle_error(error_flag, 0);
        return error_flag;
    }
    while ((fgets(line, MAX_LINE_LENGTH + 1, file)) != NULL)
    {
        line_counter++;
        macr_pos = strstr(line, MACRO_START);
        if ((macro = is_existing_macro(head, line)) != NULL)
        {
            write_macro(macro, processed_file);
        }
        /*
            Check if found MACRO_START as the first word in the line
        */
        else if (macr_pos == line && macr_pos[strlen(MACRO_START)] == ' ')
        {
            if ((error_flag = validate_macro_name(macr_pos, line, line_counter)) == 0)
            {
                add_macro(file, processed_file, &head, line);
            }
            macr_pos = NULL;
        }
        else
        {
            fprintf(processed_file, "%s", line);
        }
    }
    free_macros(head);
    if (error_flag != 0)
    {
        return error_flag;
    }
    return 0;
}

/* TODO: ADD MACRO NAME CHECK */
int validate_macro_name(char *macr_ptr, char *line, int line_number)
{
    char macro_name[MAX_LINE_LENGTH];
    char temp[MAX_LINE_LENGTH];
    ErrorCode error_flag = 0;
    strcpy(temp, line);
    strtok(temp, " \t\n");
    strcpy(macro_name, strtok(NULL, " \t\n"));
    if (macro_name == NULL)
    {
        error_flag = ERROR_MACRO_NAME_MISSING;
        handle_error(error_flag, 0);
        return error_flag;
    }
    if (strtok(NULL, " \t\n") != NULL)
    {
        error_flag = ERROR_INVALID_MACRO_DECLARATION;
        handle_error(error_flag, 0);
        return error_flag;
    }
    return 0;
}

void add_macro(FILE *file, FILE *processed_file, struct macros **ptr_to_head, char *macro_ptr)
{
    char macro_name[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    struct macros *new_macro = create_macro_node(macro_name, ptr_to_head);
    struct lines *current_line = NULL;
    struct lines *last = NULL;
    /* get the macro name and copy to macro_name */
    strtok(macro_ptr, " \t\n");
    strcpy(macro_name, strtok(NULL, " \t\n"));
    while ((fgets(line, MAX_LINE_LENGTH + 1, file)) != NULL)
    {
        /* ADD ERROR CHECK TO MACRO_END */
        if (strstr(line, MACRO_END) != NULL)
        {
            return;
        }
        current_line = malloc_line(line);
        if (new_macro->lines == NULL)
        {
            new_macro->lines = current_line;
        }
        else
        {
            last->next = current_line;
        }
        last = current_line;
    }
    return;
}

void write_macro(struct macros *macro, FILE *file)
{
    struct lines *current_line = macro->lines;
    while (current_line != NULL)
    {
        fprintf(file, "%s", current_line->line);
        current_line = current_line->next;
    }
    return;
}