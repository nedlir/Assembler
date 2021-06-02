#include "constants.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "binary_data_structure.h"
#include "assembler_helpers.h"

bool is_valid_arguments_number(int argumentsAmount)
{
    if (argumentsAmount < 2)
        return FALSE;
    else
        return TRUE;
}

void free_all(line *parsedLinesArray, head *labelLinkedList, head *imageDataLinkedList, head *imageCodeLinkedList)
{
    if (parsedLinesArray != NULL)
        free_parsed_lines_array(parsedLinesArray);

    if (labelLinkedList != NULL)
        free_label_linked_list(labelLinkedList);

    if (imageDataLinkedList != NULL)
        free_image_data_linked_list(imageDataLinkedList);

    if (imageCodeLinkedList != NULL)
        free_image_code_linked_list(imageCodeLinkedList);
}

void free_parsed_lines_array(line *_parsedLinesArray)
{
    free(_parsedLinesArray);
}

void free_label_linked_list(head *_labelLinkedList)
{
    symbol *temp = _labelLinkedList->_headOfSymbolTable;
    symbol *next;

    while (temp != NULL)
    {
        next = temp->_next;
        free(temp);
        temp = next;
    }

    free(_labelLinkedList);
}

void free_image_data_linked_list(head *_imageDataLinkedList)
{
    image *temp = _imageDataLinkedList->_headOfImageData;
    image *next;

    while (temp != NULL)
    {
        next = temp->_next;
        free(temp);
        temp = next;
    }

    free(_imageDataLinkedList);
}

void free_image_code_linked_list(head *_imageCodeLinkedList)
{
    image *temp = _imageCodeLinkedList->_headOfImageCode;
    image *next;

    while (temp != NULL)
    {
        next = temp->_next;
        free(temp);
        temp = next;
    }

    free(_imageCodeLinkedList);
}
