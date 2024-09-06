#ifndef __DIRECTIVE_HANDLER_H__
#define __DIRECTIVE_HANDLER_H__

#include "line_parser.h"
#include "symbol_table.h"
#include "binary_conversion.h"

/**
 * @brief Handles a `.string` directive line.
 * String directive lines structure is: <label:-optional> .string "<data>". 
 * The data can be any character as long it is between two "".
 * Function processes a string directive, converts it into binary code, and updates the data counter.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param directive_binary_table A pointer to the directive binary table.
 * @param line_number The line number being processed.
 * @param DC A pointer to the data counter.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_string_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line_number, int *DC);

/**
 * @brief Handles a `.data` directive line.
 * Data directive lines structure is: <label:-optional> .data <numeric values devided by `,`>.
 * The values must be whole numbers.
 * Function processes a data directive, validates the format,, converts the data into binary code and updates the data counter.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param directive_binary_table A pointer to the directive binary table.
 * @param line_number The line number being processed.
 * @param DC A pointer to the data counter.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_data_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line, int *DC);

/**
 * @brief Handles a `.extern` directive line.
 * Processes an extern directive, adds the symbol to the symbol table with default memory(0) and label type EXTERN.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table.
 * @param macro_head A pointer to the head of the macro list.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_extern_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head);

/**
 * @brief Handles a `.entry` directive line.
 * Processes an entry directive, adds the symbol to the symbol table with default memory(0) and label type ENTRY.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table.
 * @param macro_head A pointer to the head of the macro list.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_entry_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head);

#endif /* __DIRECTIVE_HANDLER_H__ */