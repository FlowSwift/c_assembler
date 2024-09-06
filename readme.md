
# 🎯 C Assembler

Welcome to the assembler program of Gal Barda and Daniel Sudakov!

This assembler translates assembly code into machine like code. It processes the code in three stages: Preprocessing, First Pass, and Second Pass, generating an object file (.ob), along with optional entry (.ent) and extern (.ext) files.

  

# 🚀 Usage

## To build the assembler:
```
make assembler
```

Executable file will be located inside `./bin/`.
## To run the assembler:

```bash
./bin/assembler <path_to_assembly_file>
``` 
(can do multiple files at a time)

** Note: Input files must have a **.as** file extension.

### Example usage:
```
make
./bin/assembler file_1 file_2 file_3
```

  
  

# 🛠️ Assembly Process Flow

## 1️⃣ Preprocessing

The pre_process function cleans the assembly file by removing unnecessary whitespace and expanding macros using strip_file and process_macros. The output is a cleaned, macro-expanded assembly file ready for further processing.

  

## 2️⃣ First Pass

During the first pass, the assembler:

  

Parses each line into an AssemblyLine structure with three components: {label (optional)} {instruction (directive or command)} {operands (optional)}.

Collects and validates labels, storing them in the symbol table, ensuring they aren't defined twice.

Validates instruction syntax, especially for .data and .string directives.

Generates partial binary code with placeholders for unresolved addresses (labels), and inserts it into a BinaryLine linked list.

  

## 3️⃣ Second Pass

In the second pass, the assembler:

  

Resolves all labels to their actual memory addresses, inserting them into the BinaryLine.

Generates the final binary code and writes it to the .ob file.

Produces .ent and .ext files if entry or extern symbols are present, ensuring valid usage.

  

# 📂 Main Program

assembler.c

The main file that coordinates the entire assembler process, invoking preprocessing, the first pass, and the second pass.

  
# 📄 Output Files

## .am: Assembly code after the pre-process stage.

## .ob (Object File): Contains the final machine code, starting with the binary instructions followed by directives (.data and .string). The numbers in the first row are the amount of instructions and amount of directives.

## .ent (Entry File): Lists symbols marked with .entry, along with their memory addresses (where they were defined).

## .ext (Extern File): Lists external symbols defined with .extern, and their memory addresses (where they were used).

  

# 📊 Data Structures

- AssemblyLine: Represents a parsed assembly line containing a label - optional, instruction, and operands - optional.

- Operand: Represents an operand, including its value and addressing type (miun).

- Macros: A linked list representing macros in the file during the preprocessing stage.

- SymbolTable: A linked list of all symbols in the file, including their memory addresses and label types.

- BinaryLine: A linked list representing the binary codes relevent for the word generated from the source file. will be written into a file.

## 🔧 Helper Modules

- line_parser.c: Parses each line during the first pass into a structure for later use.

- symbol_table.c: Manages the symbol table by adding, validating, and searching for symbols while ensuring no double definitions.

- command_handler.c: Handles command lines by validating them, checking if operand addressing types are valid, and calling binary conversion after validation.

- directive_handler.c: Manages .entry, .extern, .data, and .string directives, validating and converting them to binary code.

- binary_conversion.c: Responsible for generating the binary machine code from the source file.

- lines.c: Manages a linked list of lines, with functions to allocate and free memory, particularly useful for macros.

- macros.c: Handles macro creation, lookup, and memory management, including adding macros, checking for existing ones, and freeing allocated memory.

- error_handler.c: Reports errors such as missing symbols, syntax issues, and memory allocation failures.

- util.c: Provides helper functions for string manipulation, file handling, and operand validation.