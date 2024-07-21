#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__
#include "line_parser.h"

typedef struct {
    int code;
    char *name;
    int numOfOperands;
} Opcode;

int firstPass(char *file_name, SymbolTable *symbolTable, int *IC, int *DC);
/*check is there is dot ("."). reurns 1 if yes and 0 if not. */
int is_directive(AssemblyLine* parsedLine);
/*parse operands by miun sigintures, change values inside Operand structure in AssemblyLine*/
int operand_parser(AssemblyLine* parsedLine);
/*checks type of miun for every operande*/
int check_type(Operand *operand);
/*count occurences of some char */
int countOccurrences(const char *str, char ch);

#endif /* __FIRST_PASS_H__ */