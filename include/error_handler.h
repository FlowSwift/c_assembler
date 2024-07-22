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
    ERROR_INVALID_INSTRUCTION_NAME,
    ERROR__SYMBOL_DEFINED_TWICE,
    ERROR_SYMBOL_SYNTAX_IS_WRONG,
    ERROR_SYMBOL_TOO_LONG,
    ERROR_SYMBOL_NAME_IS_OPCODE,
    ERROR_SYMBOL_NAME_IS_REGISTER,
    ERROR_SYMBOL_NAME_IS_MACRO,
    ERROR_WRONG_DATA_DIRECTIVE_SYNTAX, 
    ERROR_MEMORY_OVERFLOW, 
    ERROR_NOT_DEFINED_DIRECTIVE,
    ERROR_STRING_SYNTAX_NOT_VALID,
    ERROR_EXTERN_NOT_VALID,
    ERROR_ENTRY_NOT_VALID,
    ERROR_INSTRUCTION_NOT_VALID,
    ERROR_NOT_ENOUGH_OPERANDS,
    ERROR_OPERAND_IS_EMPTY,
    ERROR_NOT_VALID_INTEGER,
} ErrorCode;

typedef struct Error {
    ErrorCode code;
    char *message;
} Error;

void handle_error(ErrorCode error, int line_number);
#endif /* __ERROR_CODES_H__ */