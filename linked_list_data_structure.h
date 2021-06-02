#ifndef LINKED_LIST_DATA_STRUCTURE_H
#define LINKED_LIST_DATA_STRUCTURE_H

/* Symbol table: */
typedef struct Linked_Symbol_Node
{
    char _symbolName[MAX_LABEL_LENGTH];
    char _attribute[MAX_ATTRIBUTE_LENGTH]; /*code-data-entry-external*/
    int _value;
    struct Linked_Symbol_Node *_next;
    bool _isEntry;
    bool _isExtern;
    bool _isData;
} symbol;

typedef struct Linked_Image_Node
{
    /* General Image Variables: */
    int _value;
    char _attribute; /* 'A' / 'E' / 'R' */
    unsigned int _binary;
    struct Linked_Image_Node *_next;

    /* Image Code Only Variables: */
    /* This variables are needed to retrieve the missing address during second pass */
    bool _isSymbol;                              /* if symbol is referenced */
    char _referencedSymbolName[MAX_WORD_LENGTH]; /* name of the referenced Symbol */
    bool _isError;                               /* if an error is found */
    char _errorDescription[MAX_ERROR_LENGTH];    /* if '_isError', a description will be here */
    size_t _lineNumber;                          /* line number */
    bool _isFirstOperandRelativeAddress;         /* Method 2 */

} image;

/* Pointer to head of different list types */
typedef struct head_linked_list
{
    symbol *_headOfSymbolTable;
    image *_headOfImageCode;
    image *_headOfImageData;
    int _counter;
} head;

/* Linked List Functions: */
void init_head(head *);
void init_symbol(symbol *);
void init_image(image *);

bool is_empty_symbol_linked_list(head *);
bool is_empty_image_code(head *);
bool is_empty_image_data(head *);

void append_to_symbol(head *, line *, int *, int *, char *);
void append_first_symbol_node(head *, char *, int *, int *, char *, symbol *);
void append_symbol_node(head *, char *, int *, int *, char *, symbol *);
symbol *get_symbol(char *, head *);

void append_to_image_code(head *, int *, char, unsigned int, char *, size_t);
void append_first_image_code_node(head *, int *, char, unsigned int, image *, char *, size_t);
void append_image_code_node(head *, int *, char, unsigned int, image *, char *, size_t);

void append_to_image_data(head *, int *, char, unsigned int);
void append_first_image_data_node(head *, int *, char, unsigned int, image *);
void append_image_data_node(head *, int *, char, unsigned int, image *);

void append_to_image_table(head *, line *, int *, int *);

void convert_operator(char *, int *, int *);
void convert_to_destination(line *, int *);
void convert_to_source(line *, int *);

void bit_array_organizer(line *, int *);
void append_data_to_image_data(line *, head *, int *, int *);
void append_string_to_image_data(line *, head *, int *, int *);


void append_to_image_code_immidiate_address(size_t , line *, head *, int *);
void append_to_image_code_direct_address(size_t, line *, head *, int *);
void append_to_image_code_register_address(line *, head *, int *, bool);
void append_to_image_code_relative_address(size_t, line *, head *, int *);

#endif /* LINKED_LIST_DATA_STRUCTURE_H */