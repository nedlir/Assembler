#ifndef SECOND_PASS_H
#define SECOND_PASS_H

bool second_pass(line *, head *, head *, head *, size_t);
void second_analyze(line *, head *, head *, size_t);
void update_image_address(head *, head *);
void raise_entry_flag_in_symbol(line *, head *, size_t);

#endif /* SECOND_PASS_H */