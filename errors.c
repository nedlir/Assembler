#include "constants.h"
#include "line_data_structure.h"
#include "str_handling.h"
#include "reserved_words.h"
#include "linked_list_data_structure.h"
#include "errors.h"

/* Finds and reports errors in line (first pass) */
void report_errors_in_line(line *_line)
{
    /* The errors reported through this function will be kept in line's _errorDescription variable */
    /* Once the first error is found, the function will stop searching for further errors in the line */

    char _error[MAX_ERROR_LENGTH]; /* the error description holder */

    if (!is_valid_line_length(_line))
    {
        sprintf(_error, "%sInvalid line length detected.%s\n"
                        "Line length may not be greater than %d characters.",
                ERROR_DESCRIPTION_COLOR, STOP_COLOR,
                (MAX_LINE_LENGTH));
        report_an_error(_line, _error);
        return;
    }

    if (!is_valid_commas(_line))
    {
        sprintf(_error, "%sIllegal commas usage detected.%s\n"
                        "Consecutive commas, "
                        "comma at end of sentence "
                        "or comma before first variable are not allowed.",
                ERROR_DESCRIPTION_COLOR, STOP_COLOR);
        report_an_error(_line, _error);
        return;
    }

    if (_line->_isLabel)
    {
        if (!is_valid_label(_line))
        {
            sprintf(_error, "%sInvalid label name usage detected.%s\n"
                            "\"%s\" is unauthorized label name.\n"
                            "Label name can not be greater than %d characters, "
                            "utilize saved words, "
                            "start with a non alpha character or contain special signs.",
                    ERROR_DESCRIPTION_COLOR, STOP_COLOR, _line->_lineWords[0], MAX_LABEL_LENGTH);
            report_an_error(_line, _error);
            return;
        }
    }

    if (_line->_isData)
    {
        if (!is_valid_data(_line))
        {
            sprintf(_error, "%sInvalid data variable detected.%s\n"
                            "Numbers must be decimal values only, "
                            "may not overflow \"%.0f\" or underflow \"%.0f\", "
                            "special signs or letters are unauthorized and "
                            "spaces between numbers are not allowed.",
                    ERROR_DESCRIPTION_COLOR, STOP_COLOR, MAX_MACHINE_INT, MIN_MACHINE_INT);
            report_an_error(_line, _error);
            return;
        }
    }

    if (_line->_isString)
    {
        if (!is_valid_string(_line))
        {
            sprintf(_error, "%sInvalid string detected.%s\n"
                            "String must be contained between two \" characters "
                            "and contain only printable ascii characters.",
                    ERROR_DESCRIPTION_COLOR, STOP_COLOR);
            report_an_error(_line, _error);
            return;
        }
    }

    if (_line->_isCommand)
    {
        if (!is_valid_operands_addressing(_line))
        {
            sprintf(_error, "%sInvalid addressing detected.%s\n"
                            "Usage of unauthorized addressing methods is not allowed.",
                    ERROR_DESCRIPTION_COLOR, STOP_COLOR);
            report_an_error(_line, _error);
            return;
        }
    }

    if (is_extraneous_word(_line))
    {
        sprintf(_error, "%sExtraneous word detected.%s\n"
                        "Usage of extraneous words after instruction or command variables is not allowed.",
                ERROR_DESCRIPTION_COLOR, STOP_COLOR);
        report_an_error(_line, _error);
        return;
    }

    if (is_missing_word(_line))
    {
        sprintf(_error, "%sToo few variables for instruction or command detected.%s\n"
                        "Correct amount of variables must be provided.",
                ERROR_DESCRIPTION_COLOR, STOP_COLOR);
        report_an_error(_line, _error);
        return;
    }

    if (_line->_isUnknownWord)
    {
        sprintf(_error, "%sInvalid word detected.%s\n"
                        "Usage of undefined words is not allowed.",
                ERROR_DESCRIPTION_COLOR, STOP_COLOR);
        report_an_error(_line, _error);
        return;
    }
}

