#ifndef FIRST_ANALYZE_H
#define FIRST_ANALYZE_H

void first_analyze(line *, head *, head *, head *, int *, int *);
bool is_label_warning(line *);
void remove_label_from_line(line *);
void warn_user_about_redundant_label(line *);

#endif /* FIRST_ANALYZE_H */