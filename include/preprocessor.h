#ifndef __PREPROCESSOR_H__
#define __PREPROCESSOR_H__
#include <stdio.h>
#include "macros.h"

/**
 * @brief Pre-process function that initializes and handles the file for next functions.
 * This function performs the initial processing of the assembly file, including stripping whitespace,
 * processing macros, and generating a temporary processed file.
 *
 * @param file_name The name of the file to process.
 * @param macro_head A pointer to the head of the macro list.
 *
 * @return 0 on success, or an error code on failure.
 */
int pre_process(char *file_name, struct macros **macro_head);


/**
 * @brief Strips the file of extra whitespace.
 * This function removes extra whitespace from the file, such as leading, trailing,
 * or consecutive spaces, and writes the stripped content to a temporary file.
 *
 * @param filename The name of the file to strip.
 * @param temp_file_name The name of the temporary file to store the stripped content.
 *
 * @return 0 on success, or an error code on failure.
 */
int strip_file(char *filename, char *temp_file_name);

/**
 * @brief Strips a line of extra whitespace.
 * This function removes any leading, trailing, or consecutive whitespace characters
 * from the source string and writes the stripped result to the destination string.
 *
 * @param dest The destination string where the stripped line will be stored.
 * @param source The source string to strip whitespace from.
 */
void strip_line(char *dest, char *source);

/**
 * @brief Processes macros in the file, scans the file for macros, expands any existing macros, and writes the
 * processed content to a temporary file.
 *
 * @param filename The name of the file being processed.
 * @param temp_file_name The name of the temporary file to store the processed content.
 * @param macros_head A pointer to the head of the macro list.
 *
 * @return 0 on success, or an error code on failure.
 */
int process_macros(char *filename, char *temp_file_name, struct macros **macros_head);

/**
 * @brief Validates if the given macro name is valid.
 * This function checks if the macro name in the line is valid, ensuring it doesn't conflict with
 * existing macro names or reserved keywords (.data, .string, register name, macr, endmacr).
 *
 * @param macr_ptr A pointer to the macro name.
 * @param line The line where the macro is declared.
 * @param line_number The line number for error reporting.
 * @param head A pointer to the macro list for validation.
 *
 * @return 0 if the macro name is valid, or an error code if invalid.
 */
int validate_macro_name(char *macr_ptr, char *line, int line_number, struct macros *head);

/**
 * @brief Adds a new macro to the macro list.
 *
 * This function adds a macro definition by reading its lines from the file until the macro
 * end marker is encountered. The macro is added to the macro list.
 *
 * @param file A pointer to the file being processed.
 * @param processed_file A pointer to the temporary file to store the processed content.
 * @param ptr_to_head A pointer to the head of the macro list.
 * @param macro_ptr A pointer to the macro declaration line.
 * @param line_number A pointer to the current line number for error reporting.
 * 
 * @return 0 on success, or an error code on failure.
 */
int add_macro(FILE *file, FILE *processed_file, struct macros **ptr_to_head, char *macro_ptr, int *line_counter);

/**
 * @brief Writes the contents of a macro to the file.
 * This function writes the lines of a macro to the specified file.
 *
 * @param macro A pointer to the macro structure containing the lines to write.
 * @param file A pointer to the file where the macro content will be written.
 */
void write_macro(struct macros *macro, FILE *file);

#endif /* __PREPROCESSOR_H__ */