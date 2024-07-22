#ifndef __LINE_PARSER_H__
#define __LINE_PARSER_H__

typedef struct Operand{
    /*every operand will have this structure */
    char *value; /*if register will be changed to number, if label will be -1*/
    int type; /*miun types - {0,1,2,3}, -1 if no operands */
} Operand;

typedef struct AssemblyLine {
    char *label;
    char *instruction;
    char *operands;
    Operand *srcOperand;
    Operand *destOperand;
} AssemblyLine;


/*Function prototypes: */
/*Parses to 3 parts the line it received from the file - 
    -label(optional)
    - instruction (in this stage doesnt matter which)
    - operands 
    this stage doesnt cheack if its legal, this is done in first_pass.c
     */
AssemblyLine parseAssemblyLine(const char *line);
void printAssemblyLine(const AssemblyLine *parsedLine);
void freeAssemblyLine(AssemblyLine *line);
void freeOperand(Operand *operand);

#endif /* __LINE_PARSER_H__ */