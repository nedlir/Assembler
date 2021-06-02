#include "constants.h"
#include "utils.h"
#include "line_data_structure.h"
#include "linked_list_data_structure.h"
#include "binary_data_structure.h"
#include "assembler_helpers.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output.h"

int main(int argc, char *argv[])
{
    size_t i; /* Counter to traverse the input arguments */

    if (is_valid_arguments_number(argc))
    {
        for (i = 1; i < argc; i++)
        {
            /* Variables, pointers and data structures decleration and initialization: */
            size_t fileLength; /* number of rows in file */
            FILE *filePointer;
            char *fileName = argv[i];

            int instructionCounter = FIRST_MEMORY_ADDRESS;
            int dataCounter = 0;

            line *parsedLinesArray = NULL;
            head *labelLinkedList = NULL;
            head *imageDataLinkedList = NULL;
            head *imageCodeLinkedList = NULL;

            filePointer = file_open(fileName, INPUT_FILETYPE);
            if (!is_successfull_fopen(filePointer) || strlen(fileName) > MAX_FILE_NAME_LENGTH)
            {
                printf("\n%s %s%s.%s%s %scould not be opened%s. ", FILE_OPEN_ERROR, FILE_COLOR, fileName, INPUT_FILETYPE, STOP_COLOR, ERRORS_COLOR, STOP_COLOR);
                printf("Continuing to next file...");
                free_all(parsedLinesArray, labelLinkedList, imageDataLinkedList, imageCodeLinkedList);
                continue;
            }
            fileLength = get_file_length(fileName);

            parsedLinesArray = (line *)malloc((fileLength + 1) * sizeof(line));
            labelLinkedList = (head *)malloc(sizeof(head));
            imageDataLinkedList = (head *)malloc(sizeof(head));
            imageCodeLinkedList = (head *)malloc(sizeof(head));

            if (!is_successfull_malloc(imageCodeLinkedList) || !is_successfull_malloc(imageDataLinkedList) ||
                !is_successfull_malloc(imageDataLinkedList) || !is_successfull_malloc(parsedLinesArray))
            {
                free_all(parsedLinesArray, labelLinkedList, imageDataLinkedList, imageCodeLinkedList);
                exit_program(MEMORY_ALLOCATION_ERROR, EXIT_FAILURE);
            }

            init_head(labelLinkedList);
            init_head(imageDataLinkedList);
            init_head(imageCodeLinkedList);

            /* Assembler Algorithm: */
            printf("\nAssembler will start proccessing %s%s.%s%s.", FILE_COLOR, fileName, INPUT_FILETYPE, STOP_COLOR);
            printf("\nStarting first pass...");
            if (!first_pass(filePointer, parsedLinesArray, labelLinkedList, imageDataLinkedList,
                            imageCodeLinkedList, &instructionCounter, &dataCounter, fileLength))
            {
                printf("\n%sErrors found during first pass.%s ", ERRORS_COLOR, STOP_COLOR);

                free_all(parsedLinesArray, labelLinkedList, imageDataLinkedList, imageCodeLinkedList);

                if (!is_successfull_fclose(fclose(filePointer)))
                    exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);

                printf("Continuing to next file...");
                continue;
            }

            printf("\nFinished first pass without any errors!");
            printf("\nStarting second pass...");

            if (!second_pass(parsedLinesArray, labelLinkedList, imageDataLinkedList, imageCodeLinkedList, fileLength))
            {
                printf("\n%sErrors found during second pass.%s ", ERRORS_COLOR, STOP_COLOR);

                free_all(parsedLinesArray, labelLinkedList, imageDataLinkedList, imageCodeLinkedList);

                if (!is_successfull_fclose(fclose(filePointer)))
                    exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);

                printf("Continuing to next file...");
                continue;
            }
            printf("\nFinished second pass without any errors!");
            printf("\nCreating output...");

            create_output(fileName, imageDataLinkedList, imageCodeLinkedList, labelLinkedList);

            free_all(parsedLinesArray, labelLinkedList, imageDataLinkedList, imageCodeLinkedList);

            printf("\n%sOutput files are ready!%s ", SUCCESS_COLOR, STOP_COLOR);

            if (!is_successfull_fclose(fclose(filePointer)))
                exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);

            printf("Continuing to next file...");
            continue;
        }
    }
    else
        printf("%sInvalid number of arguments.%s", ERRORS_COLOR, STOP_COLOR);

    printf("\nNo more input files to process. %sGreat success!%s\n", SUCCESS_COLOR, STOP_COLOR);

    return 0;
}