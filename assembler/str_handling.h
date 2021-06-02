#ifndef STR_HANDLING_H
#define STR_HANDLING_H

/* str handling: */
bool is_str_starts_with(char *, char);
bool is_str_ends_with(char *, char);
bool is_str_number(char *);
bool is_str_printable(char *);
bool is_str_empty(char *);
void remove_space_from_left_str(char *);
void remove_space_from_right_str(char *);
void remove_spaces_from_str(char *); /* Removes spaces from both left and right sides */

#endif /* STR_HANDLING_H */