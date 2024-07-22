#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "first_pass.h"
#include "line_parser.h"
#include "lines.h"
#include "symbol_table.h"
#include "binary_table.h"
#include "util.h"
#include "constant.h"
#include "error_handler.h"
#include "macros.h"

Opcode OPCODES[] = {
    {0, "mov", 2},
    {1, "cmp", 2},
    {2, "add", 2},
    {3, "sub", 2},
    {4, "lea", 1},
    {5, "clr", 1},
    {6, "not", 2},
    {7, "inc", 1},
    {8, "dec", 1},
    {9, "jmp", 1},
    {10, "bne", 1},
    {11, "red", 1},
    {12, "prn", 1},
    {13, "jsr", 1},
    {14, "rts", 0},
    {15, "stop", 0}
};

int firstPass(char *file_name,struct macros *macro_head, SymbolTable *symbolTable,BinaryTable *binaryTable, int *IC, int *DC)
{
    char line[MAX_LINE_LENGTH];
    FILE *amfile = fopen(file_name, "r");
    AssemblyLine parsedLine;
    SymbolNode *current = NULL;
    int lineNumber = 0;
    int is_symbol = 0;
    ErrorCode error_flag = 0; /*assume success*/
    while (fgets(line, MAX_LINE_LENGTH + 1, amfile) != NULL){
        lineNumber++;
        if (line[strlen(line) - 1] != '\n')
        {
            /* IMPROVE ERRORS*/
        }
        if ((is_empty_line(line) == 0) || (is_commented_line(line) == 0)) { /*continue to next line if empty or commented.*/
            continue;
        }
        parsedLine = parseAssemblyLine(line); /* Parse line for this format: label{optional},instruction, operands.*/
        printf("Parsed Line:\n");
        printAssemblyLine(&parsedLine);
        if(parsedLine.label!=NULL){
            if(is_symbol_in_table(symbolTable, parsedLine.label) == 0){
                error_flag = ERROR__SYMBOL_DEFINED_TWICE;
                handle_error(error_flag, lineNumber);
            }
            is_symbol = 1;
        }
        if(parsedLine.instruction[0] == '.'){ /* if '.' is found it is directive line.*/
            if(strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0){ /* if data*/
                if(is_symbol){
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, 1, *DC, macro_head); /*Checks correct syntax in function. symbol type is 1: data*/
                    handle_error(error_flag, lineNumber);
                }
                error_flag = handleDataDirective(&parsedLine, symbolTable, binaryTable, DC);
            }
            else if (strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0){ /* if string*/
                if(is_symbol){
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, 2, *DC, macro_head); /*Checks correct syntax in function. symbol type is 2: string*/
                    handle_error(error_flag, lineNumber);
                }
                error_flag = handleStringDirective(&parsedLine, symbolTable, binaryTable, DC);
                }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0){  /* if extern*/
                error_flag = handleExternDirective(&parsedLine, symbolTable, binaryTable, macro_head);
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {/* if entry*/
                error_flag = handleEntryDirective(&parsedLine, symbolTable, binaryTable, macro_head);
            }
            else{ /*if not one of defined directives.*/
                handle_error(ERROR_NOT_DEFINED_DIRECTIVE, lineNumber);
            }
        }
        else{ /* assume line is command - maybe also label is defined*/
            error_flag = handle_instruction(&parsedLine,symbolTable,binaryTable, IC);  /*label is defined inside, also makes binary*/
        }
        if (error_flag != 0) { /*if something failed*/
            handle_error(error_flag, lineNumber);
        }
        is_symbol = 0; /*return symbol flag to 0*/
        /* check if memory overflow*/
        if ((*IC + *DC) > MAX_MEMORY_WORDS) {
            error_flag = ERROR_MEMORY_OVERFLOW;
            handle_error(error_flag, lineNumber);
            return error_flag;
        }
    }
    current = symbolTable->head;
    while (current != NULL)
    {
        if (current->type == 1) /* data type is {data}*/
        {
            current->memory_place += *IC;
        }
        current = current->next;
    }
    freeAssemblyLine(&parsedLine);
    return error_flag; /* 0 -> SUCCESS*/
}

