#include "constants.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "binary_data_structure.h"
#include "utils.h"
#include "errors.h"

/* Initializing head struct */
void init_head(head *_linkedList)
{
    _linkedList->_headOfSymbolTable = NULL;
    _linkedList->_headOfImageCode = NULL;
    _linkedList->_headOfImageData = NULL;
    _linkedList->_counter = 0;
}

/* Initializing symbol struct */
void init_symbol(symbol *_linkedList)
{
    /* Clear all strings: */
    memset(_linkedList->_symbolName, 0, sizeof(_linkedList->_symbolName));
    memset(_linkedList->_attribute, 0, sizeof(_linkedList->_attribute));

    /* Set all boolean flags to FALSE: */
    _linkedList->_isEntry = FALSE;
    _linkedList->_isExtern = FALSE;
    _linkedList->_isData = FALSE;
    _linkedList->_value = 0;

    _linkedList->_next = NULL;
}

/* Initializing image struct */
void init_image(image *_linkedList)
{
    /* Clear all strings: */
    memset(_linkedList->_referencedSymbolName, 0, sizeof(_linkedList->_referencedSymbolName));
    memset(_linkedList->_errorDescription, 0, sizeof(_linkedList->_errorDescription));

    _linkedList->_attribute = '\0';
    _linkedList->_value = 0;
    _linkedList->_binary = 0;
    _linkedList->_lineNumber = 0;

    /* Set all boolean flags to FALSE: */
    _linkedList->_isFirstOperandRelativeAddress = FALSE;
    _linkedList->_isError = FALSE;
    _linkedList->_isSymbol = FALSE;
    _linkedList->_next = NULL;
}

/* checks if symbol linked list is empty */
bool is_empty_symbol_linked_list(head *_node)
{
    if (_node->_headOfSymbolTable == NULL)
        return TRUE;
    else
        return FALSE;
}

/* checks if image code linked list is empty */
bool is_empty_image_code(head *_node)
{
    if (_node->_headOfImageCode == NULL)
        return TRUE;
    else
        return FALSE;
}

/* checks if image data linked list is empty */
bool is_empty_image_data(head *_node)
{
    if (_node->_headOfImageData == NULL)
        return TRUE;
    else
        return FALSE;
}

/* Adding label to a label-table */
void append_to_symbol(head *_pointerToFirstNode, line *_line, int *_dataCounter, int *_instructionCounter, char *_attribute)
{
    size_t i; /* Counter to indicate Label's location */
    char *_symbolName;

    symbol *_newNode = (symbol *)malloc(sizeof(symbol));
    init_symbol(_newNode);

    if (!is_successfull_malloc(_newNode))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    i = 0;
    if (strcmp(_attribute, EXTERNAL_ATTRIBUTE) == 0) /* if label is external -label name will be next word after ".extern" */
        i++;

    _symbolName = _line->_lineWords[i];

    _pointerToFirstNode->_counter++; /* increase count in one and keeps how many symbols are in the table */

    if (is_empty_symbol_linked_list(_pointerToFirstNode)) /* checks if symbol is first in the table */
        append_first_symbol_node(_pointerToFirstNode, _symbolName, _dataCounter, _instructionCounter, _attribute, _newNode);

    else /* if exists at least 1 node */
    {
        report_existing_symbol_error(_line, _symbolName, _pointerToFirstNode->_headOfSymbolTable); /* Checks if symbol already exists */

        if (!_line->_isError) /* if symbol doesn't exist already in symbol table */
            append_symbol_node(_pointerToFirstNode, _symbolName, _dataCounter, _instructionCounter, _attribute, _newNode);
        else
            free(_newNode);
    }
}

/* Adding only first node to symbol table*/
void append_first_symbol_node(head *_pointerToFirstNode, char *_symbolName, int *_dataCounter, int *_instructionCounter, char *_attribute, symbol *_newNode)
{
    /*Inserts new node to empty list*/
    strcpy(_newNode->_symbolName, _symbolName);
    strcpy(_newNode->_attribute, _attribute);

    _newNode->_next = NULL;
    _pointerToFirstNode->_headOfSymbolTable = _newNode;
    _newNode->_value = 0;

    if (strcmp(_attribute, DATA_ATTRIBUTE) == 0)
    {
        _newNode->_value = *_dataCounter;
        _newNode->_isData = TRUE;
    }

    else if (strcmp(_attribute, EXTERNAL_ATTRIBUTE) == 0)
    {
        _newNode->_value = 0;
        _newNode->_isExtern = TRUE;
    }

    else if (strcmp(_attribute, CODE_ATTRIBUTE) == 0)
        _newNode->_value = *_instructionCounter;
}

