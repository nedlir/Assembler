#include "constants.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "errors.h"
#include "first_analyze.h"

/* Performs first analyze to the line, an operation that carries out first pass algo on non errornous and non skipable lines*/
void first_analyze(line *_parsedLine, head *_labels, head *_imageData, head *_imageCode,
                   int *_dataCounter, int *_instructionCounter)
{
    /* Checks if there is a label before .entry / .extern decleration and removes it */
    if (is_label_warning(_parsedLine))
    {
        remove_label_from_line(_parsedLine);
        warn_user_about_redundant_label(_parsedLine);
    }

    if (_parsedLine->_isString || _parsedLine->_isData)
    {
        if (_parsedLine->_isLabel)
            append_to_symbol(_labels, _parsedLine, _dataCounter, _instructionCounter, DATA_ATTRIBUTE);

        append_to_image_table(_imageData, _parsedLine, _dataCounter, _instructionCounter);
    }

    else if (_parsedLine->_isEntry)
        return;

    else if (_parsedLine->_isExtern)
        append_to_symbol(_labels, _parsedLine, _dataCounter, _instructionCounter, EXTERNAL_ATTRIBUTE);

    else /* if (!_parsedLine->_isString && !_parsedLine->_isData) */
    {
        if (_parsedLine->_isLabel)
            append_to_symbol(_labels, _parsedLine, _dataCounter, _instructionCounter, CODE_ATTRIBUTE);

        append_to_image_table(_imageCode, _parsedLine, _dataCounter, _instructionCounter);
    }
}

/* Checks if line has a label for .entry / .extern declerations */
bool is_label_warning(line *_line)
{
    if (_line->_isLabel)
        if (_line->_isExtern || _line->_isEntry)
            return TRUE;

    return FALSE;
}

/* removes label from a given line */
void remove_label_from_line(line *_line)
{
    /* structure of this kind of line before removal is:
        someLabel: .entry/.extern anotherLabel */

    /* remove first word from line */
    strcpy(_line->_lineWords[0], _line->_lineWords[1]);
    strcpy(_line->_lineWords[1], _line->_lineWords[2]);
    memset(_line->_lineWords[2], 0, sizeof(_line->_lineWords[2]));

    /* structure of this kind of line before removal is:
        .entry/.extern anotherLabel */

    /* sets _isLabel flag to FALSE */
    _line->_isLabel = FALSE;
}

void warn_user_about_redundant_label(line *_line)
{
    char _warningDescription[MAX_WARNING_LENGTH];

    if (_line->_isExtern)
    {
        sprintf(_warningDescription, "%sWarning!%s On %sline %d:%s %sIdentified a label with \".extern\" decleration.%s"
                                     "\nThe label was removed from the line and the assembler will ignore it. The label is treated as if it doesn't exist."
                                     "\nUnexpected behaviour may occur during end of first pass / second pass if there are any reference to the label.",
                WARNING_DESCRIPTION_COLOR, STOP_COLOR,
                LINE_COLOR, _line->_number, STOP_COLOR,
                ERROR_DESCRIPTION_COLOR, STOP_COLOR);
    }

    else if (_line->_isEntry)
    {
        sprintf(_warningDescription, "%sWarning!%s On %sline %d:%s %sIdentified a label with \".entry\" decleration.%s"
                                     "\nThe label was removed from the line and the assembler will ignore it. The label is treated as if it doesn't exist."
                                     "\nUnexpected behaviour may occur during end of first pass / second pass if there are any reference to the label.",
                WARNING_DESCRIPTION_COLOR, STOP_COLOR,
                LINE_COLOR, _line->_number, STOP_COLOR,
                ERROR_DESCRIPTION_COLOR, STOP_COLOR);
    }

    printf("\n%s", _warningDescription);
}