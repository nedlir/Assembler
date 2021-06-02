#include "constants.h"
#include "line_data_structure.h"
#include "binary_data_structure.h"

/* Translates mirrored 'binary word' into int array, each cell in array represent individual symbol (0 or 1)  */
void create_array_of_bits(int *_arrayOfBits, binary *_bitArray)
{
    int bit_0, bit_1;
    int temp;

    temp = _bitArray->destination;
    bit_0 = temp % 2;
    bit_1 = temp / 2;
    _arrayOfBits[11] = bit_0;
    _arrayOfBits[10] = bit_1;

    temp = _bitArray->source;
    bit_0 = temp % 2;
    bit_1 = temp / 2;
    _arrayOfBits[9] = bit_0;
    _arrayOfBits[8] = bit_1;

    temp = _bitArray->funct;
    bit_0 = temp % 2;
    _arrayOfBits[7] = bit_0;

    temp = temp / 2;
    bit_0 = temp % 2;
    _arrayOfBits[6] = bit_0;

    temp = temp / 2;
    bit_0 = temp % 2;
    _arrayOfBits[5] = bit_0;

    temp = temp / 2;
    bit_0 = temp % 2;
    _arrayOfBits[4] = bit_0;

    temp = _bitArray->opcode;
    bit_0 = temp % 2;
    _arrayOfBits[3] = bit_0;

    temp = temp / 2;
    bit_0 = temp % 2;
    _arrayOfBits[2] = bit_0;

    temp = temp / 2;
    bit_0 = temp % 2;
    _arrayOfBits[1] = bit_0;

    temp = temp / 2;
    bit_0 = temp % 2;
    _arrayOfBits[0] = bit_0;
}

/* Converts mirrored array into one decimal digit */
unsigned int convert_array_of_bits_to_decimal(int *_arrayOfBits)
{
    int i;
    int j;
    unsigned int decimalNumber = 0;

    for (i = 11, j = 0; i >= 0; i--)
    {
        decimalNumber = decimalNumber + (pow(2, j) * _arrayOfBits[i]);
        j++;
    }
    return decimalNumber;
}

/* Get first register's name and translate it into 'word' */
unsigned int get_first_register_in_binary(line *_parsedLine)
{
    size_t i;
    char *registerName;
    int result;

    i = 1;

    if (_parsedLine->_isLabel)
        i++;

    registerName = _parsedLine->_lineWords[i];

    result = register_to_int(registerName);

    return (unsigned int)result;
}

/* Get second register's name and translate it into 'word' */
unsigned int get_second_register_in_binary(line *_parsedLine)
{
    size_t i;
    char *registerName;
    unsigned int result;

    i = 2;
    if (_parsedLine->_isLabel)
        i++;

    registerName = _parsedLine->_lineWords[i];

    result = register_to_int(registerName);

    return result;
}

/* Converting register`s name into decimal number, represent position in the "word" */
int register_to_int(char *_str)
{
    if (strcmp(_str, "r0") == 0)
        return 1;
    else if (strcmp(_str, "r1") == 0)
        return 2;
    else if (strcmp(_str, "r2") == 0)
        return 4;
    else if (strcmp(_str, "r3") == 0)
        return 8;
    else if (strcmp(_str, "r4") == 0)
        return 16;
    else if (strcmp(_str, "r5") == 0)
        return 32;
    else if (strcmp(_str, "r6") == 0)
        return 64;
    else if (strcmp(_str, "r7") == 0)
        return 128;

    return -1; /* The function should never get to this point */
}