/* Adding second and following nodes to the symbol table */
void append_symbol_node(head *_pointerToFirstNode, char *_symbolName, int *_dataCounter, int *_instructionCounter, char *_attribute, symbol *_newNode)
{
    /* Initialize pointer to iterate through list */
    symbol *_traverseNode = _pointerToFirstNode->_headOfSymbolTable;

    /* REACH END OF LIST: */
    while (_traverseNode != NULL && _traverseNode->_next != NULL)
        _traverseNode = _traverseNode->_next;

    strcpy(_newNode->_symbolName, _symbolName);
    strcpy(_newNode->_attribute, _attribute);

    _newNode->_next = NULL;
    _traverseNode->_next = _newNode; /*connects new node to list by inserting to end*/
    _newNode->_value = 0;

    if (strcmp(_attribute, DATA_ATTRIBUTE) == 0)
    {
        _newNode->_value = *_dataCounter;
        _newNode->_isData = TRUE;
    }

    else if (strcmp(_attribute, EXTERNAL_ATTRIBUTE) == 0)
    {
        _newNode->_value = 0;
        _newNode->_isExtern = TRUE;
    }

    else if (strcmp(_attribute, CODE_ATTRIBUTE) == 0)
        _newNode->_value = *_instructionCounter;
}

/* Adding code (instraction) to image table of codes only */
void append_to_image_code(head *pointerToFirstNode, int *_instructionCounter, char _attribute, unsigned int _bitsInDecimal, char *_symbolName, size_t _lineNumber)
{
    image *_newNode = (image *)malloc(sizeof(image));
    init_image(_newNode);

    if (!is_successfull_malloc(_newNode))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    pointerToFirstNode->_counter++;

    if (is_empty_image_code(pointerToFirstNode)) /* If no nodes in list exist -> creates first node */
        append_first_image_code_node(pointerToFirstNode, _instructionCounter, _attribute, _bitsInDecimal, _newNode, _symbolName, _lineNumber);

    else /* at least one node exist */
        append_image_code_node(pointerToFirstNode, _instructionCounter, _attribute, _bitsInDecimal, _newNode, _symbolName, _lineNumber);
}

/* Adding only first node to image table of codes (instructions)*/
void append_first_image_code_node(head *pointerToFirstNode, int *_instructionCounter, char _attribute, unsigned int _bitsInDecimal, image *_newNode, char *_symbolName, size_t _lineNumber)
{
    _newNode->_value = *_instructionCounter;
    _newNode->_attribute = _attribute;  /* setAttribute return char (A-R-E) */
    _newNode->_binary = _bitsInDecimal; /* command translated to 'word' - 12 bits converted to decimal number */

    /* updates new node's status: */
    strcpy(_newNode->_referencedSymbolName, _symbolName); /* store symbol name in _referencedSymbolName variable */

    if (_newNode->_referencedSymbolName[0] == '%') /* checks if command is jmp */
    {
        /* remove % sign from name of symbol*/
        memmove(_newNode->_referencedSymbolName, _newNode->_referencedSymbolName + 1, strlen(_newNode->_referencedSymbolName));
        _newNode->_isFirstOperandRelativeAddress = TRUE; /* operand is relative */
    }

    if (strcmp(_newNode->_referencedSymbolName, NO_SYMBOL) != 0) /* checks if symbol exist in the command */
    {
        _newNode->_isSymbol = TRUE;
        _newNode->_lineNumber = _lineNumber;
    }

    _newNode->_next = NULL;
    pointerToFirstNode->_headOfImageCode = _newNode;
}

