#ifndef RESERVED_WORDS_H
#define RESERVED_WORDS_H

/* Reserved words: */
extern const char *COMMANDS_LIST[];
extern unsigned int COMMANDS_LIST_LENGTH;

extern const char *RESGISTERS_LIST[];
extern unsigned int RESGISTERS_LIST_LENGTH;

/* Operands (basically, commands list divided): */
extern const char *TWO_OPERAND_LIST[];
extern unsigned int TWO_OPERAND_LIST_LENGTH;

extern const char *SINGLE_OPERAND_LIST[];
extern unsigned int SINGLE_OPERAND_LIST_LENGTH;

extern const char *ZERO_OPERAND_LIST[];
extern unsigned int ZERO_OPERAND_LIST_LENGTH;

/* Reserved words functions: */
bool is_reserved_word(char *);
bool is_command(char *);
bool is_register(char *);
bool is_instruction(char *);

/* Operand functions: */
bool is_two_operand(char *);
bool is_single_operand(char *);
bool is_zero_operand(char *);

#endif /* RESERVED_WORDS_H */