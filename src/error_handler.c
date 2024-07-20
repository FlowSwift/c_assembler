#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"

void handle_error(ErrorCode error, int line_number) {
    switch (error) {
        case ERROR_FILE_NOT_FOUND:
            printf("Error: File not found.\n");
            break;
        case ERROR_CANT_WRITE_FILE:
            printf("Error: Permission denied.\n");
            break;
        case ERROR_MEMORY_ALLOCATION_FAILED:
            printf("Error: Memory allocation failed.\n");
            exit(1);
            break;
        case ERROR_COULD_NOT_STRIP_FILE:
            printf("Error: Could not strip file.\n");
            break;
        case ERROR_COULD_NOT_PROCESS_MACROS:
            printf("Error: Could not process macros.\n");
            break;
        case ERROR_LINE_TOO_LONG:
            printf("Error: Line too long on line %d.\n", line_number);
            break;
        case ERROR_INVALID_MACRO_NAME:
            printf("Error: Invalid macro name on line %d\n", line_number);
            break;
        case ERROR_MACRO_NAME_MISSING:
            printf("Error: Macro name missing. on line %d\n", line_number);
            break;
        case ERROR_INVALID_MACRO_DECLARATION:
            printf("Error: Invalid macro declaration on line %d\n", line_number);
            break;
        default:
            printf("Unknown error occurred.\n");
    }
}