/* Finds and reports errors in symbol declerations (first pass) */
void report_errors_in_symbol_first_pass(line *_line, head *_labels, size_t _fileLength)
{
    /* This function will search for entries that are declared as external, 
        for local labels that are also decalred as external 
        and for non-existant entries */

    size_t _lineNumber;                                         /* counter to traverse lines array */
    size_t i;                                                   /* counter to mark label location of declared entry/extern */
    symbol *_symbol = NULL;                                     /* will assist us receiving data regarding a symbol*/
    symbol *_labelsTraversalNode = _labels->_headOfSymbolTable; /* will assist us traversing symbol table */
    char _error[MAX_ERROR_LENGTH];
    char *_lineSymbolName = NULL; /* helper variable for redability */

    _lineNumber = 0;
    while (_lineNumber < _fileLength)
    {
        i = 1;
        /* if a label precedes the .entry/.extern we will refer to the next symbol */
        if (_line[_lineNumber]._isLabel)
            i++;

        _lineSymbolName = _line[_lineNumber]._lineWords[i]; /* symbol in case of ".entry" / ".extern" */

        if (_line[_lineNumber]._isEntry && !_line[_lineNumber]._isError)
        {
            if (!is_exist_symbol(_lineSymbolName, _labelsTraversalNode))
            {
                sprintf(_error, "%sInvalid entry decleration detected.%s\n"
                                "Entry \"%s\" is not declared in the file.",
                        ERROR_DESCRIPTION_COLOR, STOP_COLOR, _lineSymbolName);

                report_an_error(&_line[_lineNumber], _error);
            }

            else /* if is declared entry already declared as external */
            {
                _symbol = get_symbol(_lineSymbolName, _labels);

                if (_symbol->_isExtern)
                {
                    if (!_line->_isError) /* report error only if there are no other errors */
                    {
                        sprintf(_error, "%sInvalid entry decleration detected.%s\n"
                                        "Entry \"%s\" is declared as external in the file.",
                                ERROR_DESCRIPTION_COLOR, STOP_COLOR, _lineSymbolName);

                        report_an_error(&_line[_lineNumber], _error);
                    }
                }
            }
        }

        _lineNumber++;
    }
}

void report_existing_symbol_error(line *_line, char *_symbolName, symbol *_labels)
{
    /* Once the first instance of the symbol is found, the function will stop searching for further existence of the symbol */

    char _error[MAX_ERROR_LENGTH];
    symbol *_labelsTraversalNode = NULL;

    _labelsTraversalNode = _labels;
    if (is_exist_symbol(_symbolName, _labelsTraversalNode))
    {
        if (!_line->_isError) /* report error only if there are no other errors */
        {
            sprintf(_error, "%sInvalid symbol usage.%s\n"
                            "Declared symbol \"%s\" has multiple declerations in the program.",
                    ERROR_DESCRIPTION_COLOR, STOP_COLOR, _symbolName);
            report_an_error(_line, _error);
            return;
        }
    }
}

/* Finds and reports errors in symbol declerations (second pass) */
bool report_errors_in_symbol_second_pass(head *_labels, head *_imageCode, line *_lines, size_t _fileLength)
{
    bool _isErrorsFound = FALSE;
    /* all referenced symbols which do not exist in program: */
    if (!report_non_existent_symbol(_labels, _imageCode, _lines, _fileLength))
        _isErrorsFound = TRUE;

    /* At this point we know that referenced symbol exists, we will check it is also valid logically: */
    if (!report_errornous_relative_symbols(_labels, _imageCode, _lines, _fileLength))
        _isErrorsFound = TRUE;

    if(_isErrorsFound)
        return FALSE;
    else
        return TRUE;
}