/* Adding second and following nodes to the image table of codes (instructions) */
void append_image_code_node(head *pointerToFirstNode, int *_instructionCounter, char _attribute, unsigned int _bitsInDecimal, image *_newNode, char *_symbolName, size_t _lineNumber)
{
    /*Initialize pointer to iterate through list*/
    image *_traverseNode = pointerToFirstNode->_headOfImageCode;

    while (_traverseNode != NULL && _traverseNode->_next != NULL) /* reaching last node */
        _traverseNode = _traverseNode->_next;

    _newNode->_value = *_instructionCounter;
    _newNode->_attribute = _attribute;  /*setAttribute return char (A-R-E)*/
    _newNode->_binary = _bitsInDecimal; /* command translated to 'word' - 12 bits converted to decimal number */

    /* updates new node's status: */
    strcpy(_newNode->_referencedSymbolName, _symbolName);

    if (_newNode->_referencedSymbolName[0] == '%') /* checks if command is jmp */
    {
        /* remove % sign from name of symbol*/
        memmove(_newNode->_referencedSymbolName, _newNode->_referencedSymbolName + 1, strlen(_newNode->_referencedSymbolName));
        _newNode->_isFirstOperandRelativeAddress = TRUE; /* operand is relative */
    }

    if (strcmp(_newNode->_referencedSymbolName, NO_SYMBOL) != 0)
    {
        _newNode->_isSymbol = TRUE;
        _newNode->_lineNumber = _lineNumber;
    }

    _newNode->_next = NULL;
    _traverseNode->_next = _newNode;
}

/* Adding command to image table for data only */
void append_to_image_data(head *_pointerToFirstNode, int *_dataCounter, char _attribute, unsigned int _bitArray)
{
    /* append to image table data */
    image *_newNode = (image *)malloc(sizeof(image)); /*allocate memory for node*/
    init_image(_newNode);

    if (!is_successfull_malloc(_newNode))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    _pointerToFirstNode->_counter++;

    if (is_empty_image_data(_pointerToFirstNode))
        append_first_image_data_node(_pointerToFirstNode, _dataCounter, _attribute, _bitArray, _newNode);
    else
        append_image_data_node(_pointerToFirstNode, _dataCounter, _attribute, _bitArray, _newNode);
}

/* Adding only first node to image table of data*/
void append_first_image_data_node(head *_pointerToFirstNode, int *_dataCounter, char _attribute, unsigned int _bitArray, image *_newNode)
{

    _newNode->_value = *_dataCounter;
    _newNode->_attribute = _attribute; /* _attribute return char (A-R-E) */
    _newNode->_binary = _bitArray;     /* command translated to 'word' - 12 bits converted to decimal number */
    _newNode->_next = NULL;
    _pointerToFirstNode->_headOfImageData = _newNode;
}

/* Adding second and following nodes to the image table of data */
void append_image_data_node(head *_pointerToFirstNode, int *_dataCounter, char _attribute, unsigned int _bitArray, image *_newNode)
{
    /*Initialize pointer to iterate through list*/
    image *_traverseNode = _pointerToFirstNode->_headOfImageData;

    while (_traverseNode->_next != NULL) /* reaching last node */
        _traverseNode = _traverseNode->_next;

    _newNode->_value = *_dataCounter;
    _newNode->_attribute = _attribute; /* _attribute return char (A-R-E) */
    _newNode->_binary = _bitArray;     /* command translated to 'word' - 12 bits converted to decimal number */
    _newNode->_next = NULL;
    _traverseNode->_next = _newNode;
}