int check_type(Operand *operand){
    ErrorCode error_flag = 0; /*assume success*/
    if ((operand->value[0] == '\0')) {
        error_flag = ERROR_NOT_ENOUGH_OPERANDS;
        return error_flag;
    }
    switch (operand->value[0]) 
    {
        case '#':
            /*Remove '#' and check if the remaining part is a valid integer*/
            memmove(operand->value, operand->value + 1, strlen(operand->value)-1);
            error_flag = is_valid_integer(operand->value);
            if (error_flag==0) {
                operand->type = 0;
                operand->value[strlen(operand->value) - 1] = '\0';
            }
            else {
                return error_flag;
            }
            break;
        case '*':
            memmove(operand->value, operand->value + 1, strlen(operand->value)-1);
            error_flag = valid_reg_name(operand->value);
            if (error_flag==0) {
                operand->value[strlen(operand->value) - 1] = '\0';
                operand->type = 0;
                break;
            }
            else{
                return error_flag;
            }
        case 'r':
            error_flag = valid_reg_name(operand->value);
            if(error_flag==0){
                operand->type = 3;
                break;
            }
            else {
                return error_flag;
            }
        default:
            operand->type = 1; /*to do:  Default is it's label, will be checked in 2 pass */
            break;
    }
    return error_flag; /*0 ->success*/
}