/* Returns FALSE and updates an error if a referenced label is errornous logically in the program */
bool report_errornous_relative_symbols(head *_labels, head *_imageCode, line *_lines, size_t _fileLength)
{
    /* relative referenced labels can't be .data/.string types or .extern */
    /* relative addressing receives only labels and only 1 operand (which is a symbol)*/
    size_t i; /* counter to point to operand value */
    size_t _lineNumber;
    bool _isErrorsFound;
    char *_operandSymbol;
    char _errorDescription[MAX_ERROR_LENGTH];
    symbol *_symbol = NULL;
    _isErrorsFound = FALSE;

    _lineNumber = 0;
    while (_lineNumber < _fileLength)
    {
        if (!_lines[_lineNumber]._isError && _lines[_lineNumber]._isFirstOperandRelativeAddress) /* check for error only for errorless lines */
        {
            i = 1; /* first operand */
            if (_lines[_lineNumber]._isLabel)
                i++;

            /* Labels referred to may start with '%' char */
            if (is_str_starts_with(_lines[_lineNumber]._lineWords[i], '%'))
                _operandSymbol = _lines[_lineNumber]._lineWords[i] + 1;
            else
                _operandSymbol = _lines[_lineNumber]._lineWords[i];

            _symbol = get_symbol(_operandSymbol, _labels);

            if (_symbol->_isExtern)
            {
                _isErrorsFound = TRUE;

                sprintf(_errorDescription, "%sReferenced symbol \"%s\" is declared as external in the program.%s"
                                           "\nRelative addressing operands have to be local symbols "
                                           "and can not be data symbols.",
                        ERROR_DESCRIPTION_COLOR, _operandSymbol, STOP_COLOR);

                report_an_error(&_lines[_lineNumber], _errorDescription);

                printf("\n%s", _lines[_lineNumber]._errorDescription);
            }

            if (_symbol->_isData)
            {
                _isErrorsFound = TRUE;

                sprintf(_errorDescription, "%sReferenced symbol \"%s\" is declared as data in the program.%s"
                                           "\nRelative addressing operands have to be local symbols "
                                           "and can not be data symbols.",
                        ERROR_DESCRIPTION_COLOR, _operandSymbol, STOP_COLOR);

                report_an_error(&_lines[_lineNumber], _errorDescription);

                printf("\n%s", _lines[_lineNumber]._errorDescription);
            }
        }

        _lineNumber++;
    }

    if (_isErrorsFound)
        return FALSE;
    else
        return TRUE;
}

/* Returns FALSE and updates an error if a referenced label does not exist in the program */
bool report_non_existent_symbol(head *_labels, head *_imageCode, line *_lines, size_t _fileLength)
{
    image *_imageTraversalNode = _imageCode->_headOfImageCode;
    symbol *_labelsTraversalNode = _labels->_headOfSymbolTable;
    bool _isErrorsFound;

    _isErrorsFound = FALSE;

    /* Checks that each symbol referred in line actually exists in symbol table: */
    while (_imageTraversalNode != NULL) /* Traverse the image code list */
    {
        _labelsTraversalNode = _labels->_headOfSymbolTable;
        if (strcmp(_imageTraversalNode->_referencedSymbolName, NO_SYMBOL) != 0)
        {
            if (!is_exist_symbol(_imageTraversalNode->_referencedSymbolName, _labelsTraversalNode))
            {
                sprintf(_imageTraversalNode->_errorDescription, "Error on %sline %d:%s %sInvalid symbol usage.%s\n"
                                                                "Referenced symbol \"%s\" is not declared in the program.",
                        LINE_COLOR, _imageTraversalNode->_lineNumber, STOP_COLOR,
                        ERROR_DESCRIPTION_COLOR, STOP_COLOR,
                        _imageTraversalNode->_referencedSymbolName);

                printf("\n%s", _imageTraversalNode->_errorDescription);

                _isErrorsFound = TRUE;
            }
        }
        _imageTraversalNode = _imageTraversalNode->_next;
    }

    if (_isErrorsFound)
        return FALSE;
    else
        return TRUE;
}

