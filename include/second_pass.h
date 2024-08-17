#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "symbol_table.h"
#include "binary_conversion.h"

int second_pass(char *filename, struct macros *head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC);

#endif /* __SECOND_PASS_H__ */