#ifndef LINE_DATA_STRUCTURE_H
#define LINE_DATA_STRUCTURE_H

typedef struct Single_Line_From_File
{
    /* Line Data:  */
    size_t _number;                                          /* line number */
    char _lineStr[MAX_LINE_LENGTH + EXTRANEOUS_LINE_BUFFER]; /* str representation of the line  */
    char _lineWords[MAX_WORDS_IN_LINE][MAX_WORD_LENGTH];     /* line's seperated words  */
    char _errorDescription[MAX_ERROR_LENGTH];                /* if '_isError', a description will be here */

    /* Flags: */
    bool _isLabel;

    /* Instruction Flags:*/
    bool _isData;   /* isInstruction */
    bool _isString; /* isInstruction */
    bool _isExtern; /* isInstruction */
    bool _isEntry;  /* isInstruction */

    /* Command Flags: */
    bool _isCommand;
    bool _isFirstOperandImmediateAddress; /* Method 0 */
    bool _isFirstOperandDirectAddress;    /* Method 1 */
    bool _isFirstOperandRelativeAddress;  /* Method 2 */
    bool _isFirstOperandRegisterDirect;   /* Method 3 */

    bool _isSecondOperandImmediateAddress;               /* Method 0 */
    bool _isSecondOperandDirectAddress;                  /* Method 1 */
    bool _isSecondOperandRelativeAddress; /* Method 2 */ /* This method is not used by the program for implementation but only for error detection */
    bool _isSecondOperandRegisterDirect;                 /* Method 3 */

    /* Reference Flags: */
    bool _isUnknownWord; /* if an unknown word is found within the input */
    bool _isError;       /* if an error is found */
    bool _isIgnore;      /* If the line is comment or if the line is empty */

} line;

/* Line Functions: */

/* Boolean flag raisers: */
void raise_label_flag(line *);
void raise_first_word_flags(line *, char *);
void raise_instruction_flag(line *, char *);
void raise_command_flag(line *);
void raise_unknown_word_flag(line *);
void raise_ignore_flag(line *);
void raise_operand_flags(line *);

/* Boolean helpers for flag raisers:*/
bool is_label(char *);
bool is_comment(char *);
bool is_relative_address(char *);
bool is_immediate_address(char *);

/* General Helpers: */
void initialize_line_structure(line *);

/* Boolean helpers for external functions: */
bool is_exist_first_operand(line *);
bool is_exist_second_operand(line *);

#endif /* LINE_DATA_STRUCTURE_H */