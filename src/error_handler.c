#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"

void handle_error(ErrorCode error, int line_number)
{
    switch (error)
    {
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
    case ERROR_SYMBOL_DEFINED_TWICE:
        printf("Error: Symbol defined more than once on line %d\n", line_number);
        break;
    case ERROR_ENTRY_DEFINED_TWICE:
        printf("Error: Entry defined more than once on line %d\n", line_number);
        break;
    case ERROR_EXTERN_DEFINED_TWICE:
        printf("Error: Extern defined more than once on line %d\n", line_number);
        break;
    case ERROR_SYMBOL_SYNTAX_IS_WRONG:
        printf("Error: Symbol syntax is wrong.  %d\n", line_number);
        break;
    case ERROR_SYMBOL_TOO_LONG:
        printf("Error: Symbol syntax problem - symbol too long. on line %d\n", line_number);
        break;
    case ERROR_SYMBOL_NAME_IS_OPCODE:
        printf("Error: Symbol name is not allowed - is Opcode, on line %d\n", line_number);
        break;
    case ERROR_SYMBOL_NAME_IS_REGISTER:
        printf("Error: Symbol name is not allowed - is Register, on line %d\n", line_number);
        break;
    case ERROR_SYMBOL_NAME_IS_MACRO:
        printf("Error: Symbol name is not allowed - Macro, on line %d\n", line_number);
        break;
    case ERROR_WRONG_DATA_DIRECTIVE_SYNTAX:
        printf("Error: .data directive syntax is not valid: should receive only integers with comma's between, on line %d\n", line_number);
        break;
    case ERROR_NOT_DEFINED_DIRECTIVE:
        printf("Error: Directive is not valid (should be one of .data, .string, .entry or .extern): on line %d\n.", line_number);
        break;
    case ERROR_MEMORY_OVERFLOW:
        printf("Error: Memory Overflow.");
        break;
    case ERROR_STRING_SYNTAX_NOT_VALID:
        printf("Error: .string directive syntax is not valid: should be quotantion and not empty, on line %d\n.", line_number);
        break;
    case ERROR_EXTERN_NOT_VALID:
        printf("Error: .extern directive syntax is not valid: has label definition, on line %d\n.", line_number);
        break;
    case ERROR_ENTRY_NOT_VALID:
        printf("Error: .entry directive syntax is not valid: has label definition, on line %d\n.", line_number);
        break;
    case ERROR_INSTRUCTION_NOT_VALID:
        printf("ERROR: Instruction is not valid, on line %d\n.", line_number);
        break;
    case ERROR_NOT_ENOUGH_OPERANDS:
        printf("ERROR: Not enough operands for instruction, on line %d\n.", line_number);
        break;
    case ERROR_OPERAND_IS_EMPTY:
        printf("ERROR: Operand is empty, on line %d\n.", line_number);
        break;
    case ERROR_NOT_VALID_INTEGER:
        printf("ERROR: Operand not valid Inetger, on line %d\n.", line_number);
        break;
    case ERROR_MACRO_NAME_EXISTS:
        printf("ERROR: Macro name exists, on line %d\n.", line_number);
        break;
    case ERROR_MACRO_NAME_IS_OPCODE:
        printf("ERROR: Macro name is Opcode, on line %d\n.", line_number);
        break;
    case ERROR_TOO_MANY_OPERANDS:
        printf("ERROR: Too many operands for instruction, on line %d\n.", line_number);
        break;
    case ERROR_SYMBOL_SHORT:
        printf("ERROR: Symbol is too short, on line %d\n.", line_number);
        break;
    case ERROR_MIUN_TYPES_DONT_MATCH:
        printf("ERROR: Miun types don't match what is allowed for instruction, on line %d\n.", line_number);
        break;
    default:
        printf("Unknown error occurred.\n");
    }
}