/* Checks if line's length is valid */
bool is_valid_line_length(line *_line)
{
    if (strlen(_line->_lineStr) > MAX_LINE_LENGTH)
        return FALSE;

    return TRUE;
}

/* Checks if label is valid */
bool is_valid_label(line *_line)
{
    /* "_line->_lineWords[0]" is the label location */

    if (is_reserved_word(_line->_lineWords[0]))
        return FALSE;

    if (!is_valid_label_length(_line->_lineWords[0]))
        return FALSE;

    if (!is_valid_label_name(_line->_lineWords[0]))
        return FALSE;

    return TRUE;
}

/* Checks if label length is valid */
bool is_valid_label_length(char *_str)
{
    if (strlen(_str) < MAX_LABEL_LENGTH)
        return TRUE;

    return FALSE;
}

/* Checks if label name is valid */
bool is_valid_label_name(char *_str)
{
    /* Label name must start with alpha numeric character and contain only alpha numeric values or numbers without special signs */

    size_t i;

    /* Checks if first char of label is alphabet letter: */
    if (!isalpha(_str[0]))
        return FALSE;

    /* Checks if there are any non alpha-numeric chars: */
    for (i = 1; _str[i] != '\0'; i++)
        if (!isalnum(_str[i]))
            return FALSE;

    return TRUE;
}

/* Checks if commas in line are valid */
bool is_valid_commas(line *_line)
{
    /* Checks validity of commas: */
    if (!_line->_isString) /* '.string' may have a string "tHaT,, lOoKs, , LiKe, ,ThIs,,,," */
        if (is_commas_consecutive(_line->_lineStr))
            return FALSE;

    if (is_str_ends_with(_line->_lineStr, ',')) /* Check commas at end of line */
        return FALSE;

    if (is_commas_between_first_and_second_word(_line->_lineStr))
        return FALSE;

    return TRUE;
}

/* Checks if there are commas between first and second word */
bool is_commas_between_first_and_second_word(char *_str)
{
    char *_commaLocator = _str; /* This variable will help locate commas between first and second word */
    size_t i;                   /* counter to help traverse comma locator */

    i = 0;
    while (!isspace(_commaLocator[i]) && _commaLocator[i] != '\0')
        i++;
    /* At this moment we reached the first space in the sentence after the first word */

    while (isspace(_commaLocator[i]) && _commaLocator[i] != '\0')
        i++;
    /* At this moment we reached the first char which is not a space */

    if (_commaLocator[i] == ',')
        return TRUE;

    return FALSE;
}

/* Checks if there are consecutive commas  */
bool is_commas_consecutive(char *_str)
{
    size_t i;
    char prev = _str[0];
    char next = _str[0];

    /* Checks if there are 2 consecutive commas in the command */
    for (i = 0; _str[i] != '\0'; i++)
    {
        if (prev == ',' && prev == next)
            return TRUE;

        if (!isspace(_str[i]))
        {
            prev = next;
            next = _str[i];
        }
    }
    return FALSE;
}

/* Checks if data in line is valid */
bool is_valid_data(line *_line)
{
    if (!is_valid_data_numbers(_line))
        return FALSE;

    if (!is_valid_data_limit(_line))
        return FALSE;

    return TRUE;
}

/* Checks if data is only numeric values */
bool is_valid_data_numbers(line *_line)
{
    size_t i; /* serves as counter for data elements */

    i = 1; /* Counting starts from 2nd elemtnt of array */

    if (_line->_isLabel) /* if a label exists, start checking from next element */
        i++;

    for (i = i + 0; i < MAX_WORDS_IN_LINE && _line->_lineWords[i][0] != '\0'; i++)
        if (!is_str_number(_line->_lineWords[i]))
            return FALSE;

    return TRUE;
}

