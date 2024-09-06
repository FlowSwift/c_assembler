#ifndef __BINARY_CONVERSION_H__
#define __BINARY_CONVERSION_H__

#include "line_parser.h"
#include "symbol_table.h"

/**
 * @enum operand_type
 * @brief Enumeration representing the type of operand (source or destination).
 *
 * Used to differentiate between the source operand and the destination operand when generating binary word.
 *
 * @var DEST_OPERAND
 * Represents the destination operand.
 *
 * @var SRC_OPERAND
 * Represents the source operand.
 */
enum operand_type
{
    DEST_OPERAND = 0,
    SRC_OPERAND = 1
};

/**
 * @struct BinaryLine
 * @brief A structure representing a single line of binary code.
 * This structure is used to store the binary representation of an command line or directive, the original line number and memory address.
 *
 * @var BinaryLine::binary_code
 * The binary code representing the command or directive.
 *
 * @var BinaryLine::label
 * The label associated with this binary line, if any.
 * If the Operand is label it will be saved in file as the label name until the second pass. 
 *
 * @var BinaryLine::original_line_number
 * The line number from the original assembly code.
 *
 * @var BinaryLine::decimal_memory_address
 * The memory address of the instruction or directive in decimal form.
 *
 * @var BinaryLine::next
 * A pointer to the next binary line in the linked list.
 */
typedef struct BinaryLine
{
    short binary_code;
    char *label;
    int original_line_number;
    int decimal_memory_address;
    struct BinaryLine *next;
} BinaryLine;

/**
 * @brief Converts an assembly command to binary code and adds it to the binary table.
 *
 * This function processes an instruction parsed line in AssemblyLine struct and convert an instruction to binary.
 * Adds 1-3 words(in binary form) to the binary table head
 *
 * @param assembly_line A pointer to the parsed assembly line structure.
 * @param head A pointer to the head of the binary table.
 * @param line The original line number of the instruction.
 * @param IC A pointer to the instruction counter (memory address).
 */
void convert_instruction_to_binary_code(AssemblyLine *assembly_line, struct BinaryLine **head, int line, int *IC);

/**
 * @brief Converts an operand to binary code for a second or third word of an instruction.
 * This function generates the binary code for an operand and creates the appropriate binary word in BinaryLine struct.
 *
 * @param assembly_line A pointer to the parsed assembly line structure.
 * @param line The original line number of the instruction.
 * @param miun_type The address type of the operand(miun).
 * @param operand_type The type of operand (source or destination).
 * @param disable_third_word A flag to indicate whether the third word should be disabled.
 * @param IC Instruction counter (memory address of the instruction).
 *
 * @return A pointer to a BinaryLine structure representing the operand in binary form.
 */
BinaryLine *convert_word(AssemblyLine *assembly_line, int line, int miun_type, enum operand_type operand_type, int disable_third_word, int IC);

/**
 * @brief Converts a directive line to binary code.
 * This function generates the binary code for a directive and creates the appropriate binary word in BinaryLine struct.
 * The value of the directive is a number or character.
 *
 * @param value The value of the directive (e.g., a number or ASCII character).
 * @param line The original line number of the directive.
 * @param DC Data counter (memory address of directive line).
 *
 * @return A pointer to a BinaryLine structure representing the directive in binary form.
 */
BinaryLine *convert_directive_to_binary_code(int value, int line, int DC);

/**
 * @brief Calculates the binary code for the address type of the Operand(miun).
 * This function converts the miun type to the corresponding binary representation.
 *
 * @param miun The address type of the Operand (miun) of the operand.
 *
 * @return the miun type number with the right bit (1) shifted to the left.
 */
int calc_miun_binary(int miun);

/**
 * @brief Function adds a linked list of the current binary lines (the binary words of instruction or directive) to the binary table.
 * This function appends a list of binary lines to the binary table, ensuring the table is correctly linked.
 *
 * @param line A pointer to the first binary line in the list (the first word).
 * @param head A pointer to the head of the binary table.
 */
void add_binary_lines(struct BinaryLine *line, struct BinaryLine **head);

/**
 * @brief Allocates memory for a new BinaryLine structure.
 * This function allocates memory for a new BinaryLine structure and initializes its fields.
 *
 * @param line_number The original line number from the assembly code.
 * @param decimal_memory_address The memory address in decimal form.
 *
 * @return A pointer to the newly allocated BinaryLine structure.
 */
struct BinaryLine *malloc_BinaryLine(int line_number, int decimal_memory_address);

/**
 * @brief Converts a label's memory address to binary code and updates the binary line.
 * This function updates a binary line with the address of a label, converting the label's memory address
 * to binary and setting the appropriate type (R, E).
 *
 * @param binary_line A pointer to the BinaryLine structure to update.
 * @param symbol A pointer to the symbol node representing the label after update.
 */
void convert_label_adress_to_binary_line(BinaryLine *binary_line, SymbolNode *symbol);


/**
 * @brief Function Frees a BinaryLine structures.
 * This function frees all memory allocated for a linked list of BinaryLine structures, including the labels.
 *
 * @param line A pointer to the first BinaryLine structure in the list.
 */

void free_BinaryLine(struct BinaryLine *line);


/**
 * @brief Converts a decimal number to binary and stores it in a string.
 * This function converts a decimal number to its binary representation and modifies the str array with the binary number.
 *
 * @param num The decimal number to convert.
 * @param str A string to store the binary representation.
 * @param size The size of the string (including null terminator).
 */
void decimal_to_binary(unsigned int num, char str[], int size);

/**
 * @brief Converts a decimal number to octal and stores it in a string.
 * This function converts a decimal number to its octal representation and modifies the str array with the octal number.
 * The octal representation is the final one.
 *
 * @param num The decimal number to convert.
 * @param str A string to store the octal representation.
 * @param size The size of the string (including null terminator).
 */
void decimal_to_octal(unsigned int num, char str[], int size);

#endif /* __BINARY_CONVERSION_H__ */