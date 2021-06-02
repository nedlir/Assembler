#include "constants.h"
#include "utils.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "binary_data_structure.h"
#include "errors.h"
#include "first_analyze.h"
#include "parser.h"
#include "first_pass.h"

/* First pass function will perform first pass of the assembler. If no errors are found a TRUE value will be returned */
bool first_pass(FILE *_filePointer, line *_parsedLinesArray, head *_labelLinkedList, head *_imageDataLinkedList,
                head *_imageCodeLinkedList, int *_instructionCounter, int *_dataCounter, size_t _fileLength)
{
    size_t _lineNumber;
    bool _isErrorsFound;

    _lineNumber = 0;
    _isErrorsFound = FALSE;

    while (_lineNumber < _fileLength)
    {
        initialize_line_structure(&_parsedLinesArray[_lineNumber]);

        fgets(_parsedLinesArray[_lineNumber]._lineStr, (MAX_LINE_LENGTH + EXTRANEOUS_LINE_BUFFER + 1), _filePointer);

        parse_line(&_parsedLinesArray[_lineNumber], _lineNumber);

        if (_parsedLinesArray[_lineNumber]._isIgnore) /* If line is to be ignored (empty or spaces only), continue to next line */
        {
            _lineNumber++;
            continue;
        }

        report_errors_in_line(&_parsedLinesArray[_lineNumber]); /* primary errors will be found at this point */

        if (_parsedLinesArray[_lineNumber]._isError) /* If errors are found in line, continue to next line */
        {
            _lineNumber++;
            continue;
        }
        else
            first_analyze(&_parsedLinesArray[_lineNumber], _labelLinkedList, _imageDataLinkedList, _imageCodeLinkedList,
                          _dataCounter, _instructionCounter);

        _lineNumber++;
    }

    /* post-first-pass errors: */
    /* After finishing processing all the lines and having our symbols we can check for new errors */
    report_errors_in_operand_values_first_pass(_parsedLinesArray, _labelLinkedList, _fileLength); /* errors in operand values could be determined only after the labels are declared */

    report_errors_in_symbol_first_pass(_parsedLinesArray, _labelLinkedList, _fileLength); /* errors regarding existence of symbols could be determined only after the labels are declared */

    /* Some non-existant symbols that are referenced to will be declared as errors during 2nd pass! */

    /* referenced symbols that are errornous, i.e. "jmp %%nonExistantLabel" will be found during second pass */

    if (!print_first_pass_errors(_parsedLinesArray, _fileLength))
        _isErrorsFound = TRUE;
    else
        update_data_symbol_values(_labelLinkedList, _instructionCounter);

    if (_isErrorsFound)
        return FALSE;
    else
        return TRUE;
}

/* Prints all errors in parse line array and returns TRUE if there are no errors found */
bool print_first_pass_errors(line *_parsedLinesArray, size_t _fileLength)
{
    size_t _lineNumber;
    bool _isErrorsFound = FALSE;

    _lineNumber = 0;
    while (_lineNumber < _fileLength)
    {
        if (_parsedLinesArray[_lineNumber]._isError)
        {
            _isErrorsFound = TRUE;
            printf("\n%s", _parsedLinesArray[_lineNumber]._errorDescription);
        }
        _lineNumber++;
    }

    if (_isErrorsFound)
        return FALSE;
    else
        return TRUE;
}

/* Updates data labels with instructionCounter after first pass */
void update_data_symbol_values(head *_labelLinkedList, int *_instructionCounter)
{
    /* Traverses the symbol table and adds the current (and final) instruction counter, 
    this will allow us to have updated addresses for data labels */

    symbol *_traversalNode = NULL; /* This node will help us traverse symbol table */

    _traversalNode = _labelLinkedList->_headOfSymbolTable;
    while (_traversalNode != NULL)
    {
        if (strcmp(_traversalNode->_attribute, DATA_ATTRIBUTE) == 0)
            _traversalNode->_value = _traversalNode->_value + *_instructionCounter;

        _traversalNode = _traversalNode->_next;
    }
}