/* Checks if data is overflowing/underflowing memory */
bool is_valid_data_limit(line *_line)
{
    size_t i; /* serves as counter for data elements */
    int numericValue;

    i = 1; /* Counting starts from 2nd elemtnt of array */

    if (_line->_isLabel) /* if a label exists, start checking from next element */
        i++;

    for (i = i + 0; i < MAX_WORDS_IN_LINE && _line->_lineWords[i][0] != '\0'; i++)
    {
        numericValue = atoi(_line->_lineWords[i]);

        if (numericValue > MAX_MACHINE_INT || numericValue < MIN_MACHINE_INT)
            return FALSE;
    }

    return TRUE;
}

/* Checks if string in line is valid */
bool is_valid_string(line *_line)
{
    size_t i;                   /* serves as counter for string data from start of string */
    size_t _firstInstanceIndex; /* first instance index of " char */
    size_t _lastInstanceIndex;  /* last instance index of " char */
    char *_firstInstance;       /* first instance of " char */
    char *_lastInstance;        /* last instance of " char */

    _firstInstance = strchr(_line->_lineStr, '"');
    _firstInstanceIndex = (int)(_firstInstance - _line->_lineStr);

    _lastInstance = strrchr(_line->_lineStr, '"');
    _lastInstanceIndex = (int)(_lastInstance - _line->_lineStr);

    i = 1; /* Counting starts from 2nd elemtnt of array */
    /* Finds indexes of first and last .string elements: */
    if (_line->_isLabel) /* if a label exists the value to be checked has to be next one */
        i++;

    /* if first word of array is empty, then the rest of the line is empty as well */
    if (is_str_empty(_line->_lineWords[i]))
        return FALSE;

    if (!is_str_starts_with(_line->_lineWords[i], '"'))
        return FALSE;

    /* Checks if " exists in line: */
    if (_firstInstance == NULL || _lastInstance == NULL)
        return FALSE;

    /* Checks if there is only single " in line: */
    if (_firstInstanceIndex == _lastInstanceIndex)
        return FALSE;

    /* Checks if there is extraneous chars after last " in line: */
    for (i = _lastInstanceIndex + 1; _line->_lineStr[i] != '\0'; i++)
        if (!isspace(_line->_lineStr[i]))
            return FALSE;

    return TRUE;
}

/* Checks if operands addressing in line is valid */
bool is_valid_operands_addressing(line *_line)
{
    size_t i; /* Counter to reach command name */

    i = 0;
    if (_line->_isLabel)
        i++;

    if (strcmp(_line->_lineWords[i], "mov") == 0 && !is_valid_mov_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "cmp") == 0 && !is_valid_cmp_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "add") == 0 && !is_valid_add_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "sub") == 0 && !is_valid_sub_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "lea") == 0 && !is_valid_lea_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "clr") == 0 && !is_valid_clr_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "not") == 0 && !is_valid_not_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "inc") == 0 && !is_valid_inc_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "dec") == 0 && !is_valid_dec_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "jmp") == 0 && !is_valid_jmp_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "bne") == 0 && !is_valid_bne_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "jsr") == 0 && !is_valid_jsr_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "red") == 0 && !is_valid_red_addressing(_line))
        return FALSE;
    else if (strcmp(_line->_lineWords[i], "prn") == 0 && !is_valid_prn_addressing(_line))
        return FALSE;
    /* "rts" and "stop" will be verified for addressing validity in 'is_extraneous_instruction_words()' */

    return TRUE;
}

bool is_valid_mov_addressing(line *_line)
{
    /* Checks if origin operand is invalid: */
    if (_line->_isFirstOperandRelativeAddress) /* Method 2 */
        return FALSE;

    /* Checks if destination operand is invalid: */
    if (_line->_isSecondOperandImmediateAddress || _line->_isSecondOperandRelativeAddress) /* Method 0 or Method 2*/
        return FALSE;

    return TRUE;
}
bool is_valid_cmp_addressing(line *_line)
{
    /* Checks if origin operand is invalid: */
    if (_line->_isFirstOperandRelativeAddress) /* Method 2 */
        return FALSE;

    /* Checks if destination operand is invalid: */
    if (_line->_isSecondOperandRelativeAddress) /* Method 2 */
        return FALSE;

    return TRUE;
}

