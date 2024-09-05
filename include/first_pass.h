#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__
/*#define MAX_LABEL_LEN 31*/
#include "line_parser.h"
#include "symbol_table.h"
#include "binary_conversion.h"

/**
 * @brief Performs the first pass on the assembly file.
 * The function foes through each line of the assembly code, parses it into AssemblyLine structure , by parseAssemblyLine function in line)parser.c.
 * If the line is valid it converts it into binary, by convert_instruction_to_binary_code function in binary_conversion.c.
 * Depends on the type of the Assembly line(directive or command) the function calls the relevent functions to continue parsing and validation. 
 * The function updates the symbol table with the symbols defined in the file.
 *
 * @param file_name The name of the assembly file to process.
 * @param macro_head A pointer to the head of the macro list.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table for instructions and directives.
 * @param IC Instruction counter, updated during processing. For direct instructions. 
 * @param DC Data counter, updated during processing. For .string and .data directives.
 *
 * @return 0 on success, or an error code on failure.
 */
int first_pass(char *file_name, struct macros *macro_head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC);

#endif /* __FIRST_PASS_H__ */