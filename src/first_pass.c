#include <stdio.h>
#include <string.h>

#include "first_pass.h"
#include "line_parser.h"
#include "lines.h"
#include "symbol_table.h"
#include "binary_table.h"
#include "util.h"

const Opcode OPCODES[] = {
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

int firstPass(char *file_name, SymbolTable *symbolTable,BinaryTable *binaryTable, int *IC, int *DC)
{
    char line[MAX_LINE_LENGTH];
    FILE *amfile = fopen(file_name, "r");
    AssemblyLine parsedLine;
    SymbolNode *current = NULL;
    int lineNumber = 0;
    int is_symbol = 0;
    while (fgets(line, MAX_LINE_LENGTH + 1, amfile) != NULL){
        lineNumber++;
        if (line[strlen(line) - 1] != '\n')
        {
            /* IMPROVE ERRORS*/
        }
        if (is_empty_line(line) || is_commented_line(line)) {
            continue;
        }
        parsedLine = parseAssemblyLine(line); /* now has label{optional}, value of command and operands.*/
        printf("Parsed Line:\n");
        printAssemblyLine(&parsedLine);
        if(parsedLine.label!=NULL){
            if(is_symbol_in_table(symbolTable, parsedLine.label)){
                /* IMRPOVE ERRORS*/
                /* label id defined already*/
            }
            is_symbol = 1;
        }
        if(parsedLine.instruction[0] == '.'){ /* checks if there is '.'*/
            if(strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0){
                if(is_symbol){
                    add_symbol_to_table(symbolTable, parsedLine.label, 1, *DC); /*add to symbol with 1 as data*/
                }
                error = handleDataDirective(&parsedLine, symbolTable, binaryTable, DC);
            }
            if (strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0){
                if(is_symbol){
                    add_symbol_to_table(symbolTable, parsedLine.label, 2, *DC); /*add to symbol with 2 as string*/
                }
                /*instruction is .data or .string*/
                error = handleStringDirective(&parsedLine, symbolTable, binaryTable, DC);
                }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0){  /* if extern*/
                handleExternDirective(&parsedLine, symbolTable, binaryTable, *DC);
                externCount += 1;
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {/* if entry*/
                handleEntryDirective(&parsedLine, symbolTable, binaryTable, *DC);
                entryCount += 1;
            }
            else{
                continue;
                /* IMPROVE ERRORS*/
                /* not defined directive*/
            }
            if (error != 1) {
                continue; /*something failed*/
                /* IMRPOVE ERRORS*/
                /* -> by lineNumber, error*/
            }
        }
        else{ /* line is command - maybe also label is defined*/
            handle_instruction(parsedLine,symbolTable,binaryTable, IC); /*label is defined inside, also makes binary*/
        }
        is_symbol = 0;
        /* check if memory overflow*/
        if ((*IC + *DC) > MAX_MEMORY_WORDS) {
            /* IMPROVE ERRORS*/
        }
    }
    current = symbolTable->head;
    while (current != NULL)
    {
        if (strcmp(current->symbolType, 1) == 0) /* data type is {data}*/
        {
            current->symbolValue = current->symbolValue + *IC;
        }
        current = current->next;
    }
    freeAssemblyLine(&parsedLine);
    return 1; /*SUCCESS*/
}

int check_type(Operand *operand){
    if (operand->value[0] == '\0' or operand->value[0] == NULL) {
        operand->type = -1;
        return 1;
    }
    switch (operand->value[0]) 
    {
        case '#':
            /*Remove '#' and check if the remaining part is a valid integer*/
            memmove(operand->value, operand->value + 1, strlen(operand->value)-1);
            if (is_valid_integer(operand->value)) {
                operand->type = 0;
            } 
            else {
                return 0;
                /* IMPROVE ERRORS*/
            }
            operand->value[strlen(operand->value) - 1] = '\0';
            break;
        case '*':
            memmove(operand->value, operand->value + 1, strlen(operand->value)-1);
            if (valid_reg_name(operand->value)) {
                operand->value[strlen(operand->value) - 1] = '\0';
                operand->type = 0;
                break;
            }
            elsel{
                return 0;
                /* IMPROVE ERRORS*/
            }
        case 'r':
            if(valid_reg_name(operand->value)){
                operand->type = 3;
                break;
            }
            else {
                /* IMPROVE ERRORS*/
                /*supposed to be register but not */
                return 0;
            }
        default:
            operand->type = 1; /* Default is it's label, will be checked in 2 pass */
            break;
    }
    return 1;
}

int get_opcode_operands(AssemblyLine* parsedLine){
    /* gets allowed operands of insturction*/
    int i =0;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(parsedLine->instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].numOfOperands;
            /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int get_opcode_code(AssemblyLine* parsedLine){
    /* gets opcode number of insturction*/
    int i =0;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(parsedLine->instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].code;
            /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int operand_parser(AssemblyLine* parsedLine){
    char *ptr_in_line = NULL;
    char *start = NULL;
    ptr_in_line = parsedLine->operands;
    Operand *temp_srcOperand = NULL, *temp_destOperand = NULL;
    int operandCount = 0;
    int num_operands = 0;
    temp_srcOperand  = (Operand *)malloc(sizeof(Operand));
    temp_destOperand  = (Operand *)malloc(sizeof(Operand));
    char *operandValue;
    int operandLen = 0;
    num_operands = get_opcode_operands(parsedLine); /*got how many allowed*/
    if(num_operands == -1){/*insturction not found*/
        /* IMPROVE ERRORS*/
        free(temp_srcOperand);
        free(temp_destOperand );
        return 0;
    }
    while (*ptr_in_line != '\0' && operandCount < num_operands) {
        while (*ptr_in_line != '\0' && isspace(*ptr_in_line)) ptr_in_line++; /* Skip whitespace */
        start = ptr_in_line;
        operandLen = 0;
        while (*ptr_in_line != '\0' && *ptr_in_line != ',') { /*after whitespaces*/
            /*go until first ',' the syntax of the value will be checked later*/
            ptr_in_line++;
            operandLen++;
        }
        *operandValue = (char *)malloc(operandLen + 1);
        strncpy(operandValue, start, operandLen);
        operandValue[operandLen] = '\0';
        if (num_operands == 2 && operandCount == 0) {
            temp_srcOperand->value = operandValue;
            if (!check_type(temp_srcOperand)) { /*type of miun will be inserted*/
                /* IMPROVE ERRORS */
                free(temp_srcOperand->value);
                free(temp_destOperand->value);
                free(temp_destOperand);
                free(temp_srcOperand);
                return 0;
            }
        }
        else if (num_operands == 1 || (num_operands == 2 && operandCount == 1)){
            temp_destOperand->value = operandValue; /*if there is only one it is assigned to dest, if teo and this is seconf opperand also dest.*/
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
    if(num_operands!=operandCount){
        /* IMPROVE ERRORS*/
        /*too many operands*/
        if (temp_srcOperand->value) free(temp_srcOperand->value);
        if (temp_destOperand->value) free(temp_destOperand->value);
        free(temp_srcOperand);
        free(temp_destOperand);
        return 0;
    }
    /* Assign operands to parsedLine */
    if (num_operands == 1) {
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
    else if (num_operands == 0 && parsedLine->operands == NULL) {
        srcOperand->type  = -1;
        srcOperand->value = '\0';
        temp_destOperand->type  = -1;
        temp_destOperand->value = '\0';
    }
    else{
        parsedLine->srcOperand = temp_srcOperand;
        parsedLine->destOperand = temp_destOperand;
    }
    int opcode_code = -1;
    int type_miun = -1;
    int type_miun_src = -1;
    int type_miun_dest = -1;
    opcode_code = get_opcode_address(parsedLine);
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
            if (!(parsedLine->srcOperand->type >= 0 && parsedLine->srcOperand->type <=3) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 1:
            /*cmp(1)*/
            if (!(parsedLine->srcOperand->type >= 0 && parsedLine->srcOperand->type <=3) && (parsedLine->destOperand->type >= 0 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 4:
            /*lea(4)*/
            if (!(parsedLine->srcOperand->type == 1) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=3 ))
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
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 9:
        case 10:
        case 13:
            /*jmp (9), bne(10), jsr(1)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=2 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 12:
            /*prn(12)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type >= 0 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
        case 14:
        case 15:
            /*rts(14), stop(15)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type == -1))
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

int handle_instruction(AssemblyLine* parsedLine,SymbolTable *symbol_table,BinaryTable *binary_table, *IC){
    int did_parse = 0;
    did_parse = operand_parser(parsedLine); /*checks if OPCODE is legal and Operands are as should be, by types of miun*/
    if(did_parse){
        /*IMPROVE ERRORS*/
        /*couldnt parse operands + OPCODE not legal*/
    }
    /*make_binary(parsedLine,IC); TO DO*/
    /*the line is in correct syntax*/
    if(parsedLine->label!=NULL){ /* already checked that wan't defined yet*/
        add_symbol_to_table(symbol_table, parsedLine->label, 0,IC); /*also checks if name is legal, symbol gets IC place*/
    }
    return 1;
}

int valid_reg_name(char *value){
    if (strlen(value) == 2 && value[0] == 'r' && value[1] >= '0' && value[1] <= '7'){
        return 1;
    }
    return 0;

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
    token = strtok(parsedLine->operands, ",");
    int value = 0, error = 0;
    while (token != NULL)
    {
        /*Trim leading whitespace - TO DO: ADD whitespaces if's before and after ','
        while (isspace((unsigned char)*token)) token++; - move until not ' '
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        end[1] = '\0';*/
        if (!is_valid_integer(token)) {
            /* IMRPOVE ERRORS*/
            return 0;
            /*operans is not valid number */
        }
        else { /*symbol syntax is checked in add symbol*/
            value = atoi(token); /*numeral value of the operand*/
        }
        /* Insert the binary */
        /*make binary - 
        error = insertToBinaryCodesTable(BinaryTable, *DC, parsedLine, convertIntToBinary(value, BINARY_CODE_LEN), parsedLine->operands);
        */
        if (error != SUCCESS){
            return 0;
            /* IMRPOVE ERRORS*/
            /* some error in binary conversion*/
        }
        *DC = *DC + 1;
        token = strtok(NULL, ",");
    }
    return 1;
}

int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryTable *BinaryTable, int *DC)
{
    int stringLen = 0, i = 0;
    char binaryCode[BINARY_CODE_LEN];
    char *int_to_Binary_Res = NULL;

    stringLen = strlen(parsedLine->operands);
    memset(binaryCode, '\0', sizeof(binaryCode));

    if (is_valid_string(parsedLine->operands) == 0) {
        /* IMPROVE ERRORS*/
        /*doesnt start with ""*/
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
    return 1;
}

int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryTable *BinaryTable){
    int err = 0;
    char *token = NULL;
    if (parsedLine->label != NULL) {
        /* IMPROVE ERRORS*/
        /*extern cant have label defenition*/
    }
    token = strtok(parsedLine->operands, ","); /*operands should be the name of the labels from extern*/
    int err = 1; /* Assume success */
    while (token != NULL) {
        if (!add_symbol_to_table(symbolTable, token, 4, 0)) {/*extern is type 4*/
            /*cheks inside if the symbol is in valid name*/
            /* IMPROVE ERRORS */
            err = 0; /* Failed to add symbol */
            break;
        }
        token = strtok(NULL, ",");
    }
    return err;
}

int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryTable *BinaryTable){
    int err = 0;
    char *token = NULL;
    if (parsedLine->label != NULL) {
        /* IMPROVE ERRORS*/
        /*entry shouldnt have label defenition*/
    }
    token = strtok(parsedLine->operands, ","); /*operands should be the name of the labels from extern*/
    int err = 1; /* Assume success */
    while (token != NULL) {
        if (!add_symbol_to_table(symbolTable, token, 4, 0)) {
            add_symbol_to_table(symbolTable,parsedLine->operands,3,0); /*entry is type 3*/
            /*cheks inside if the symbol is in valid name*/
            /* IMPROVE ERRORS */
            err = 0; /* Failed to add symbol */
            break;
        }
        token = strtok(NULL, ",");
    }
    return err;
}