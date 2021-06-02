#include "constants.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "binary_data_structure.h"
#include "errors.h"
#include "output.h"
#include "second_pass.h"

/* Second pass function will perform second pass of the assembler. If no errors are found a TRUE value will be returned */
bool second_pass(line *_lines, head *_labels, head *_imageData, head *_imageCode, size_t _fileLength)
{
    bool _isErrorsFound;

    _isErrorsFound = FALSE;

    if (!report_errors_in_symbol_second_pass(_labels, _imageCode, _lines, _fileLength))
        _isErrorsFound = TRUE;

    if (_isErrorsFound)
        return FALSE;

    else
    {
        second_analyze(_lines, _labels, _imageCode, _fileLength);
        return TRUE;
    }
}

void second_analyze(line *_lines, head *_labels, head *_imageCode, size_t _fileLength)
{
    update_image_address(_labels, _imageCode);
    raise_entry_flag_in_symbol(_lines, _labels, _fileLength);
}

void update_image_address(head *_labels, head *_imageCode)
{
    image *_imageTraversalNode = _imageCode->_headOfImageCode;

    while (_imageTraversalNode != NULL) /* Traverse the image code list */
    {
        if (strcmp(_imageTraversalNode->_referencedSymbolName, NO_SYMBOL) == 0)
        {
            _imageTraversalNode = _imageTraversalNode->_next;
            continue;
        }
        else
        {
            symbol *_symbol;
            _symbol = get_symbol(_imageTraversalNode->_referencedSymbolName, _labels);

            if (_imageTraversalNode->_isFirstOperandRelativeAddress)
                _imageTraversalNode->_binary = (_symbol->_value - _imageTraversalNode->_value);

            else
            {
                _imageTraversalNode->_binary = _symbol->_value;

                if (_symbol->_value == 0) /* If attribute is external */
                    _imageTraversalNode->_attribute = 'E';

                else /* Attribute is relocatable */
                    _imageTraversalNode->_attribute = 'R';
            }
        }
        _imageTraversalNode = _imageTraversalNode->_next;
    }
}

void raise_entry_flag_in_symbol(line *_lines, head *_labels, size_t _fileLength)
{
    size_t _lineNumber;
    size_t i; /* location of label */
    char *_symbolName;
    symbol *_symbol;

    _lineNumber = 0;
    while (_lineNumber < _fileLength)
    {
        i = 1;
        if (_lines[_lineNumber]._isLabel)
            i++;
            
        if (_lines[_lineNumber]._isEntry)
        {

            _symbolName = _lines[_lineNumber]._lineWords[i];
            _symbol = get_symbol(_symbolName, _labels);
            _symbol->_isEntry = TRUE;
        }
        _lineNumber++;
    }
}
