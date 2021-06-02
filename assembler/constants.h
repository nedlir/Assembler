#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Boolean Values: */
typedef enum
{
    FALSE,
    TRUE
} bool;

/* Lengths: */
#define MAX_LINE_LENGTH 80 /* Max line length*/
#define EXTRANEOUS_LINE_BUFFER 100 /* Any additional characters in this buffer are erroneous, helps prevent stack smashing to an extent */
#define MAX_WORDS_IN_LINE (MAX_LINE_LENGTH / 2) /* Since we are using commas as seperators it is unlikely to have more than (MAX_LINE_LENGTH / 2) words given a correct input */
#define MAX_LABEL_LENGTH 31
#define MAX_INSTRUCTION_LENGTH 8 /* ".string" */
#define MAX_WORD_LENGTH (MAX_LINE_LENGTH) /* The longest word in the line could be a word that doesn't exist and takes the whole line's length  */
#define MAX_ERROR_LENGTH 300 /* Error description's length */
#define MAX_WARNING_LENGTH 500 /* Warning description's length */
#define MAX_FILE_NAME_LENGTH 255 /* In most modern OS, this is the max filename length allowed */
#define MAX_ATTRIBUTE_LENGTH 20
#define MACHINE_CODE_WORD_LENGTH 12
#define MAX_MACHINE_INT ((pow(2, (MACHINE_CODE_WORD_LENGTH - 1))) - 1)
#define MIN_MACHINE_INT (-pow(2, (MACHINE_CODE_WORD_LENGTH - 1)))

/* Filetypes Suffix: */
#define INPUT_FILETYPE "as"
#define OBJECT_FILETYPE "ob"
#define ENTRIES_FILETYPE "ent"
#define EXTERNALS_FILETYPE "ext"

/* Addresses: */
#define FIRST_MEMORY_ADDRESS 100

/* Symbols */
#define NO_SYMBOL "00_EMPTY_SYMBOL_00"
#define ABSOLUTE 'A'
#define RELOCATABLE 'R'
#define EXTERNAL 'E'

/* Labels Attributes */
#define DATA_ATTRIBUTE "data"
#define CODE_ATTRIBUTE "code"
#define EXTERNAL_ATTRIBUTE "external"

/* Program errors: */
#define MEMORY_ALLOCATION_ERROR "Memory allocation failure."
#define FILE_OPEN_ERROR "File opening failure."
#define FILE_CLOSE_ERROR "File closing failure."
#define EXIT_MESSAGE "The program will now exit."
#define FILE_REMOVE_ERROR "Unable to delete the file."
#define NO_ERROR_CODE 0

/* Colors: */
#ifdef __unix__ /* Unix based OS */
#define FILE_COLOR "\x1b[33m"
#define ERRORS_COLOR "\x1b[31m"
#define ERROR_DESCRIPTION_COLOR "\x1b[35m"
#define WARNING_DESCRIPTION_COLOR "\x1b[36m"
#define LINE_COLOR "\x1b[34m"
#define SUCCESS_COLOR "\x1b[36m"
#define STOP_COLOR "\x1b[0m"

#elif _WIN32 /* Windows 32 bit + Windows 64 bit */
#define FILE_COLOR "\033[0;33m"
#define ERRORS_COLOR "\033[0;31m"
#define ERROR_DESCRIPTION_COLOR "\033[0;35m"
#define WARNING_DESCRIPTION_COLOR "\033[0;36m"
#define LINE_COLOR "\033[0;34m"
#define SUCCESS_COLOR "\033[0;32m"
#define STOP_COLOR "\033[0m"

#else /* Other OS - no colors */
#define FILE_COLOR ""
#define ERRORS_COLOR ""
#define ERROR_DESCRIPTION_COLOR ""
#define WARNING_DESCRIPTION_COLOR ""
#define LINE_COLOR ""
#define SUCCESS_COLOR ""
#define STOP_COLOR ""

#endif

#endif /* CONSTANTS_H */