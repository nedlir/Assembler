#include "constants.h"
#include "utils.h"

bool is_successfull_malloc(void *pointer)
{
    if (pointer == NULL)
        return FALSE;
    else
        return TRUE;
}

bool is_successfull_fopen(FILE *filePointer)
{
    if (filePointer == NULL)
        return FALSE;
    else
        return TRUE;
}

bool is_successfull_fclose(int closingResult)
{
    if (closingResult == NO_ERROR_CODE)
        return TRUE;
    else
        return FALSE;
}

FILE *file_open(char *fileName, char *fileType)
{
    FILE *filePointer;
    char fileState[2] = "r";
    char workingfile[MAX_FILE_NAME_LENGTH];

    if (strcmp(fileType, INPUT_FILETYPE) == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, INPUT_FILETYPE); /* builds the filename for the fopen */

        strcpy(fileState, "r");

        filePointer = fopen(workingfile, fileState);
    }

    else if (strcmp(fileType, OBJECT_FILETYPE) == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, OBJECT_FILETYPE); /* builds the filename for the fopen */

        strcpy(fileState, "w");

        filePointer = fopen(workingfile, fileState);
    }

    else if (strcmp(fileType, ENTRIES_FILETYPE) == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, ENTRIES_FILETYPE); /* builds the filename for the fopen */

        strcpy(fileState, "w");

        filePointer = fopen(workingfile, fileState);
    }

    else if (strcmp(fileType, EXTERNALS_FILETYPE) == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, EXTERNALS_FILETYPE); /* builds the filename for the fopen */

        strcpy(fileState, "w");

        filePointer = fopen(workingfile, fileState);
    }

    return filePointer;
}

size_t get_file_length(char *fileName)
{
    FILE *fd;
    size_t fileLength;
    int fileClosingResult;
    int newLine;

    fd = file_open(fileName, INPUT_FILETYPE);
    if (!is_successfull_fopen(fd))
        return -1;

    fileLength = 1;
    do /* count instances of '\n' until reaching last line of file */
    {
        newLine = fgetc(fd);
        if (newLine == '\n')
            fileLength++;
    } while (newLine != EOF);

    fileClosingResult = fclose(fd);
    if (!is_successfull_fclose(fileClosingResult))
        exit_program(FILE_CLOSE_ERROR, EXIT_FAILURE);

    return fileLength;
}

void exit_program(char *errorDescription, int exitType)
{
    printf("\n%s%s%s %s", ERRORS_COLOR, errorDescription, STOP_COLOR, EXIT_MESSAGE);
    exit(exitType); /* exitType is either EXIT_SUCCESS or EXIT_FAILURE constant */
}