#ifndef __BINARY_CONVERSION_H__
#define __BINARY_CONVERSION_H__

#include "line_parser.h"
#include "symbol_table.h"

enum operator_type
{
    DEST_OPERAND = 0,
    SRC_OPERAND = 1
};

typedef struct BinaryLine
{
    short binary_code;
    char *label;
    int original_line_number;
    int decimal_memory_address;
    struct BinaryLine *next;
} BinaryLine;

void convert_instruction_to_binary_code(AssemblyLine *assembly_line, struct BinaryLine **head, int line, int *IC);
BinaryLine *convert_word(AssemblyLine *assembly_line, int line, int miun_type, int operand_type, int disable_third_word, int IC);
BinaryLine *convert_directive_to_binary_code(int value, int line, int DC);
int calc_miun_binary(struct BinaryLine *line, int miun);
void add_binary_lines(struct BinaryLine *line, struct BinaryLine **head);
struct BinaryLine *malloc_BinaryLine(int line_number, int decimal_memory_address);
void free_BinaryLine(struct BinaryLine *line);
void decimal_to_binary(unsigned int num, char str[], int size);

#endif /* __BINARY_CONVERSION_H__ */