
# C_Assembler
This is the Assembler program of Gal Barda and Daniel Sudakov . Welcome!
This assembler translates assembly code into "machine" code. 
It processes the code in three stages: Preprocessing, First Pass, and Second Pass, generating an object file (.ob), along with optional entry (.ent) and extern (.ext) files.

## Usage
```bash
# To build the assembler:
make assembler

# To run the assembler:
assembler.exe <.as file path>

**make sure to use Unix format when using our assembler.**

## Assembly Process Flow
Preprocessing: The pre_process function cleans the assembly file by removing unnecessary whitespace and expanding macros using strip_file and process_macros. The output is a stripped, macro-expanded assembly file ready for further processing. 

First Pass: 
The first pass scans each line of the preprocessed assembly file. It performs the following tasks:

Parses each line into an AssemblyLine structure with three components: {label (optional)} {instruction (can be a directive or command)} {operands (optional)}.
Collects labels and stores them in the symbol table, checking if they are valid by format and ensuring they aren't defined twice.
Validates instruction syntax and checks the syntax for .data and .string directives.
Generates partial binary code, leaving placeholders for unresolved addresses (labels), and inserts it into a BinaryLine linked list.

Second Pass:
The second pass finalizes the assembly process. It has the following stages - 
Resolving all labels to their actual memory addresses and inserting them into the BinaryLine.
Generating the final binary code and writing it to the .ob file.
Producing .ent and .ext files if entry or extern symbols are present, while ensuring that their usage is valid.

assembler.c: The Msain file, coordinates the entire assembler process, calling each stage (preprocessing, first pass, and second pass).

## Helper programs:
line_parser.c: Parses a line recived in first pass and makes it into a structure used later. 
symbol_table: Manages the symbol table, providing functions to add, validates the correct usage for every label, no double definition and searchs symbols in Symbol table.
command_handler.c: Handles command lines by validating them, checking if address tyoes of operands are valid etc The binary conversion is callled from here after validation.
directive_handler.c: Contains 4 handle functions for .entry, .extern, .data and .string. The binary conversion is callled from here after validation. 
binary_conversion: Responsible for creating the binary code of the source code for every line.
lines.c: Manages a linked list of lines, providing functions to allocate memory for new lines and free the entire list. helpful for macros. 
macro.c: Handles the creation, lookup, and memory management of macros, including functions to add new macros, check for existing ones, and free allocated macro memory.
error_handler.c: Reports errors such as missing symbols, syntax issues, and memory allocation failures.
util.c: Contains helper functions for string manipulation, file handling, and operand validation.

## Data Structures
- **AssemblyLine**: Represents a parsed assembly line, with a label, instruction and operands.
- **Operand**: Represents an operand, with a value and address type.
- **Macros**: A linked list representing the list of macros in the file in the Preprocesser stage.
- **SymbolTable**: A linked list of all the symbols in the file, with memory address and types of labels.
- **Binary_table**: A linked list representing the binary codes of all of the source file code.

## Output Files
.ob (Object File): Contains the final machine code by order - first binary instructions and later directive(.data and .string).
.ent (Entry File): Lists symbols marked as .entry, along with their addresses.
.ext (Extern File): Lists external symbols defined with .extern and their memory addresses.


## testing
Use .test extension for output files to avoid commiting
```c
gcc -ansi -pedantic -Wall -g preprocessor.c -o r.test && ./r.test test_in1
```