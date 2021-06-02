#ifndef FIRST_PASS_H
#define FIRST_PASS_H

bool first_pass(FILE *, line *, head *, head *, head *, int *, int *, size_t);
bool print_first_pass_errors(line *, size_t);
void update_data_symbol_values(head *, int *);

#endif /* FIRST_PASS_H */