bool is_valid_add_addressing(line *_line)
{
    /* Checks if origin operand is invalid: */
    if (_line->_isFirstOperandRelativeAddress) /* Method 2 */
        return FALSE;

    /* Checks if destination operand is invalid: */
    if (_line->_isSecondOperandImmediateAddress || _line->_isSecondOperandRelativeAddress) /* Method 0 or 2 */
        return FALSE;

    return TRUE;
}

bool is_valid_sub_addressing(line *_line)
{
    /* Checks if origin operand is invalid: */
    if (_line->_isFirstOperandRelativeAddress) /* Method 2 */
        return FALSE;

    /* Checks if destination operand is invalid: */
    if (_line->_isSecondOperandImmediateAddress || _line->_isSecondOperandRelativeAddress) /* Method 0 or 2 */
        return FALSE;

    return TRUE;
}

bool is_valid_lea_addressing(line *_line)
{
    /* Checks if origin operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRelativeAddress ||
        _line->_isFirstOperandRegisterDirect) /* Method 0, 2 or 3 */
        return FALSE;

    /* Checks if destination operand is invalid: */
    if (_line->_isSecondOperandImmediateAddress || _line->_isSecondOperandRelativeAddress) /* Method 0 or 2 */
        return FALSE;

    return TRUE;
}

bool is_valid_clr_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRelativeAddress) /* Method 0 or 2*/
        return FALSE;

    return TRUE;
}

bool is_valid_not_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRelativeAddress) /* Method 0 or 2*/
        return FALSE;

    return TRUE;
}

bool is_valid_inc_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRelativeAddress) /* Method 0 or 2*/
        return FALSE;

    return TRUE;
}

bool is_valid_dec_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRelativeAddress) /* Method 0 or 2*/
        return FALSE;

    return TRUE;
}

bool is_valid_jmp_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRegisterDirect) /* Method 0 or 3*/
        return FALSE;

    return TRUE;
}

bool is_valid_bne_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRegisterDirect) /* Method 0 or 3*/
        return FALSE;

    return TRUE;
}

bool is_valid_jsr_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRegisterDirect) /* Method 0 or 3*/
        return FALSE;

    return TRUE;
}

bool is_valid_red_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandRelativeAddress) /* Method 0 or 2*/
        return FALSE;

    return TRUE;
}

bool is_valid_prn_addressing(line *_line)
{
    /* Checks if destination operand is invalid: */
    if (_line->_isFirstOperandRelativeAddress) /* Method 2*/
        return FALSE;

    return TRUE;
}
bool is_extraneous_word(line *_line)
{
    /* extraneous '.data' variables are checked in 'is_valid_data()' and extraneous '.string' variables are checked in 'is_valid_string()'*/
    if (_line->_isEntry || _line->_isExtern)
        if (is_extraneous_instruction_words(_line))
            return TRUE;

    if (_line->_isCommand)
        if (is_extraneous_command_words(_line))
            return TRUE;

    return FALSE;
}

