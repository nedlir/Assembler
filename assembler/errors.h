#ifndef ERRORS_H
#define ERRORS_H

/* First Pass: */
void report_errors_in_line(line *); /* finds and reports all errors in line */
void report_errors_in_symbol_first_pass(line *_line, head *_labels, size_t _fileLength);
void report_existing_symbol_error(line *, char *, symbol *); /* finds and reports all errors in symbol table */

/* Second Pass: */
bool report_errors_in_symbol_second_pass(head *, head *, line *, size_t); /* finds and reports all errors in symbol table */
bool report_non_existent_symbol(head *, head *, line *, size_t);
bool report_errornous_relative_symbols(head *, head *, line *, size_t);

/* General: */
void report_an_error(line *, char *); /* Initializes a str error in line struct and raises boolean error flag */

/* Line length: */
bool is_valid_line_length(line *);

/* Label: */
bool is_valid_label(line *);
bool is_valid_label_length(char *);
bool is_valid_label_name(char *);
bool is_exist_symbol(char *, symbol *);

/* Comma: */
bool is_valid_commas(line *);
bool is_commas_consecutive(char *);
bool is_commas_between_first_and_second_word(char *);

/* Directives: */
bool is_valid_data(line *);
bool is_valid_data_numbers(line *);
bool is_valid_data_limit(line *);
bool is_valid_string(line *);

/* Commands: */
bool is_valid_operands_addressing(line *);
bool is_valid_mov_addressing(line *);
bool is_valid_cmp_addressing(line *);
bool is_valid_add_addressing(line *);
bool is_valid_sub_addressing(line *);
bool is_valid_lea_addressing(line *);
bool is_valid_clr_addressing(line *);
bool is_valid_not_addressing(line *);
bool is_valid_inc_addressing(line *);
bool is_valid_dec_addressing(line *);
bool is_valid_jmp_addressing(line *);
bool is_valid_bne_addressing(line *);
bool is_valid_jsr_addressing(line *);
bool is_valid_red_addressing(line *);
bool is_valid_prn_addressing(line *);

/* Operands: */
void report_errors_in_operand_values_first_pass(line *, head *, size_t);

bool is_valid_immediate_addressing_values(line *, head *);
bool is_valid_immediate_address_operand_data(char *);

bool is_valid_direct_addressing_values(line *, head *);

/* Extraneous words: */
bool is_extraneous_word(line *);
bool is_extraneous_command_words(line *);
bool is_extraneous_instruction_words(line *);

/* Missing word: */
bool is_missing_word(line *);
bool is_missing_label_word(line *);
bool is_missing_instruction_word(line *);
bool is_missing_command_word(line *);

#endif /* ERRORS_H */