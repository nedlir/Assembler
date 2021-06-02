#include "constants.h"
#include "reserved_words.h"

/* Reserved Words lists: */
const char *RESGISTERS_LIST[] =
    {
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"};
unsigned int RESGISTERS_LIST_LENGTH = sizeof(RESGISTERS_LIST) / sizeof(RESGISTERS_LIST[0]);
/* Operands lists (commands list): */

const char *TWO_OPERAND_LIST[] = /* Commands list part 1 */
    {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea"};
unsigned int TWO_OPERAND_LIST_LENGTH = sizeof(TWO_OPERAND_LIST) / sizeof(TWO_OPERAND_LIST[0]);

const char *SINGLE_OPERAND_LIST[] = /* Commands list part 2 */
    {
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "red",
        "prn"};
unsigned int SINGLE_OPERAND_LIST_LENGTH = sizeof(SINGLE_OPERAND_LIST) / sizeof(SINGLE_OPERAND_LIST[0]);

const char *ZERO_OPERAND_LIST[] = /* Commands list part 3 */
    {
        "rts",
        "stop"};
unsigned int ZERO_OPERAND_LIST_LENGTH = sizeof(ZERO_OPERAND_LIST) / sizeof(ZERO_OPERAND_LIST[0]);

const char *INSTRUCTIONS_LIST[] =
    {
        ".data",
        ".string",
        ".entry",
        ".extern"};
unsigned int INSTRUCTIONS_LIST_LENGTH = sizeof(INSTRUCTIONS_LIST) / sizeof(INSTRUCTIONS_LIST[0]);

/* Reserved Words functions: */
bool is_command(char *_str)
{

    if (is_two_operand(_str) || is_single_operand(_str) || is_zero_operand(_str))
        return TRUE;

    return FALSE;
}

bool is_register(char *_str)
{
    size_t i;

    for (i = 0; i < RESGISTERS_LIST_LENGTH && *RESGISTERS_LIST[i] != '\0'; i++)
        if (strcmp(RESGISTERS_LIST[i], _str) == 0)
            return TRUE;

    return FALSE;
}

bool is_instruction(char *_str)
{
    size_t i;

    for (i = 0; i < INSTRUCTIONS_LIST_LENGTH && *INSTRUCTIONS_LIST[i] != '\0'; i++)
        if (strcmp(INSTRUCTIONS_LIST[i], _str) == 0)
            return TRUE;

    return FALSE;
}

/* Checks if a given string is a saved word: */
bool is_reserved_word(char *_str)
{
    if (is_command(_str) || is_register(_str) || is_instruction(_str))
        return TRUE;

    return FALSE;
}

/* Operands functions: */
bool is_two_operand(char *_str)
{
    size_t i;

    for (i = 0; i < TWO_OPERAND_LIST_LENGTH && *TWO_OPERAND_LIST[i] != '\0'; i++)
        if (strcmp(TWO_OPERAND_LIST[i], _str) == 0)
            return TRUE;

    return FALSE;
}

bool is_single_operand(char *_str)
{
    size_t i;

    for (i = 0; i < SINGLE_OPERAND_LIST_LENGTH && *SINGLE_OPERAND_LIST[i] != '\0'; i++)
        if (strcmp(SINGLE_OPERAND_LIST[i], _str) == 0)
            return TRUE;

    return FALSE;
}

bool is_zero_operand(char *_str)
{
    size_t i;

    for (i = 0; i < ZERO_OPERAND_LIST_LENGTH && *ZERO_OPERAND_LIST[i] != '\0'; i++)
        if (strcmp(ZERO_OPERAND_LIST[i], _str) == 0)
            return TRUE;

    return FALSE;
}