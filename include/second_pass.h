#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "symbol_table.h"
#include "binary_conversion.h"

int second_pass(char *file_name, struct macros *head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC);
int validate_symbols(SymbolTable *symbolTable);
int create_ent_file(char *file_name, SymbolTable *symbolTable);
int create_ext_file(char *file_name, SymbolTable *symbolTable, BinaryLine **binary_table);

#endif /* __SECOND_PASS_H__ */