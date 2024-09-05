#ifndef __UTIL_H__
#define __UTIL_H__
#define MAX_FILE_NAME 100 /* Assuming file name no bigger than 100 chars */

#include "symbol_table.h"
#include "macros.h"
#include "binary_conversion.h"


/**
 * @brief Adds a file extension to the given filename.
 * This function appends the specified extension to the given filename and returns
 * a newly allocated string containing the filename with the extension.
 *
 * @param filename The original filename without an extension.
 * @param extension The file extension to append.
 *
 * @return A pointer to the new string with the filename and extension, or NULL if memory allocation fails.
 */
char *add_file_extension(char *filename, char *extension);


/**
 * @brief Checks if a line is empty (only contains whitespace).
 * This function checks if the provided line is empty, containing only whitespace characters.
 *
 * @param line The line to check.
 * @return 0 if the line is empty, 1 if it contains non-whitespace characters.
 */
int is_empty_line(char *line);

/**
 * @brief Checks if a line is a commented line (starts with ';').
 * This function checks if the provided line is a commented line, indicated by a semicolon (';') at the start.
 *
 * @param line The line to check.
 *
 * @return 0 if the line is a comment, 1 otherwise.
 */
int is_commented_line(char *line);

/**
 * @brief Validates if the operand is a valid integer.
 * This function checks if the provided operand is a valid integer, including optional signs ('+' or '-') at the start.
 *
 * @param operand The operand to validate.
 *
 * @return 0 if the operand is valid, or an error code if the operand is not a valid integer.
 */
int is_valid_integer(char *operand);

/**
 * @brief Validates if the value is a valid register name (r0 to r7).
 * This function checks if the provided value is a valid register name in the format `r0` to `r7`.
 *
 * @param value The value to validate.
 *
 * @return 0 if the value is a valid register name, or an error code if invalid.
 */
int valid_reg_name(char *value);

/**
 * @brief Checks if a string is an opcode.
 * This function checks if the provided string is a valid opcode.
 *
 * @param name The string to check.
 *
 * @return 1 if the string is an opcode, 0 otherwise.
 */
int check_if_opcode(char *name);

/**
 * @brief Counts the occurrences of a character in a string.
 * This function counts how many times the specified character appears in the provided string.
 *
 * @param str The string to search.
 * @param ch The character to count.
 *
 * @return The number of occurrences of the character in the string.
 */
int count_occurrences(const char *str, char ch);

/**
 * @brief Duplicates a string to a new allocated memory location.
 * This function allocates memory and duplicates the provided string into the newly allocated memory.
 *
 * @param src_string The source string to duplicate.
 *
 * @return A pointer to the duplicated string, or NULL if memory allocation fails.
 */
char *strdup1(char *src_string);

/**
 * @brief Checks if a string is valid (starts and ends with double quotes).
 *
 * This function checks if the provided string starts and ends with double quotes (").
 *
 * @param str The string to validate.
 *
 * @return 0 if the string is valid, or an error code if invalid.
 */
int is_valid_string(char *str);

/**
 * @brief Trims leading and trailing whitespace from a string.
 * This function removes any leading or trailing whitespace characters from the provided string.
 *
 * @param str The string to trim.
 *
 * @return A pointer to the trimmed string.
 */
char *trim_whitespace(char *str);

/**
 * @brief Calculates the number of words (L) required for an instruction based on operand types.
 * This function determines the number of words (L) needed to represent command, based on
 * the addressing type (miun types) of the source and destination operands.
 *
 * @param srcType The addressing mode of the source operand.
 * @param dstType The addressing mode of the destination operand.
 *
 * @return The number of words (L) required to represent the instruction.
 */
int calculate_L(int srcType, int dstType);

/**
 * @brief Frees all allocated memory for macros, symbol table, and binary table.
 * This function frees all allocated memory for the macro list, symbol table, and binary table, setting their pointers to NULL.
 *
 * @param macro_head A pointer to the head of the macro list.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table.
 */
void free_all(struct macros **macro_head, SymbolTable **symbolTable, BinaryLine **binary_table);

#endif