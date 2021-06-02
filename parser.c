#include "constants.h"
#include "utils.h"
#include "reserved_words.h"
#include "line_data_structure.h"
#include "str_handling.h"
#include "parser.h"

/* parses line from file into the line data structure */
void parse_line(line *_line, int _lineNumber)
{
    char *_firstWord; /* first word of the sentence for both cases where label exists and where it doesn't */
    char *_firstWordPointer = NULL; /* Will be utilized for freeing after strtok */
    char *_secondWord = NULL; /* second word of the sentence for both cases where label exists and where it doesn't */

    /* Initializes line number: */
    _line->_number = _lineNumber + 1; /* unlike computer arrays, human start counting lines from 1 */

    /* Determines if current line is to be ignored (if line is a space or a comment): */
    if (is_comment(_line->_lineStr) || is_str_empty(_line->_lineStr))
    {
        raise_ignore_flag(_line);
        return; /* No need to continue parsing process or check for errors */
    }

    /* Clears all spaces from left and right of the line: */
    remove_spaces_from_str(_line->_lineStr);

    /* Initializes '_firstWord' with first word in the sentence: */
    _firstWord = (char *)malloc(strlen(_line->_lineStr) + 1);
    if (!is_successfull_malloc(_firstWord))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    strcpy(_firstWord, _line->_lineStr);
    _firstWordPointer = _firstWord;
    _firstWord = strtok(_firstWord, " \t\n");

    /* Checks if first word in line is a label: */
    if (is_label(_firstWord))
    {
        raise_label_flag(_line);

        /* Reaches first word after the label: */
        _firstWord = strtok(NULL, " \t\n"); /* '\n' sign is for 'stop' / 'rts' command after label: "SOMELABEL: stop / rts'\n'"*/
    }

    raise_first_word_flags(_line, _firstWord);

    /* Tokenizes words in the line into an array of words: */
    parse_words(_line);

    if (_line->_isCommand)
        raise_operand_flags(_line);

    /* Checks if second word in line is unknown word: */
    _secondWord = _line->_lineWords[1];
    if (_line->_isLabel)
        if (!is_reserved_word(_secondWord))
            _line->_isUnknownWord = TRUE;

    free(_firstWordPointer);
}

/* Separates words in line into array of words: */
void parse_words(line *_line)
{
    size_t i; /* Counter to traverse _lineWords array */
    char *_word;
    char *_wordPointer; /* a variable to freeing _word after strtok */

    /* Initializes '_word' with first word in the sentence: */
    _word = (char *)malloc(strlen(_line->_lineStr) + 1);
    if (!is_successfull_malloc(_word))
        exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);

    strcpy(_word, _line->_lineStr);
    _wordPointer = _word;
    _word = strtok(_word, " \t\n");

    i = 0;
    strcpy(_line->_lineWords[i], _word);

    if (_line->_isLabel) /* Tokenizes next word after label:  */
    {
        _line->_lineWords[i][strlen(_line->_lineWords[i]) - 1] = '\0'; /* removes ":" sign*/
        i++;
        _word = strtok(NULL, " \t\n"); /* '\n' sign is for 'stop' / 'rts' command after label: "SOMELABEL: stop / rts'\n'"*/
        strcpy(_line->_lineWords[i], _word);
        remove_spaces_from_str(_line->_lineWords[i]);
    }

    do /* Inserts data values seperated by ',' into '_lineWords' array: */
    {
        strcpy(_line->_lineWords[i], _word);
        remove_spaces_from_str(_line->_lineWords[i]);
        _word = strtok(NULL, ",");
        i++;
    } while (_word != NULL && i < MAX_WORDS_IN_LINE);

    free(_wordPointer);
}