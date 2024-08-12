#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handler.h"
#include "line_parser.h"
#include "first_pass.h"
#include "binary_conversion.h"
#include "util.h"
#include "constant.h"

void convert_instruction_to_binary_code(AssemblyLine *assembly_line, struct BinaryLine **head, int line, int *IC)
{
    struct BinaryLine *lines = malloc_BinaryLine(line);
    struct BinaryLine *current = lines;
    int extra_word = 1;
    current->next = malloc_BinaryLine(line);
    char *label = NULL;
    char *instruction = NULL;
    int src_type = -1;
    int dest_type = -1;
    if (assembly_line->srcOperand != NULL)
    {
        src_type = assembly_line->srcOperand->type;
    }
    if (assembly_line->destOperand != NULL)
    {
        dest_type = assembly_line->destOperand->type;
    }
    int L = calculate_L(src_type, dest_type);
    printf("L AMOUNT %d\n", L);
    *IC += L;
    /* Instruction */
    current->binary_code |= (1 << BITWISE_SHIFT_LEFT_A);
    current->binary_code |= ((assembly_line->opcode_code) << BITWISE_SHIFT_LEFT_OPCODE);
    /* Check operands */
    printf("%d %d\n", src_type, dest_type);
    printf("%d\n", assembly_line->opcode_code);
    if (dest_type != -1)
    {
        current->binary_code |= (calc_miun_binary(current, dest_type) << BITWISE_SHIFT_LEFT_DEST_MIUN);
    }
    if (src_type != -1)
    {
        current->binary_code |= (calc_miun_binary(current, src_type) << BITWISE_SHIFT_LEFT_SRC_MIUN);
    }
    if (L >= 2)
    {
        current->next = malloc_BinaryLine(line);
        current = current->next;
        if (dest_type == 0)
        {
            current->binary_code |= (1 << BITWISE_SHIFT_LEFT_A);
            current->binary_code |= (atoi(assembly_line->destOperand->value) << BITWISE_SHIFT_LEFT_NUM_WORD);
        }
        else if (dest_type == 1)
        {
            current->label = strdup1(assembly_line->destOperand->value);
        }
        else if (dest_type == 2 || dest_type == 3)
        {
            current->binary_code |= (1 << BITWISE_SHIFT_LEFT_A);
            current->binary_code |= atoi(assembly_line->destOperand->value[1]) << BITWISE_SHIFT_LEFT_DEST_REGISTER;
            if (src_type == 2 || src_type == 3)
            {
                current->binary_code |= atoi(assembly_line->srcOperand->value[1]) << BITWISE_SHIFT_LEFT_SRC_REGISTER;
                extra_word = 0;
            }
        }
    }
    if (L >= 3) {
        
    }
    add_binary_line(lines, head);
    return;
}



int calc_miun_binary(struct BinaryLine *line, int miun)
{
    if (miun == 0)
    {
        return (1 << 0);
    }
    else if (miun == 1)
    {
        return (1 << 1);
    }
    else if (miun == 2)
    {
        return (1 << 2);
    }
    else if (miun == 3)
    {
        return (1 << 3);
    }
    return -1;
}

void add_binary_line(struct BinaryLine *line, struct BinaryLine **head)
{
    if (*head == NULL)
    {
        *head = line;
        return;
    }
    struct BinaryLine *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = line;
}

struct BinaryLine *malloc_BinaryLine(int line_number)
{
    struct BinaryLine *line = NULL;
    line = (struct BinaryLine *)malloc(sizeof(struct BinaryLine));
    if (line == NULL)
    {
        handle_error(ERROR_MEMORY_ALLOCATION_FAILED, 0);
        return NULL;
        return 0;
    }
    line->binary_code = 0;
    line->label = NULL;
    line->next = NULL;
    line->original_line_number = line_number;
    return line;
}

void free_BinaryLine(struct BinaryLine *line)
{
    if (line == NULL)
    {
        return;
    }
    free(line->label);
    free(line);
}

void decimal_to_binary(unsigned int num, char str[], int size)
{
    char currentBinDigit;
    int i = size - 2;     /* set i to the index before \0 */
    str[size - 1] = '\0'; /* set a null terminator */
    /*
        run a loop checking each bit from the left side to the right.
        (a & (1u << (i))) will check the bit in the current index, if the bit == 1 then modify str accordingly
    */
    while (i >= 0)
    {
        (num & (1u << (i))) ? (currentBinDigit = '1') : (currentBinDigit = '0');

        str[size - 2 - i] = currentBinDigit;
        i--;
    }
    return;
}