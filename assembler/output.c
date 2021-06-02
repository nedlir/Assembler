#include "constants.h"
#include "utils.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "output.h"

void create_output(char *_fileName, head *_imageData, head *_imageCode, head *_labelList)
{
    output_object(_fileName, _imageData, _imageCode);
    output_entry(_fileName, _labelList);
    output_extern(_fileName, _imageCode, _labelList);
}

void output_object(char *_fileName, head *_pointerToHeadData, head *_pointerToHeadCode)
{
    int hexaTrimmer = 0xfff;                                                    /* We will perform a bitwise with ("&") to limit the output to 3 hexa digits */
    if (_pointerToHeadCode->_counter == 0 && _pointerToHeadData->_counter == 0) /* If image table is empty, file does not created */
        return;
    else
    {
        image *_traversalNodeData = _pointerToHeadData->_headOfImageData;
        image *_traversalNodeCode = _pointerToHeadCode->_headOfImageCode;

        FILE *filePointer;
        filePointer = file_open(_fileName, OBJECT_FILETYPE);
        if (!is_successfull_fopen(filePointer))
            exit_program(FILE_OPEN_ERROR, EXIT_FAILURE);

        fprintf(filePointer, "\t%d %d\n", _pointerToHeadCode->_counter, _pointerToHeadData->_counter);

        while (_traversalNodeCode != NULL)
        {
            fprintf(filePointer, "%04d %03X %c\n",
                    _traversalNodeCode->_value,
                    _traversalNodeCode->_binary & hexaTrimmer,
                    _traversalNodeCode->_attribute);

            _traversalNodeCode = _traversalNodeCode->_next;
        }

        while (_traversalNodeData != NULL)
        {
            fprintf(filePointer, "%04d %03X %c\n",
                    _traversalNodeData->_value + FIRST_MEMORY_ADDRESS + _pointerToHeadCode->_counter,
                    _traversalNodeData->_binary & hexaTrimmer,
                    _traversalNodeData->_attribute);

            _traversalNodeData = _traversalNodeData->_next;
        }

        if (!is_successfull_fclose(fclose(filePointer)))
            exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);
    }
}

void output_entry(char *_fileName, head *_pointerToHead)
{
    if (_pointerToHead->_headOfSymbolTable == NULL) /* If no symbols exist, file does not created */
        return;
    else
    {
        symbol *_traversalNode = _pointerToHead->_headOfSymbolTable;
        FILE *filePointer;
        char workingfile[MAX_FILE_NAME_LENGTH];

        filePointer = file_open(_fileName, ENTRIES_FILETYPE);

        if (!is_successfull_fopen(filePointer))
            exit_program(FILE_OPEN_ERROR, EXIT_FAILURE);

        while (_traversalNode != NULL)
        {
            if (_traversalNode->_isEntry)
                fprintf(filePointer, "%s %04d\n", _traversalNode->_symbolName, _traversalNode->_value);

            _traversalNode = _traversalNode->_next;
        }

        fseek(filePointer, 0, SEEK_END); /* goto end of file */
        if (ftell(filePointer) == 0)     /* if file empty */
        {
            sprintf(workingfile, "%s.%s", _fileName, ENTRIES_FILETYPE); /* builds the filename for the fopen */
            if (!is_successfull_fclose(fclose(filePointer)))
                exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);
            if (remove(workingfile) != 0)
                exit_program(FILE_REMOVE_ERROR, EXIT_FAILURE);
        }
        else
        {
            if (!is_successfull_fclose(fclose(filePointer)))
                exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);
        }
    }
}

void output_extern(char *_fileName, head *_pointerToHeadCode, head *_pointerToHeadLabel)
{
    if (_pointerToHeadLabel->_headOfSymbolTable == NULL) /* If no extern exist, file does not created */
        return;
    else
    {
        image *_traversalNode = _pointerToHeadCode->_headOfImageCode;
        FILE *filePointer;
        char workingfile[MAX_FILE_NAME_LENGTH];

        filePointer = file_open(_fileName, EXTERNALS_FILETYPE);

        if (!is_successfull_fopen(filePointer))
            exit_program(FILE_OPEN_ERROR, EXIT_FAILURE);

        while (_traversalNode != NULL)
        {
            if (_traversalNode->_attribute == EXTERNAL)
                fprintf(filePointer, "%s %04d\n", _traversalNode->_referencedSymbolName, _traversalNode->_value);

            _traversalNode = _traversalNode->_next;
        }

        fseek(filePointer, 0, SEEK_END); /* goto end of file */
        if (ftell(filePointer) == 0)     /* if file empty */
        {
            sprintf(workingfile, "%s.%s", _fileName, EXTERNALS_FILETYPE); /* builds the filename for the fopen */
            if (!is_successfull_fclose(fclose(filePointer)))
                exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);
            if (remove(workingfile) != 0)
                exit_program(FILE_REMOVE_ERROR, EXIT_FAILURE);
        }
        else
        {
            if (!is_successfull_fclose(fclose(filePointer)))
                exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);
        }
    }
}