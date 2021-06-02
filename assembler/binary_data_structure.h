#ifndef BINARY_DATA_STRUCTURE_H
#define BINARY_DATA_STRUCTURE_H * /

typedef struct
{
    unsigned int destination : 2;
    unsigned int source : 2;
    unsigned int funct : 4;
    unsigned int opcode : 4;
    unsigned int : 0;
    unsigned int word_for_asci : MACHINE_CODE_WORD_LENGTH;
} binary;

/* Binary Functions: */

void create_array_of_bits(int *, binary *);
unsigned int convert_array_of_bits_to_decimal(int *);
unsigned int get_first_register_in_binary(line *);
unsigned int get_second_register_in_binary(line *);
int register_to_int(char *);
#endif /* BINARY_DATA_STRUCTURE_H */