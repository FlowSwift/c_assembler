#ifndef __BINARY_CONVERSION_H__
#define __BINARY_CONVERSION_H__

#include "line_parser.h"
#include "symbol_table.h"

typedef struct BinaryLine
{
    short binary_code;
    char *label;
    int original_line_number;
    struct BinaryLine *next;
} BinaryLine;



void convert_instruction_to_binary_code(AssemblyLine *assembly_line, struct BinaryLine **head, int line);
int calc_opcode_binary(struct BinaryLine *line, int opcode);
int calc_miun_binary(struct BinaryLine *line, int miun);
void add_binary_line(struct BinaryLine *line, struct BinaryLine **head);
struct BinaryLine *malloc_BinaryLine(int line_number);
void free_BinaryLine(struct BinaryLine *line);
void decimal_to_binary(unsigned int a, char str[], int size);

#endif /* __BINARY_CONVERSION_H__ */