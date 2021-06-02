#ifndef UTILS_H
#define UTILS_H

/* Memory allocation: */
bool is_successfull_malloc(void *);

/* File Handling: */
bool is_successfull_fopen(FILE *);
bool is_successfull_fclose(int);
size_t get_file_length(char *);
FILE *file_open(char *, char *);

/* General: */
void exit_program(char *, int);

#endif /* UTILS_H */