int get_opcode_operands(char* instruction){
    /* gets allowed operands of insturction*/
    int i = 0;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].numOfOperands; /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int get_opcode_code(char* instruction){
    /* gets opcode number of insturction*/
    int i =0;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].code; /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int operand_parser(AssemblyLine* parsedLine){
    char *ptr_in_line = NULL;
    char *start = NULL;
    ptr_in_line = parsedLine->operands;
    ErrorCode error_flag = 0; /*assume success*/
    /*helper variables:*/
    int opcode_code = -1, type_miun_src = -1, type_miun_dest = -1, operandCount = 0, num_operands_allowed = 0, operandLen = 0;
    char *operandValue = NULL;
    Operand *temp_srcOperand = NULL;
    Operand *temp_destOperand = NULL;

    temp_srcOperand  = (Operand *)malloc(sizeof(Operand));
    temp_destOperand  = (Operand *)malloc(sizeof(Operand));

    if(temp_srcOperand == NULL || temp_destOperand == NULL){
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        return error_flag;
    }

    num_operands_allowed = get_opcode_operands(parsedLine->instruction); /*got how many operands allowed for Opcode*/
    if(num_operands_allowed == -1){ /*insturction not found*/
        error_flag = ERROR_INSTRUCTION_NOT_VALID;
        free(temp_srcOperand);
        free(temp_destOperand );
        return error_flag;
    }
    while (*ptr_in_line != '\0' && operandCount < num_operands_allowed) {
        while (*ptr_in_line != '\0' && isspace(*ptr_in_line)) ptr_in_line++; /* Skip whitespace */
        start = ptr_in_line;
        operandLen = 0;
        while (*ptr_in_line != '\0' && *ptr_in_line != ',') { /*after whitespaces*/
            /*go until first ',' the syntax of the value will be checked later*/
            ptr_in_line++;
            operandLen++;
        }
        operandValue = (char *)malloc(operandLen + 1);
        if(operandValue == NULL){
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        return error_flag;
        }
        strncpy(operandValue, start, operandLen);
        operandValue[operandLen] = '\0';
        if (num_operands_allowed == 2 && operandCount == 0) {/*while reading first operand where should be two -> assign to srcOperand*/
            temp_srcOperand->value = operandValue;
            if (!check_type(temp_srcOperand)) { /*inserts type of miun to parsedLine and checks errors*/
                /* IMPROVE ERRORS */
                freeOperand(temp_srcOperand);
                freeOperand(temp_destOperand);
                return 0;
            }
        }
        else if (num_operands_allowed == 1 || (num_operands_allowed == 2 && operandCount == 1)){
            temp_destOperand->value = operandValue; /*if only one operand allowed or if two and this is second opperand -> assign to destOperand .*/
            if (!check_type(temp_destOperand)) {/*type of miun will be inserted*/
                /* IMPROVE ERRORS */
                free(temp_srcOperand->value);
                free(temp_destOperand->value);
                free(temp_destOperand);
                free(temp_srcOperand);
                return 0;
            }
        }
        else continue;
        operandCount++;
        if (*ptr_in_line == ',') ptr_in_line++; /* Skip comma */
    }
    /* Check for extra characters after the last operand (up to 2), if num_operands == 0 will also arrive here and check*/
    while (*ptr_in_line != '\0') {
        if (!isspace(*ptr_in_line)) {
            /*there are chars after last operand or operands when needed to be 0*/
            /* IMPROVE ERRORS */
            if (temp_srcOperand->value) free(temp_srcOperand->value);
            if (temp_destOperand->value) free(temp_destOperand->value);
            free(temp_srcOperand);
            free(temp_destOperand);
            return 0;
        }
        ptr_in_line++;
    }
    if(num_operands_allowed!=operandCount){
        /* IMPROVE ERRORS*/
        /*too many operands*/
        if (temp_srcOperand->value) free(temp_srcOperand->value);
        if (temp_destOperand->value) free(temp_destOperand->value);
        free(temp_srcOperand);
        free(temp_destOperand);
        return 0;
    }
    /* Assign operands to parsedLine */
    if (num_operands_allowed == 1) {
        temp_srcOperand->type  = -1;
        temp_srcOperand->value = '\0';
        parsedLine->destOperand = temp_destOperand;
    }
    else if (operandCount == 0 && parsedLine->operands != NULL){
            /* IMROVE ERRORS*/
            /*chars but shouldnt be */
            free(temp_srcOperand);
            free(temp_destOperand);
            return 0;
    } 
    else if (num_operands_allowed == 0 && parsedLine->operands == NULL) {
        temp_srcOperand->type  = -1;
        temp_srcOperand->value = '\0';
        temp_destOperand->type  = -1;
        temp_destOperand->value = '\0';
    }
    else{
        parsedLine->srcOperand = temp_srcOperand;
        parsedLine->destOperand = temp_destOperand;
    }
    opcode_code = get_opcode_code(parsedLine);
    type_miun_src = parsedLine->srcOperand->type;
    type_miun_dest = parsedLine->destOperand->type;
    switch (opcode_code){
        /*mov (0), add(2), sub(3), - have {0,1,2,3) types allowed for src*/
        case -1:
            /*cannot find insturction*/
            /*IMPROVE ERRORS*/
            break;
        case 0:
        case 2:
        case 3:
            if (!(type_miun_src >= 0 && type_miun_src <=3) && (type_miun_dest >= 1 && type_miun_dest <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 1:
            /*cmp(1)*/
            if (!(type_miun_src >= 0 && type_miun_src <=3) && (type_miun_dest >= 0 && type_miun_dest <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 4:
            /*lea(4)*/
            if (!(type_miun_src == 1) && (type_miun_dest >= 1 && type_miun_dest <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 11:
            /*clr(5), not(6), inc(7), dec(8), red(11)*/
            if (!(type_miun_src == -1) && (type_miun_dest >= 1 && type_miun_dest <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 9:
        case 10:
        case 13:
            /*jmp (9), bne(10), jsr(1)*/
            if (!(type_miun_src == -1) && (type_miun_dest >= 1 && type_miun_dest <=2 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 12:
            /*prn(12)*/
            if (!(type_miun_src == -1) && (type_miun_dest >= 0 && type_miun_dest <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
        case 14:
        case 15:
            /*rts(14), stop(15)*/
            if (!(type_miun_src == -1) && (type_miun_dest == -1))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
        default:
            break;
    }
    /*the instructions mathed the operands succefully and all was allocated in parsedLine*/
    return 1;
}

int handle_instruction(AssemblyLine *parsedLine,SymbolTable *symbol_table,BinaryTable *binary_table, int *IC, struct macros *macro_head){
    ErrorCode error_flag = 0; /*assume success*/
    error_flag = operand_parser(parsedLine); /*checks if OPCODE is legal and Operands are as should be, by types of miun*/
    if(error_flag != 0){
        return error_flag;
    }
    /*make_binary(parsedLine,IC); TO DO*/
    /*the line is in correct syntax*/
    if(parsedLine->label!=NULL){ /* already checked that wan't defined yet*/
        error_flag = add_symbol_to_table(symbol_table, parsedLine->label, 0, *IC,macro_head); /*also checks if name is legal, symbol gets IC place*/
    }
    return error_flag; /* 0 -> SUCCESS*/
}

int calculate_L(int srcType, int dstType)
{
    int L = 1;/* at least 1, the base word */
    if (srcType == -1 && dstType == -1)
    {
        return L;
    }
    if (srcType == 3 && dstType == 3){
        return L + 1;
    }
    if (srcType == 0 || srcType == 3 || srcType == 1){
        L = L + 1;
    }
    if (dstType == 0 || dstType == 3 || dstType == 1){
        L = L + 1;
    }
    if (dstType == 2){
        L = L + 2;
    }
    if (srcType == 2){
        L = L + 2;
    }
    return L;
}

int handleDataDirective(AssemblyLine* parsedLine,SymbolTable *symbolTable,BinaryTable *BinaryTable, int *DC){
    char *token = NULL;
    token = strtok(parsedLine->operands, ", ");
    int value = 0, error = 0;
    ErrorCode error_flag = 0; /*assume success*/
    while (token != NULL)
    {
        /*Trim leading whitespace - TO DO: ADD whitespaces if's before and after ','
        while (isspace((unsigned char)*token)) token++; - move until not ' '
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        end[1] = '\0';*/
        if (is_valid_integer(token)!=0) {
            error_flag = ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
            return error_flag;
        }
        else { /*symbol syntax is checked in add symbol*/
            value = atoi(token); /*numeral value of the operand*/
        }
        /* Insert the binary */
        /*make binary - 
        error = insertToBinaryCodesTable(BinaryTable, *DC, parsedLine, convertIntToBinary(value, BINARY_CODE_LEN), parsedLine->operands);
        */
        if (error != 0){
            return 0;
            /* IMRPOVE ERRORS*/
            /* some error in binary conversion*/
        }
        *DC = *DC + 1;
        token = strtok(NULL, ",");
    }
    return error_flag; /* 0 -> SUCCESS*/
}

int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryTable *BinaryTable, int *DC)
{
    int stringLen = 0, i = 0;
    char binaryCode[BINARY_CODE_LEN];

    stringLen = strlen(parsedLine->operands);
    memset(binaryCode, '\0', sizeof(binaryCode));
    ErrorCode error_flag = 0; /*assume success*/
    if (is_valid_string(parsedLine->operands) != 0) {
        error_flag = ERROR_STRING_SYNTAX_NOT_VALID;
        return error_flag;
    }
    /*go through string without ""*/
    for (i = 1; i < stringLen -1; i++) {
        /* converts the ASCII value of the character to a binary string */
        /*make binary:
        intToBinaryRes = convertIntToBinary((int)parsedLine->operands[i], BINARY_CODE_LEN);
        strcpy(binaryCode, intToBinaryRes);
        insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, binaryCode, &parsedLine->operands[i]);
        */
        *DC = *DC + 1; 
    }

    /* Null byte at the end of the string */
    /* Check if BINARY_CODE_LEN or BINARY_CODE_LEN -1 */
    /*insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, convertIntToBinary(0, BINARY_CODE_LEN), &parsedLine->operands[i]);*/
    *DC = *DC + 1; /*for '/0'*/
    return error_flag; /* 0 -> SUCCESS*/
}

int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryTable *BinaryTable, struct macros *macro_head){
    /* TO DO - add if there is space between operands*/
    char *token = NULL;
    ErrorCode error_flag = 0; /* Assume success */
    if (parsedLine->label != NULL) {
        error_flag = ERROR_EXTERN_NOT_VALID;
        return error_flag;
    }
    token = strtok(parsedLine->operands, ","); /*in .extern the labels will be the operands.*/
    while (token != NULL) {
        error_flag = add_symbol_to_table(symbolTable, token, 4, 0, macro_head); /*extern is type 4. checks inside if the symbol is in valid name*/
        if(error_flag!=0){ /*if some label not valid.*/
            return error_flag;
        }
        token = strtok(NULL, ",");
    }
    return error_flag; /*0 -> SUCCESS*/
}

int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryTable *BinaryTable, struct macros *macro_head){
    /* TO DO - add if there is space between operands*/
    char *token = NULL;
    ErrorCode error_flag = 0; /* Assume success */
    if (parsedLine->label != NULL) {
        error_flag = ERROR_ENTRY_NOT_VALID;
        return error_flag;
    }
    token = strtok(parsedLine->operands, ","); /*in .entry the labels will be the operands.*/
    while (token != NULL) {
        error_flag = add_symbol_to_table(symbolTable, token, 3, 0, macro_head); /*extern is type 3. checks inside if the symbol is in valid name*/
        if(error_flag!=0){ /*if some label not valid.*/
            return error_flag;
        }
        token = strtok(NULL, ",");
    }
    return error_flag; /*0 -> SUCCESS*/
}