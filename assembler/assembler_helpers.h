#ifndef ASSEMBLER_HELPERS_H
#define ASSEMBLER_HELPERS_H

bool is_valid_arguments_number(int); /* Checks number of arguments and validity */

void free_parsed_lines_array(line *);
void free_label_linked_list(head *);
void free_image_data_linked_list(head *);
void free_image_code_linked_list(head *);
void free_all(line *, head *, head *, head *);



#endif /* ASSEMBLER_HELPERS_H */