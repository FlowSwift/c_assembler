#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__
/*#define MAX_LABEL_LEN 31*/
#include "line_parser.h"
#include "symbol_table.h"
#include "binary_conversion.h"

typedef struct
{
    int code;
    char *name;
    int numOfOperands;
} Opcode;

typedef enum
{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
} Register;

extern Opcode OPCODES[];

/*first pass function - goes through each line, parses and makes to binary*/
int first_pass(char *file_name, struct macros *macro_head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC);
/*checks type of miun of each operand. changes value to name and updates in Operand*/
int check_type(Operand *operand, struct macros *macro_head);
/*gets opcode number of operands*/
int get_opcode_operands(char *instruction);
/*gets opcode code value*/
int get_opcode_code(char *instruction);
/*parses operands gor command line -> checks the insturction is valid(one of 16),
    gets type of miun for each operand and checks if they are allowed for the instruction.*/
int operand_parser(AssemblyLine *parsedLine, struct macros *macro_head);
int check_valid_operands(AssemblyLine *parsedLine);
/*checks if instruction is valid (by parse_operands func),
    insters to binary table.
    inserts label to Symbol Table if there is label*/
int handle_instruction(AssemblyLine *parsedLine, SymbolTable *symbol_table, BinaryLine **instruction_binary_table, int *IC, struct macros *macro_head, int line);
/*calculates L by miun types of Operands*/
int calculate_L(int srcType, int dstType);
int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line_number, int *DC);
int handleDataDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line, int *DC);
int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head);
int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head);
#endif /* __FIRST_PASS_H__ */