#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__
/*#define MAX_LABEL_LEN 31*/
#include "line_parser.h"

typedef struct {
    int code;
    char *name;
    int numOfOperands;
} Opcode;

/*first pass function - goes through each line, parses and makes to binary*/
int firstPass(char *file_name, SymbolTable *symbolTable,BinaryTable *BinaryTable, int *IC, int *DC)
/*checks type of miun of each operand. changes value to name and updates in Operand*/
int check_type(Operand *operand);
/*gets opcode number of operands*/
int get_opcode_operands(AssemblyLine* parsedLine);
/*gets opcode code value*/
int get_opcode_code(AssemblyLine* parsedLine);
/*cheks if operand is legit integer: +789, -9, 8 are allowed*/
int is_valid_integer(char *operand);
/*parses operands gor command line -> checks the insturction is valid(one of 16), 
    gets type of miun for each operand and checks if they are allowed for the instruction.*/
int operand_parser(AssemblyLine* parsedLine);
/*checks if instruction is valid (by parse_operands func), 
    insters to binary table.
    inserts label to Symbol Table if there is label*/
int handle_instruction(AssemblyLine* parsedLine,SymbolTable *symbol_table,BinaryTable *binary_table, *IC);
/*checks if the register is valid*/
int valid_reg_name(char *value);
/*calculates L by miun types of Operands*/
int calculate_L(int srcType, int dstType);

#endif /* __FIRST_PASS_H__ */