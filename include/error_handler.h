#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__


/**
 * @enum ErrorCode
 * @brief An enumeration representing different error codes used in the assembler
 * This enum defines various error and warning codes that may occur during the assembly process,
 * such as file handling errors, syntax issues, and memory allocation problems.
 * Every error has a case that will print the Error, explenation and line number.
 * The code 0 will be assummed as Success.
 */
typedef enum
{
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
    ERROR_PARSE_LINE_FAILED,
    ERROR_INVALID_INSTRUCTION_NAME,
    ERROR_SYMBOL_DEFINED_TWICE,
    ERROR_ENTRY_DEFINED_TWICE,
    ERROR_EXTERN_DEFINED_TWICE,
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
    ERROR_EXTERN_WAS_DEFINED,
    ERROR_ENTRY_WAS_NOT_DEFINED,
    ERROR_INSTRUCTION_NOT_VALID,
    ERROR_NOT_ENOUGH_OPERANDS,
    ERROR_OPERAND_IS_EMPTY,
    ERROR_NOT_VALID_INTEGER,
    ERROR_INTEGER_VALUE_TOO_BIG,
    ERROR_INTEGER_VALUE_TOO_SMALL,
    ERROR_MACRO_NAME_EXISTS,
    ERROR_MACRO_NAME_IS_OPCODE,
    ERROR_TOO_MANY_OPERANDS,
    ERROR_MIUN_TYPES_DONT_MATCH,
    ERROR_SYMBOL_SHORT,
    ERROR_WRONG_OPERAND_SYNTAX,
    ERROR_CANT_DEFINE_EXTERN,
    ERROR_SYMBOL_DEF_ERROR,
    ERROR_SYMBOL_ALREADY_EXTERN,
    ERROR_SYMBOL_ALREADY_ENTRY,
    ERROR_SYMBOL_WAS_NOT_DEFINED,
    ERROR_INVALID_STRING_SYNTAX,
    ERROR_INVALID_STRING_CHAR,
    ERROR_MACRO_NAME_IS_REGISTER,
    ERROR_MACRO_NAME_IS_RESERVED,
    ERROR_SYMBOLS_NOT_VALID,
    ERROR_FAILED_ENT_FILE,
    ERROR_FAILED_EXT_FILE, 
    ERROR_SOME_MACRO_NOT_VALID
} ErrorCode;


/**
 * @struct Error
 * @brief A structure representing an error with an error code and a message to print.
 * This structure contains an `ErrorCode` and an associated error message to describe the error.
 *
 * @var Error::code
 * The error code representing the type of error.
 *
 * @var Error::message
 * A human-readable error message describing the error.
 */
typedef struct Error
{
    ErrorCode code;
    char *message; /*delete?*/
} Error;


/**
 * @brief Handles and displays error messages based on the provided error code.
 * This function takes an error code and the line number where the error occurred, 
 * then prints an appropriate error message to the console. 
 * Some errors as Memory Allocation error, may terminate the program.
 *
 * @param error The error code of type `ErrorCode`.
 * @param line_number The line number where the error occurred (if applicable).
 */
void handle_error(ErrorCode error, int line_number);
#endif /* __ERROR_CODES_H__ */