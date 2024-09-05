#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "symbol_table.h"
#include "binary_conversion.h"

/**
 * @brief Performs the second pass on the assembly file.
 * This function processes the assembly file in the second pass after all lines are already in binary form except labels.
 * The function validates that symbols used in commands as Operands are defined or added as .extern. 
 * The function also checks if every entry is defined by checking the memory address. 
 * After validation and binary conversions of the labels, the function writes the binary output to the object file.
 * It also generates the `.ent` and `.ext` files if are found in the file.
 * The output `.ent` and `.ext` files hold the memory adress where the entry was defined and extern for where the extern label was used. 
 *
 * @param file_name The name of the file to process.
 * @param head A pointer to the head of the macro list.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table containing the parsed lines.
 * @param IC Instruction counter, used to track memory usage of command lines.
 * @param DC Data counter, used to track memory usage of directive lines.
 *
 * @return 0 on success, or an error code on failure.
 */
int second_pass(char *file_name, struct macros *head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC);

/**
 * @brief Validates the symbols in the symbol table.
 * Function ensures that all entry symbols have been defined and that extern symbols are not defined.
 *
 * @param symbolTable A pointer to the symbol table.
 *
 * @return 0 if all symbols are valid, or an error code if any errors are found.
 */
int validate_symbols(SymbolTable *symbolTable);

/**
 * @brief Creates the `.ent` file for entry symbols in file.
 * The `.ent` file contains all symbols defined as entries along with their memory addresses(address where the label was defined).
 *
 * @param file_name The name of the assembly file (without extension).
 * @param symbolTable A pointer to the symbol table.
 *
 * @return 0 on success, or an error code if the file cannot be created.
 */
int create_ent_file(char *file_name, SymbolTable *symbolTable);

/**
 * @brief Creates the `.ext` file for extern symbols.
 * The `.ext` file contains all symbols defined as externs along with their memory addresses in the binary table.
 * Memory address is where the extern label was used in file.
 *
 * @param file_name The name of the assembly file (without extension).
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table containing the parsed instructions.
 *
 * @return 0 on success, or an error code if the file cannot be created.
 */
int create_ext_file(char *file_name, SymbolTable *symbolTable, BinaryLine **binary_table);

#endif /* __SECOND_PASS_H__ */