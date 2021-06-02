#include "constants.h"
#include "str_handling.h"

bool is_str_starts_with(char *_str, char _ch)
{
    if (_str[0] == _ch)
        return TRUE;

    return FALSE;
}

bool is_str_ends_with(char *_str, char _ch)
{
    size_t i; /* counter to traverse the string from ending */

    /*  Traverse from end of string until you reach a non space character */
    for (i = strlen(_str) - 1; isspace(_str[i]); i--)
        ;

    if (_str[i] == _ch)
        return TRUE;

    return FALSE;
}

bool is_str_empty(char *_str)
{
    size_t i;

    if (_str[0] == '\n')
        return TRUE;

    if (_str[0] == '\0')
        return TRUE;

    i = 0;
    while (_str[i] != '\0')
    {
        if (!isspace(_str[i]))
            return FALSE;
        i++;
    }
    return TRUE;
}

bool is_str_number(char *_str)
{
    size_t i; /* serves as counter for _str chars */

    i = 0;

    if (is_str_starts_with(_str, '-') || is_str_starts_with(_str, '+'))
        i++;

    for (i = i + 0; i < strlen(_str); i++)
        if (!isdigit(_str[i]))
            return FALSE;

    return TRUE;
}


/* Deletes all spaces from left of the string until it reaches a char that is not space */
void remove_space_from_left_str(char *_str)
{
    size_t i;                   /* counter to traverse the string */
    size_t current_space_index; /* Counter to traverse spaces until first word of str */

    current_space_index = 0;
    /* Reaches the last space: */
    while (isspace(_str[current_space_index]))
        current_space_index++;

    if (current_space_index != 0) /* if found any spaces */
    {
        /* Moves all chars to the left of the string: */
        i = 0;
        while (_str[i + current_space_index] != '\0')
        {
            _str[i] = _str[i + current_space_index];
            i++;
        }
        _str[i] = '\0';
    }
}

/* Deletes all spaces from right of the string until it reaches a char that is not space */
void remove_space_from_right_str(char *_str)
{
    size_t i; /* counter to traverse the string from ending */

    i = strlen(_str) - 1;
    while ((isspace(_str[i]))) /*  Traverse from end of string until you reach a non space character */
        i--;
    _str[i + 1] = '\0';
}

void remove_spaces_from_str(char *_str)
{
    remove_space_from_left_str(_str);
    remove_space_from_right_str(_str);
}
