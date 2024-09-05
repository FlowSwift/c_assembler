#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"
#include "error_handler.h"
#include "line_parser.h"
#include "first_pass.h"
#include "binary_conversion.h"
#include "util.h"
#include "constant.h"

void convert_instruction_to_binary_code(AssemblyLine *assembly_line, struct BinaryLine **head, int line, int *IC)
{
    struct BinaryLine *lines = malloc_BinaryLine(line, *IC); /* the head of the 1-3 words */
    struct BinaryLine *current = lines; /* current word */
    int disable_third_word = 0; /* disable third word when src and dest miun are both of type 2/3 */
    int L; /*amount of words in Binary Line*/
    /*initialize miun types */
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
    L = calculate_L(src_type, dest_type); /*ca,cultes the number of words that will be in binary form.*/
    printf("L AMOUNT %d\n", L); /*delete*/
    printf("IC BEFORE: %d\n", *IC); /*delete*/
    printf("IC AFTER: %d\n", *IC); /*delete*/
    /* Create the first word of the instruction, there is always at least 1 word */
    current->binary_code |= (1 << BITWISE_SHIFT_LEFT_A);                                 /* set A to 1 */
    current->binary_code |= ((assembly_line->opcode_code) << BITWISE_SHIFT_LEFT_OPCODE); /* set the opcode */
    /* Check operands */
    if (dest_type != -1)
    {
        /* If there is a dest operand set the miun type */
        current->binary_code |= (calc_miun_binary(dest_type) << BITWISE_SHIFT_LEFT_DEST_MIUN);
    }
    if (src_type != -1)
    {
        /* If there is a source operand set the miun type */
        current->binary_code |= (calc_miun_binary(src_type) << BITWISE_SHIFT_LEFT_SRC_MIUN);
    }
    /* Increment instruction count */
    *IC += 1;
    /* If both dest and src operands are of type 2/3, set to true */
    disable_third_word = (((dest_type == 2) || (dest_type == 3)) && ((src_type == 2) || (src_type == 3)));
    if (L >= 2)
    {
        current->next = convert_word(assembly_line, line, (L == 3 ? src_type : dest_type), (L == 3 ? SRC_OPERAND : DEST_OPERAND), disable_third_word, *IC);
        current = current->next;
        /* Increment instruction count */
        *IC += 1;
    }
    if (L == 3)
    {
        current->next = convert_word(assembly_line, line, dest_type, DEST_OPERAND, disable_third_word, *IC);
        current = current->next;
        /* Increment instruction count */
        *IC += 1;
    }
    /* append the 1-3 words block to the binary table head*/
    add_binary_lines(lines, head);
    return;
}

BinaryLine *convert_word(AssemblyLine *assembly_line, int line, int miun_type, enum operand_type operand_type, int disable_third_word, int IC)
{
    BinaryLine *binary_line = malloc_BinaryLine(line, IC);
    char *operand = (operand_type == DEST_OPERAND) ? (assembly_line->destOperand->value) : (assembly_line->srcOperand->value);
    if (miun_type == 0)
    {
        binary_line->binary_code |= (1 << BITWISE_SHIFT_LEFT_A);                    /* set A to 1 */
        binary_line->binary_code |= (atoi(operand) << BITWISE_SHIFT_LEFT_NUM_WORD); /* set the number */
    }
    else if (miun_type == 1)
    {
        binary_line->label = strdup1(operand); /* set the label member to the label name */
        printf("LABEL@@@@@@@@@: %s\n", binary_line->label); /*delete*/
    }
    else if (miun_type == 2 || miun_type == 3)
    {
        binary_line->binary_code |= (1 << BITWISE_SHIFT_LEFT_A);                                                                                                 /* set A to 1 */
        binary_line->binary_code |= (operand[1] - '0') << ((operand_type == DEST_OPERAND) ? BITWISE_SHIFT_LEFT_DEST_REGISTER : BITWISE_SHIFT_LEFT_SRC_REGISTER); /* set the register */
        /* If the operand is of type 3, set the second register */
        if (disable_third_word == 1)
        {
            binary_line->binary_code |= (assembly_line->srcOperand->value[1] - '0') << BITWISE_SHIFT_LEFT_SRC_REGISTER;
        }
    }
    return binary_line;
}


BinaryLine *convert_directive_to_binary_code(int value, int line, int DC)
{
    BinaryLine *binary_line = malloc_BinaryLine(line, DC);
    binary_line->binary_code = value; /*set the value, number or character in ASCII representation*/
    return binary_line;
}


int calc_miun_binary(int miun)
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


void add_binary_lines(struct BinaryLine *line, struct BinaryLine **head)
{
    struct BinaryLine *current = NULL;
    if (*head == NULL) /*if new Binary line*/
    {
        *head = line;
        return;
    }
    current = *head;
    while (current->next != NULL) /*add to previous line*/
    {
        current = current->next;
    }
    current->next = line;
}


struct BinaryLine *malloc_BinaryLine(int line_number, int decimal_memory_address)
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
    line->decimal_memory_address = decimal_memory_address;
    return line;
}

void convert_label_adress_to_binary_line(BinaryLine *binary_line, SymbolNode *symbol)
{
    if (symbol->label_type == TYPE_EXTERN) /*if label is extern than symbol is shifter to E*/
    {
        binary_line->binary_code = (1 << BITWISE_SHIFT_LEFT_E);
    }
    else /*if label is not extern than symbol is shifter to R*/
    {
        binary_line->binary_code = (1 << BITWISE_SHIFT_LEFT_R);
        binary_line->binary_code |= ((symbol->memory_place) << BITWISE_SHIFT_LEFT_ADDRESS);
    }
}


void free_BinaryLine(struct BinaryLine *line)
{
    while (line != NULL)
    {
        struct BinaryLine *temp = line;
        line = line->next;
        if (temp->label != NULL)
        {
            free(temp->label);
        }
        free(temp);
    }
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


void decimal_to_octal(unsigned int num, char str[], int size)
{
    int i = size - 2;     /* set i to the index before \0 */
    str[size - 1] = '\0'; /* set a null terminator */
    while (i >= 0)
    {
        str[i] = (num % 8) + '0';
        num /= 8;
        i--;
    }
    return;
}