/* Checks if an extraneous command word exists in line */
bool is_extraneous_command_words(line *_line)
{
    size_t i; /* Counter to reach instruction */
    size_t j; /* Counter to traverse string to check for extraneous words */

    i = 0;
    if (_line->_isLabel)
        i++;

    /* Check logical extraneous words: */

    if (is_zero_operand(_line->_lineWords[i]))
    {
        if (is_exist_first_operand(_line) || is_exist_second_operand(_line))
            return TRUE;

        i = 0; /* location of last word */
        j = 0; /* counter of string */

        if (_line->_isLabel)
            i++;

        while (_line->_lineWords[i][j] != '\0')
        {
            if (isspace(_line->_lineWords[i][j]))
                return TRUE;

            j++;
        }

        i++;                                     /* location of possible extraneous word */
        if (!is_str_empty(_line->_lineWords[i])) /* if exists extraneous word */
            return TRUE;
    }

    if (is_single_operand(_line->_lineWords[i]))
    {
        if (is_exist_second_operand(_line))
            return TRUE;

        i = 1; /* location of last word */
        j = 0; /* counter of string */

        if (_line->_isLabel)
            i++;

        while (_line->_lineWords[i][j] != '\0')
        {
            if (isspace(_line->_lineWords[i][j]))
                return TRUE;

            j++;
        }

        i++;                                     /* location of possible extraneous word */
        if (!is_str_empty(_line->_lineWords[i])) /* if exists extraneous word */
            return TRUE;
    }

    if (is_two_operand(_line->_lineWords[i]))
    {
        i = 2; /* location of last word */
        j = 0; /* counter of string */

        if (_line->_isLabel)
            i++;

        while (_line->_lineWords[i][j] != '\0')
        {
            if (isspace(_line->_lineWords[i][j]))
                return TRUE;

            j++;
        }

        i++;                                     /* location of possible extraneous word */
        if (!is_str_empty(_line->_lineWords[i])) /* if exists extraneous word */
            return TRUE;
    }

    return FALSE;
}

/* Checks if an extraneous instruction word exists in line */
bool is_extraneous_instruction_words(line *_line)
{
    /* This function checks if there are any extraneous words after '.extern' or '.entry' variables.
    Extraneous '.data' variables are checked in 'is_valid_data()' */

    size_t i; /* Counter to reach first word after given instruction variable */
    size_t j; /* Counter to traverse string to check for extraneous words */

    i = 1; /* location of last instruction word */
    if (_line->_isLabel)
        i++;

    j = 0;
    while (isspace(_line->_lineWords[i][j]))
        j++;

    while (_line->_lineWords[i][j] != '\0')
    {
        if (isspace(_line->_lineWords[i][j]))
            return TRUE;

        j++;
    }

    if (!is_str_empty(_line->_lineWords[i + 1])) /* if exists extraneous word */
        return TRUE;

    return FALSE;
}

/* Checks if there is a missing word in line */
bool is_missing_word(line *_line)
{
    if (_line->_isLabel)
        if (is_missing_label_word(_line))
            return TRUE;

    if (_line->_isData || _line->_isString || _line->_isExtern || _line->_isEntry)
        if (is_missing_instruction_word(_line))
            return TRUE;

    if (_line->_isCommand)
        if (is_missing_command_word(_line))
            return TRUE;

    return FALSE;
}

/* Checks if there is a missing word in line given a label */
bool is_missing_label_word(line *_line)
{
    if (is_str_empty(_line->_lineWords[1]))
        return TRUE;

    return FALSE;
}

/* Checks if there is a missing instruction word */
bool is_missing_instruction_word(line *_line)
{
    size_t i; /* Counter to reach first word after given instruction variable */

    i = 1; /* location of first word after the instruction*/
    if (_line->_isLabel)
        i++;

    if (is_str_empty(_line->_lineWords[i]))
        return TRUE;

    return FALSE;
}

/* Checks if there is a missing command word */
bool is_missing_command_word(line *_line)
{
    size_t i; /* Counter to reach first word after given instruction variable */

    i = 0; /* location of the first instruction word */
    if (_line->_isLabel)
        i++;

    if (is_zero_operand(_line->_lineWords[i])) /* if zero operands, no need to check missing words */
        return FALSE;

    if (is_single_operand(_line->_lineWords[i]))
        if (!is_exist_first_operand(_line))
            return TRUE;

    if (is_two_operand(_line->_lineWords[i]))
        if (!is_exist_first_operand(_line) || !is_exist_second_operand(_line))
            return TRUE;

    return FALSE;
}

/* Checks if a symbol exists in symbol table */
bool is_exist_symbol(char *_symbolName, symbol *_labels)
{
    symbol *_traversalNode = _labels;
    while (_traversalNode != NULL)
    {
        if (strcmp(_symbolName, _traversalNode->_symbolName) == 0)
            return TRUE;
        _traversalNode = _traversalNode->_next;
    }

    return FALSE;
}

