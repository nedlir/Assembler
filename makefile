FLAGS = gcc -c -Wall -ansi -pedantic

ASSEMBLER_FLAGS = gcc -g -Wall -ansi -pedantic

ASSEMBLER_OBJECTS = assembler.o \
assembler_helpers.o first_pass.o second_pass.o output.o \
parser.o errors.o first_analyze.o \
line_data_structure.o linked_list_data_structure.o binary_data_structure.o \
str_handling.o reserved_words.o utils.o \

HEADERS = assembler_helpers.h first_pass.h second_pass.h output.h \
parser.h errors.h first_analyze.h \
line_data_structure.h linked_list_data_structure.h binary_data_structure.h \
constants.h str_handling.h reserved_words.h utils.h 


# Assembler:
assembler: $(ASSEMBLER_OBJECTS)
	$(ASSEMBLER_FLAGS) $(ASSEMBLER_OBJECTS) -lm -o assembler

# Assembler Functions:
assembler.o: assembler.c $(HEADERS)
	$(FLAGS) assembler.c -o assembler.o

first_pass.o: first_pass.c $(HEADERS)
	$(FLAGS) first_pass.c -o first_pass.o
	
second_pass.o: second_pass.c $(HEADERS)
	$(FLAGS) second_pass.c -o second_pass.o

output.o: output.c $(HEADERS)
	$(FLAGS) output.c -lm -o output.o

# Assembler Function Helpers:
assembler_helpers.o: assembler_helpers.c $(HEADERS)
	$(FLAGS) assembler_helpers.c -o assembler_helpers.o

parser.o: parser.c $(HEADERS)
	$(FLAGS) parser.c -o parser.o

errors.o: errors.c $(HEADERS)
	$(FLAGS) errors.c -o errors.o

first_analyze.o: first_analyze.c $(HEADERS)
	$(FLAGS) first_analyze.c -lm -o first_analyze.o

# Data Structures:
line_data_structure.o: line_data_structure.c $(HEADERS)
	$(FLAGS) line_data_structure.c -o line_data_structure.o

linked_list_data_structure.o: $(HEADERS)
	$(FLAGS) linked_list_data_structure.c -o linked_list_data_structure.o

binary_data_structure.o: $(HEADERS)
	$(FLAGS) binary_data_structure.c -o binary_data_structure.o

# General Helpers and Utilities:
str_handling.o: str_handling.c $(HEADERS)
	$(FLAGS) str_handling.c -o str_handling.o

reserved_words.o: reserved_words.c $(HEADERS)
	$(FLAGS) reserved_words.c -o reserved_words.o

utils.o: utils.c $(HEADERS)
	$(FLAGS) utils.c -o utils.o

# Remove ".o" files:
clean:
	rm *.o

# Remove ".ent", ".ext", ".ob" files:
ro:
	rm *.ent
	rm *.ext
	rm *.ob