/* Function dividing between data and instruction commands and add them to designated image table */
void append_to_image_table(head *_pointerToFirstNode, line *_parsedLine, int *_dataCounter, int *_instructionCounter)
{
    /* command is .data or .string only*/
    if (_parsedLine->_isString || _parsedLine->_isData)
    {

        if (_parsedLine->_isString)
        {
            append_string_to_image_data(_parsedLine, _pointerToFirstNode, _dataCounter, _instructionCounter);
        }

        else if (_parsedLine->_isData)
        {
            append_data_to_image_data(_parsedLine, _pointerToFirstNode, _dataCounter, _instructionCounter);
        }
    }

    else /* command is instruction */
    {
        int arrayOfBits[12] = {0}; /* 'word' of 12 bits will be mirrored to array of 12 cells, for each digit (0 or 1) one cell in array */
        char attribute;
        unsigned int binaryInDecimal; /* represent 'word' in decimal */
        size_t i;

        attribute = ABSOLUTE;

        bit_array_organizer(_parsedLine, arrayOfBits);                   /* collect information of opcode, funct, source and direction and mirroring to array */
        binaryInDecimal = convert_array_of_bits_to_decimal(arrayOfBits); /* convert mirrored array to one deimal number */

        append_to_image_code(_pointerToFirstNode, _instructionCounter, attribute, binaryInDecimal,
                             NO_SYMBOL, _parsedLine->_number);

        *_instructionCounter = *_instructionCounter + 1; /* count instructions in the assembler code */

        if (is_exist_first_operand(_parsedLine))
        {
            i = 1;
            if (_parsedLine->_isFirstOperandImmediateAddress)
            {
                if (_parsedLine->_isLabel)
                    i++;

                append_to_image_code_immidiate_address(i, _parsedLine, _pointerToFirstNode, _instructionCounter);
            }

            if (_parsedLine->_isFirstOperandDirectAddress)
            {

                if (_parsedLine->_isLabel)
                    i++;

                append_to_image_code_direct_address(i, _parsedLine, _pointerToFirstNode, _instructionCounter);
            }

            if (_parsedLine->_isFirstOperandRelativeAddress)
            {
                if (_parsedLine->_isLabel)
                    i++;

                append_to_image_code_relative_address(i, _parsedLine, _pointerToFirstNode, _instructionCounter);
            }

            if (_parsedLine->_isFirstOperandRegisterDirect)
            {
                bool isFirstRegister = TRUE;
                append_to_image_code_register_address(_parsedLine, _pointerToFirstNode, _instructionCounter, isFirstRegister);
            }
        }

        if (is_exist_second_operand(_parsedLine))
        {
            i = 2;
            if (_parsedLine->_isSecondOperandImmediateAddress)
            {
                if (_parsedLine->_isLabel)
                    i++;

                append_to_image_code_immidiate_address(i, _parsedLine, _pointerToFirstNode, _instructionCounter);
            }

            if (_parsedLine->_isSecondOperandDirectAddress)
            {
                if (_parsedLine->_isLabel)
                    i++;

                append_to_image_code_direct_address(i, _parsedLine, _pointerToFirstNode, _instructionCounter);
            }

            if (_parsedLine->_isSecondOperandRegisterDirect)
            {
                bool isFirstRegister = FALSE;
                append_to_image_code_register_address(_parsedLine, _pointerToFirstNode, _instructionCounter, isFirstRegister);
            }
        }
    }
}
/* Adding command .string to image table for data only */
void append_string_to_image_data(line *_parsedLine, head *_pointerToFirstNode, int *_dataCounter, int *_instructionCounter)
{
    binary *bitArray;
    size_t _firstInstanceIndex; /* first instance index of " char */
    size_t _lastInstanceIndex;  /* last instance index of " char */
    char *_firstInstance;       /* first instance of " char */
    char *_lastInstance;        /* last instance of " char */
    char attribute;

    bitArray = (binary *)malloc(sizeof(binary));
    if (!is_successfull_malloc(bitArray))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    attribute = ABSOLUTE;

    _firstInstance = strchr(_parsedLine->_lineStr, '"');
    _firstInstanceIndex = (int)(_firstInstance - _parsedLine->_lineStr); /* finds first " char index in line */
    _firstInstanceIndex++;                                               /* finds index of 1st ascii char in string */

    _lastInstance = strrchr(_parsedLine->_lineStr, '"');
    _lastInstanceIndex = (int)(_lastInstance - _parsedLine->_lineStr); /* finds last " char index in line*/

    while (_firstInstanceIndex < _lastInstanceIndex) /* pass by each char and add to the table one by one */
    {
        bitArray->word_for_asci = _parsedLine->_lineStr[_firstInstanceIndex];
        append_to_image_data(_pointerToFirstNode, _dataCounter, attribute, bitArray->word_for_asci);
        *_dataCounter = *_dataCounter + 1;
        _firstInstanceIndex++; /* mooving to next char */
    }
    bitArray->word_for_asci = 0; /* represent end of string */
    append_to_image_data(_pointerToFirstNode, _dataCounter, attribute, bitArray->word_for_asci);
    *_dataCounter = *_dataCounter + 1;

    free(bitArray);
}

