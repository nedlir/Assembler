#include "constants.h"
#include "line_data_structure.h"
#include "str_handling.h"
#include "reserved_words.h"

/* Initializes a line data structure */
void initialize_line_structure(line *_line)
{
    size_t i; /* counter to traverse _lineWords */

    /* Clear all strings: */
    for (i = 0; i < MAX_WORDS_IN_LINE; i++)
        memset(_line->_lineWords[i], '\0', sizeof(_line->_lineWords[i]));
    memset(_line->_lineStr, 0, sizeof(_line->_lineStr));
    memset(_line->_errorDescription, 0, sizeof(_line->_errorDescription));

    /* Set all boolean flags to FALSE: */
    _line->_isLabel = FALSE;
    _line->_isData = FALSE;
    _line->_isString = FALSE;
    _line->_isExtern = FALSE;
    _line->_isEntry = FALSE;
    _line->_isCommand = FALSE;
    _line->_isFirstOperandImmediateAddress = FALSE;
    _line->_isFirstOperandDirectAddress = FALSE;
    _line->_isFirstOperandRelativeAddress = FALSE;
    _line->_isFirstOperandRegisterDirect = FALSE;
    _line->_isSecondOperandImmediateAddress = FALSE;
    _line->_isSecondOperandDirectAddress = FALSE;
    _line->_isSecondOperandRelativeAddress = FALSE;
    _line->_isSecondOperandRegisterDirect = FALSE;
    _line->_isUnknownWord = FALSE;
    _line->_isError = FALSE;
    _line->_isIgnore = FALSE;
}

/* Checks if a given str is a comment */
bool is_comment(char *_str)
{
    if (is_str_starts_with(_str, ';'))
        return TRUE;
    else
        return FALSE;
}

/* Checks if a given str is a label */
bool is_label(char *_str)
{
    if (is_str_ends_with(_str, ':'))
        return TRUE;
    else
        return FALSE;
}

/* Raises a label flag in line */
void raise_label_flag(line *_line)
{
    _line->_isLabel = TRUE;
}

/* Raises a first word flag in line */
void raise_first_word_flags(line *_line, char *_firstWord)
{
    if (is_instruction(_firstWord))
        raise_instruction_flag(_line, _firstWord);

    else if (is_command(_firstWord))
        raise_command_flag(_line);

    else if (!_line->_isLabel && !is_reserved_word(_firstWord)) /* if unknown word */
        raise_unknown_word_flag(_line);
}

/* Raises an instruction flag in line based on a given str */
void raise_instruction_flag(line *_line, char *_firstWord)
{
    if (strcmp(_firstWord, ".data") == 0)
        _line->_isData = TRUE;

    else if (strcmp(_firstWord, ".string") == 0)
        _line->_isString = TRUE;

    else if (strcmp(_firstWord, ".extern") == 0)
        _line->_isExtern = TRUE;

    else if (strcmp(_firstWord, ".entry") == 0)
        _line->_isEntry = TRUE;
}

/* Raises a command flag in line based on a given str */
void raise_command_flag(line *_line)
{
    _line->_isCommand = TRUE;
}

/* Raises unknown word flag in line */
void raise_unknown_word_flag(line *_line)
{
    _line->_isUnknownWord = TRUE;
}

/* Raises ignore word flag in line */
void raise_ignore_flag(line *_line)
{
    _line->_isIgnore = TRUE;
}

/* Raises operand flags in line */
void raise_operand_flags(line *_line)
{
    size_t i; /* Counter which starts at first operand */

    i = 1;
    if (_line->_isLabel) /* if a label exists, start checking from next element */
        i++;

    /* Determines first operand flags: */
    if (is_immediate_address(_line->_lineWords[i]))
        _line->_isFirstOperandImmediateAddress = TRUE;

    else if (is_relative_address(_line->_lineWords[i]))
        _line->_isFirstOperandRelativeAddress = TRUE;

    else if (is_register(_line->_lineWords[i]))
        _line->_isFirstOperandRegisterDirect = TRUE;

    else if (!is_str_empty(_line->_lineWords[i]) && !is_reserved_word(_line->_lineWords[i])) /* if operand is a label */
        _line->_isFirstOperandDirectAddress = TRUE;

    i++; /* sets counter to next word */

    /* Determines second operand flags: */
    if (is_immediate_address(_line->_lineWords[i]))
        _line->_isSecondOperandImmediateAddress = TRUE;

    else if (is_register(_line->_lineWords[i]))
        _line->_isSecondOperandRegisterDirect = TRUE;

    else if (is_relative_address(_line->_lineWords[i]))
        _line->_isSecondOperandRelativeAddress = TRUE;

    else if (!is_str_empty(_line->_lineWords[i]) && !is_reserved_word(_line->_lineWords[i])) /* if operand is a label */
        _line->_isSecondOperandDirectAddress = TRUE;
}

/* Checks if a given str is immediate address  */
bool is_immediate_address(char *_str)
{
    if (is_str_starts_with(_str, '#'))
        return TRUE;
    else
        return FALSE;
}

/* Checks if a given str is relative address  */
bool is_relative_address(char *_str)
{
    if (is_str_starts_with(_str, '%'))
        return TRUE;
    else
        return FALSE;
}

/* Checks if first operand exists in line */
bool is_exist_first_operand(line *_line)
{
    if (_line->_isFirstOperandImmediateAddress || _line->_isFirstOperandDirectAddress ||
        _line->_isFirstOperandRelativeAddress || _line->_isFirstOperandRegisterDirect)
        return TRUE;
    else
        return FALSE;
}

/* Checks if second operand exists in line */
bool is_exist_second_operand(line *_line)
{

    if (_line->_isSecondOperandImmediateAddress || _line->_isSecondOperandDirectAddress ||
        _line->_isSecondOperandRelativeAddress || _line->_isSecondOperandRegisterDirect)
        return TRUE;
    else
        return FALSE;
}