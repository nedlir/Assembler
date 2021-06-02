#ifndef PARSER_H
#define PARSER_H

/* Parser function: */
void parse_line(line *, int); /* Translates line into struct with data */

/* Helper: */
void parse_words(line *); /* Translates line into words with data */

#endif /* PARSER_H */