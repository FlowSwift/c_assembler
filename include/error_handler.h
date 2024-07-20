#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

typedef enum {
    ERROR_NONE = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_CANT_WRITE_FILE,
    ERROR_MEMORY_ALLOCATION_FAILED,
    ERROR_COULD_NOT_STRIP_FILE,
    ERROR_COULD_NOT_PROCESS_MACROS,
    ERROR_LINE_TOO_LONG,
    ERROR_INVALID_MACRO_NAME,
    ERROR_MACRO_NAME_MISSING,
    ERROR_INVALID_MACRO_DECLARATION,
} ErrorCode;

typedef struct Error {
    ErrorCode code;
    char *message;
} Error;

void handle_error(ErrorCode error, int line_number);
#endif /* __ERROR_CODES_H__ */