/* Adding command of .data to image table for data only */
void append_data_to_image_data(line *_parsedLine, head *_pointerToFirstNode, int *_dataCounter, int *_instructionCounter)
{
    /*parse a line and get a digits one by one and send them to append_to_image_data*/
    char attribute;
    size_t i;
    binary *bitArray;

    bitArray = (binary *)malloc(sizeof(binary));
    if (!is_successfull_malloc(bitArray))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    attribute = ABSOLUTE;

    i = 1;
    if (_parsedLine->_isLabel)
        i++;

    while (*_parsedLine->_lineWords[i] != '\0') /* inserting each number one by one into image table */
    {
        bitArray->word_for_asci = atoi(_parsedLine->_lineWords[i]);
        append_to_image_data(_pointerToFirstNode, _dataCounter, attribute, bitArray->word_for_asci);
        *_dataCounter = *_dataCounter + 1;

        i++;
    }

    free(bitArray);
}

/* Create a 'word' from destination, source, funct and opcode and mirroring into int array*/
void bit_array_organizer(line *_parsedLine, int *arrayOfBits)
{
    /* each cell in the array represent a digit of binary code (0 or 1) */
    int _destination = 0, _source = 0, _funct = 0, _opcode = 0;
    binary *bitArray;
    size_t i;

    bitArray = (binary *)malloc(sizeof(binary));
    if (!is_successfull_malloc(bitArray))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    i = 0;
    if (_parsedLine->_isLabel)
        i = 1;

    convert_to_destination(_parsedLine, &_destination); /* getting destination  */
    if (is_exist_second_operand(_parsedLine))
    {
        convert_to_source(_parsedLine, &_source); /* getting source */
        bitArray->source = _source;
    }
    else
        bitArray->source = 0;
    convert_operator(_parsedLine->_lineWords[i], &_funct, &_opcode); /* gitting opcode and funct */

    /* initialize received info into 'word' - 12 bits */
    bitArray->destination = _destination;
    bitArray->funct = _funct;
    bitArray->opcode = _opcode;

    create_array_of_bits(arrayOfBits, bitArray);
    free(bitArray);
}

/* Adding an immidiate addressed operand to image table of code only */
void append_to_image_code_immidiate_address(size_t i, line *_parsedLine, head *_pointerToFirstNode, int *_instructionCounter)
{
    int operandInDecimal;
    char attribute;
    char *str = (char *)malloc(MAX_WORD_LENGTH + 1);
    char *pointerToStr = str;

    if (!is_successfull_malloc(str))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    /* removes '%' char from begining of string and saves decimal value */
    strcpy(str, _parsedLine->_lineWords[i]);
    str++;
    operandInDecimal = atoi(str);

    attribute = ABSOLUTE;

    append_to_image_code(_pointerToFirstNode, _instructionCounter, attribute, operandInDecimal,
                         NO_SYMBOL, _parsedLine->_number);

    *_instructionCounter = *_instructionCounter + 1;

    free(pointerToStr);
}

/* Adding a direct addressed operand to image table of code only */
void append_to_image_code_direct_address(size_t i, line *_parsedLine, head *_pointerToFirstNode, int *_instructionCounter)
{
    char attribute;
    char *_symbolName;
    attribute = '\0';

    _symbolName = _parsedLine->_lineWords[i];

    append_to_image_code(_pointerToFirstNode, _instructionCounter, attribute, (unsigned int)NULL, _symbolName, _parsedLine->_number);
    *_instructionCounter = *_instructionCounter + 1;
}

/* Adding aregister addresed operand to image table of code only */
void append_to_image_code_register_address(line *_parsedLine, head *_pointerToFirstNode, int *_instructionCounter, bool isFirstRegister)
{
    int rigisterInBinary;
    char attribute;

    if (isFirstRegister)
        rigisterInBinary = get_first_register_in_binary(_parsedLine);
    else
        rigisterInBinary = get_second_register_in_binary(_parsedLine);

    attribute = ABSOLUTE;

    append_to_image_code(_pointerToFirstNode, _instructionCounter, attribute, rigisterInBinary,
                         NO_SYMBOL, _parsedLine->_number);

    *_instructionCounter = *_instructionCounter + 1;
}

