#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct AssemblyLine{
    /*every line will be parsed and inputed to this structure */
    Label *label; /*will be NULL at first*/
    char *instruction;
    char *operands;
    Operand *srcOperand;
    Operand *destOperand;
}AssemblyLine;

typedef struct SymbolNode{
    /*every label will have this structure after it is checked that it is legal definition */
    char name;
    int type; /*entry, extern, new defenition*/
    int *memory_place;
}SymbolNode;

typedef struct SymbolTable{
    /*linked list of all the symbols*/
    SymbolNode *head;
    SymbolNode *last;
} SymbolTable;

typedef struct Operand{
    /*every operand will have this structure */
    char name;
    int type; /*miun types */
    int memory_place;
    int is_label;
} Operand;

/*Function prototypes: */
/*Parses to 3 parts the line it received from the file - 
    -label(optional)
    - instruction (in this stage doesnt matter which)
    - operands 
    this stage doesnt cheack if its legal, this is done in first_pass.c
     */
AssemblyLine* parseAssemblyLine(const char *line);
void printAssemblyLine2(const AssemblyLine *parsedLine);
void freeAssemblyLine(AssemblyLine *line);
void freeOperand(Operand *operand);