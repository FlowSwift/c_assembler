#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 
    Current state:
        Strips file of extra whitespace
        Checks for macros
        Adds macros to linked list
        Writes .am file without macros (switch to tmp?)
*/
/*
    TODO:
        Add error handling
        Add comments
        Split into multiple files
        Add start macro name check
        Add end macro name check
*/

#define MAX_LINE_LENGTH 81   /* Max 80 chars + \n */
#define MAX_FILE_NAME 100    /* Assuming file name no bigger than 100 chars */
#define MACRO_START "macr"   /* Macro start keyword */
#define MACRO_END "end_macr" /* Macro end keyword */
struct macros
{
    char name[MAX_LINE_LENGTH];
    struct lines *lines;
    struct macros *next;
};

struct lines
{
    char line[MAX_LINE_LENGTH];
    struct lines *next;
};

FILE *pre_process(char *file_name);
int strip_file(char *filename, char *temp_file_name);
void strip_line(char *dest, char *source);
FILE *process_macros(char *filename, char *temp_file_name);
int validate_macro_name(char *macr_ptr, char *line);
void add_macro(FILE *file, FILE *processed_file, struct macros **ptr_to_head, char *macro_ptr);
struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head);
struct lines *malloc_line(char *line);
char *add_file_extension(char *filename, char *extension);
int hash(char *str);

/*
    To do:
        Check macros name
*/
int main(int argc, char *argv[])
{
    pre_process(argv[1]);
    return 0;
}

FILE *pre_process(char *file_name)
{
    char *full_file_name; /* File name including extension */
    char *temp_file_name; /* File name without extension */
    /* IMPROVE ERRORS */
    full_file_name = add_file_extension(file_name, ".as");
    temp_file_name = add_file_extension(file_name, ".tmp");
    strip_file(full_file_name, temp_file_name);
    process_macros(file_name, temp_file_name);
    return 0;
}

int strip_file(char *filename, char *stripped_file_name)
{
    FILE *stripped_file = NULL;
    FILE *file = NULL;
    char line[MAX_LINE_LENGTH];
    char stripped_line[MAX_LINE_LENGTH];
    int error_flag = 0;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening source file");
        return 1; /* IMPROVE ERRORS*/
    }
    stripped_file = fopen(stripped_file_name, "w");
    if (stripped_file == NULL)
    {
        fclose(file);
        printf("Error opening temporary file to write to");
        return 1; /* IMPROVE ERRORS*/
    }
    /* Strip lines assuming the line length can't be over MAX_LINE_LENGTH including the extra whitespace*/
    while (fgets(line, MAX_LINE_LENGTH + 1, file) != NULL)
    {
        if (line[strlen(line) - 1] != '\n')
        {
            error_flag = 1;
            printf("Line too long\n");
            return 1; /* IMPROVE ERRORS*/
        }
        else
        {
            strip_line(stripped_line, line);
            fprintf(stripped_file, "%s", stripped_line);
        }
    }
    fclose(file);
    fclose(stripped_file);
    if (error_flag)
    {
        return error_flag; /* IMPROVE ERRORS*/
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

FILE *process_macros(char *filename, char *temp_file_name)
{
    FILE *file = NULL, *processed_file = NULL;
    char line[MAX_LINE_LENGTH];
    struct macros *head = NULL;
    char *macr_pos = NULL;
    file = fopen(temp_file_name, "r");
    if (file == NULL)
    {
        printf("Error opening file");
        return NULL; /* IMPROVE ERRORS */
    }
    processed_file = fopen(add_file_extension(filename, ".am"), "w");
    if (processed_file == NULL)
    {
        fclose(file);
        printf("Error opening file to write to");
        return NULL; /* IMPROVE ERRORS */
    }
    while ((fgets(line, MAX_LINE_LENGTH + 1, file)) != NULL)
    {
        macr_pos = strstr(line, MACRO_START);
        if (macr_pos != NULL)
        {
            if (validate_macro_name(macr_pos, line))
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
    return file;
}

/* TODO: ADD MACRO NAME CHECK */
int validate_macro_name(char *macr_ptr, char *line)
{
    char macro_name[MAX_LINE_LENGTH];
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);
    if ((macr_ptr != line) && (strstr(line, MACRO_END) != NULL))
    {
        printf("Macro definition must start at the beginning of the line\n");
        return 0; /* IMPROVE ERRORS */
    }
    if (strcmp(strtok(macr_ptr, " \t\n"), MACRO_START) != 0)
    {
        printf("Macro definition must start with 'macr macr_name'\n");
        return 0; /* IMPROVE ERRORS */
    }
    strcpy(macro_name, strtok(NULL, " \t\n"));
    if (macro_name == NULL)
    {
        printf("Macro name missing\n");
        return 0; /* IMPROVE ERRORS */
    }
    if (strtok(NULL, " \t\n") != NULL)
    {
        printf("Macro name must be a single word\n");
        return 0; /* IMPROVE ERRORS */
    }
    strcpy(line, temp);
    return 1;
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

    struct macros *create_macro_node(char *macro_name, struct macros **ptr_to_head)
    {
        struct macros *head = *ptr_to_head;
        struct macros *new_macro = malloc(sizeof(struct macros));
        strcpy(new_macro->name, macro_name);
        new_macro->lines = NULL;
        new_macro->next = NULL;
        if (*ptr_to_head == NULL)
        {
            *ptr_to_head = new_macro;
            return new_macro;
        }
        while (head->next != NULL)
        {
            head = head->next;
        }
        head->next = new_macro;
        return new_macro;
    }

    /* ASSUMES CURRENT LINE */
    struct lines *malloc_line(char *line)
    {
        struct lines *new_line = malloc(sizeof(struct lines));
        if (new_line == NULL)
        {
            printf("MEM ERROR\n");
            return NULL; /* WHAT TO RETURN? */
        }
        strcpy(new_line->line, line);
        new_line->next = NULL;
        return new_line;
    }
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
    int hash(char *str)
    {
        int hash = 53;
        while (*str)
        {
            hash += (*str++) % hash;
        }
        return hash;
    }