/* Reports an error to line */
void report_an_error(line *_line, char *_description)
{
    _line->_isError = TRUE;
    sprintf(_line->_errorDescription, "Error on %sline %d:%s %s", LINE_COLOR, _line->_number, STOP_COLOR, _description);
}

/* Finds and reports errors in operand values */
void report_errors_in_operand_values_first_pass(line *_line, head *_labels, size_t _fileLength)
{
    size_t _lineNumber; /* counter to traverse lines array */
    char _error[MAX_ERROR_LENGTH];

    _lineNumber = 0;
    while (_lineNumber < _fileLength)
    {
        if (_line[_lineNumber]._isCommand && !_line[_lineNumber]._isError)
        {
            if (!is_valid_immediate_addressing_values(&_line[_lineNumber], _labels))
            {
                sprintf(_error, "%sInvalid immediate address data variable detected.%s\n"
                                "Numbers must be decimal values only, "
                                "start with a # char, "
                                "may not overflow \"%.0f\" or underflow \"%.0f\", "
                                "special signs or letters are unauthorized and "
                                "spaces between numbers are not allowed.",
                        ERROR_DESCRIPTION_COLOR, STOP_COLOR, MAX_MACHINE_INT, MIN_MACHINE_INT);
                report_an_error(&_line[_lineNumber], _error);
            }

        }

        _lineNumber++;
    }
}

bool is_valid_immediate_addressing_values(line *_line, head *_labels)
{
    size_t i;
    char *_firstOperand;
    char *_secondOperand;

    i = 1;
    if (_line->_isLabel)
        i++;

    if (is_exist_first_operand(_line))
    {
        _firstOperand = _line->_lineWords[i];

        if (_line->_isFirstOperandImmediateAddress)
            if (!is_valid_immediate_address_operand_data(_firstOperand))
                return FALSE;
    }

    if (is_exist_second_operand(_line))
    {
        i++;
        _secondOperand = _line->_lineWords[i];

        if (_line->_isSecondOperandImmediateAddress)
            if (!is_valid_immediate_address_operand_data(_secondOperand))
                return FALSE;
    }

    return TRUE;
}

bool is_valid_immediate_address_operand_data(char *_str)
{
    int _numericValue;

    if (!is_str_starts_with(_str, '#'))
        return FALSE;

    _numericValue = atoi(_str + 1); /* turn string to integer from second char (ignore the '#' of immediate addressing) */

    if (is_immediate_address(_str))
    {
        if (!is_str_number(_str + 1)) /* check string from second char (ignore the '#' of immediate addressing) */
            return FALSE;

        if (_numericValue > MAX_MACHINE_INT || _numericValue < MIN_MACHINE_INT) /* Checks for possible buffer overflow */
            return FALSE;
    }

    return TRUE;
}

bool is_valid_direct_addressing_values(line *_line, head *_labels)
{
    size_t i;
    char *_firstOperand;
    char *_secondOperand;
    symbol *_labelsTraversalNode = _labels->_headOfSymbolTable;

    i = 1;
    if (_line->_isLabel)
        i++;

    if (is_exist_first_operand(_line))
    {
        _firstOperand = _line->_lineWords[i];

        if (_line->_isFirstOperandDirectAddress)
            if (!is_exist_symbol(_firstOperand, _labelsTraversalNode)) /* referenced symbol undeclared in program */
                return FALSE;
    }

    if (is_exist_second_operand(_line) && _line->_isLabel)
    {
        i++;
        _secondOperand = _line->_lineWords[i];

        if (_line->_isSecondOperandDirectAddress)
            if (!is_exist_symbol(_secondOperand, _labelsTraversalNode)) /* referenced symbol undeclared in program */
                return FALSE;
    }

    return TRUE;
}