/* Adding a relative addressed operand to image table of code only */
void append_to_image_code_relative_address(size_t i, line *_parsedLine, head *_pointerToFirstNode, int *_instructionCounter)
{
    char attribute;
    char *_symbolName;

     _symbolName = _parsedLine->_lineWords[i];

    attribute = ABSOLUTE;
    append_to_image_code(_pointerToFirstNode, _instructionCounter, attribute, (unsigned int)NULL, _symbolName, _parsedLine->_number);
    *_instructionCounter = *_instructionCounter + 1;
}

/* Converting operand to designated opcode and funct */
void convert_operator(char *_operator, int *_funct, int *_opcode)
{
    if (strcmp(_operator, "cmp") == 0)
        *_opcode = 1;

    else if (strcmp(_operator, "add") == 0)
    {
        *_funct = 10;
        *_opcode = 2;
    }

    else if (strcmp(_operator, "sub") == 0)
    {
        *_funct = 11;
        *_opcode = 2;
    }

    else if (strcmp(_operator, "lea") == 0)
        *_opcode = 4;

    else if (strcmp(_operator, "clr") == 0)
    {
        *_funct = 10;
        *_opcode = 5;
    }

    else if (strcmp(_operator, "not") == 0)
    {
        *_funct = 11;
        *_opcode = 5;
    }

    else if (strcmp(_operator, "inc") == 0)
    {
        *_funct = 12;
        *_opcode = 5;
    }

    else if (strcmp(_operator, "dec") == 0)
    {
        *_funct = 13;
        *_opcode = 5;
    }

    else if (strcmp(_operator, "jmp") == 0)
    {
        *_funct = 10;
        *_opcode = 9;
    }

    else if (strcmp(_operator, "bne") == 0)
    {
        *_funct = 11;
        *_opcode = 9;
    }

    else if (strcmp(_operator, "jsr") == 0)
    {
        *_funct = 12;
        *_opcode = 9;
    }

    else if (strcmp(_operator, "red") == 0)
        *_opcode = 12;

    else if (strcmp(_operator, "prn") == 0)
        *_opcode = 13;

    else if (strcmp(_operator, "rts") == 0)
        *_opcode = 14;

    else if (strcmp(_operator, "stop") == 0)
        *_opcode = 15;
}

/* Get type of destination operand in decimal - 0 - Immediate, 1 - Direct, 2 - Relative, 3 - Register */
void convert_to_destination(line *_parsedLine, int *_destination)
{
    if (is_exist_second_operand(_parsedLine))
    {
        if (_parsedLine->_isSecondOperandImmediateAddress)
            *_destination = 0;
        if (_parsedLine->_isSecondOperandDirectAddress)
            *_destination = 1;
        if (_parsedLine->_isSecondOperandRegisterDirect)
            *_destination = 3;
    }
    else /* if is_exist_first_operand(_parsedLine))*/
    {
        if (_parsedLine->_isFirstOperandImmediateAddress)
            *_destination = 0;
        if (_parsedLine->_isFirstOperandDirectAddress)
            *_destination = 1;
        if (_parsedLine->_isFirstOperandRelativeAddress)
            *_destination = 2;
        if (_parsedLine->_isFirstOperandRegisterDirect)
            *_destination = 3;
    }
}

/* Get type of source operand in decimal - 0 - Immediate, 1 - Direct, 2 - Relative, 3 - Register */
void convert_to_source(line *_parsedLine, int *_source)
{
    if (_parsedLine->_isFirstOperandImmediateAddress)
        *_source = 0;
    if (_parsedLine->_isFirstOperandDirectAddress)
        *_source = 1;
    if (_parsedLine->_isFirstOperandRelativeAddress)
        *_source = 2;
    if (_parsedLine->_isFirstOperandRegisterDirect)
        *_source = 3;
}

/* Function searchs for received symbol list in label linked list */
symbol *get_symbol(char *_symbolName, head *_labels)
{
    symbol *_symbolTraversalNode = _labels->_headOfSymbolTable;

    while (_symbolTraversalNode != NULL)
    {
        if (strcmp(_symbolName, _symbolTraversalNode->_symbolName) == 0)
            return _symbolTraversalNode;
        _symbolTraversalNode = _symbolTraversalNode->_next;
    }
    return NULL;
}