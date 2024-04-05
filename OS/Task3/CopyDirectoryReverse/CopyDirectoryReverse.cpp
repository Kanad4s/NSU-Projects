#include <direct.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

enum constants {
    BUFFER_SIZE = 256,
    MAX_NAME_FILE_LENGTH = 256,
    MY_ERROR = 1,
    INCREASE_POS = 1,
    CORRECT_NUMBER_ARGS = 2,
    OK = 0
};

bool isDEntryNameLengthAvailable(int nameLength) {
    if (nameLength > MAX_NAME_FILE_LENGTH) {
        return false;
    }
    return true;
}

void swap(char* sym_first, char* sym_second) {
    char sym_current = *sym_first;
    *sym_first = *sym_second;
    *sym_second = sym_current;
}

void reverseLineByLength(char* line, const size_t size) {
    int mid_len = size / 2;
    for (int i = 0; i < mid_len; i++) {
        swap(&line[i], &line[size - i - INCREASE_POS]);
    }
}

void reverseLine(char* lineToReverse, char* reverseLine) {
    int lineLength = strlen(lineToReverse);
    char* bufferLine = (char*)malloc(sizeof(char) * strlen(lineToReverse));
    memcpy(bufferLine, lineToReverse, lineLength);
    for (int i = 0; i < lineLength; i++) {
        reverseLine[i] = bufferLine[lineLength - 1 - i];
    }
}

char* getOriginDirectoryName(char* pathToDirectory) {
    char* originDirectoryName = strrchr(pathToDirectory, '/');
    if (originDirectoryName == NULL) {
        originDirectoryName = pathToDirectory;
    }
    else {
        originDirectoryName += INCREASE_POS;
    }
    return originDirectoryName;
}

char* getPathToOriginDirectory(char* directoryPath, char* directoryName) {
    int pathToDirectoryLength = strlen(directoryPath) - strlen(directoryName);
    char* pathToDirectory = (char*)malloc(sizeof(char) * pathToDirectoryLength);
    memcpy(pathToDirectory, directoryPath, sizeof(char) * pathToDirectoryLength);
    return pathToDirectory;
}

bool isCurrentOrPreviouseDirectory(char* directory) {
    return (strcmp(directory, ".") == 0 || strcmp(directory, "..") == 0);
}

size_t createReverseFile(char* pathToOrigin, char* pathToReverse) {
    FILE* originFile = fopen(pathToOrigin, "rb");
    size_t ret = OK;
    if (originFile == NULL) {
        perror("originFile was not open");
        return MY_ERROR;
    }
    FILE* reverseFile = fopen(pathToReverse, "wb");
    if (originFile == NULL) {
        perror("reverseFile was not open");
        fclose(originFile);
        return MY_ERROR;
    }
    struct stat fileStat;
    stat(pathToOrigin, &fileStat);
    //printf("name = %s, size = %ld,\n",pathToOrigin, fileStat.st_size);
    if (errno) {
        perror("error to get file stat");
        return MY_ERROR;
    }
    int sizeLeft = fileStat.st_size;
    char buffer[BUFFER_SIZE];
    while (sizeLeft > 0) {
        size_t bytesToRead = sizeLeft < BUFFER_SIZE ? sizeLeft : BUFFER_SIZE;
        size_t bytesRead = fread(buffer, sizeof(char), bytesToRead, originFile);
        reverseLineByLength(buffer, bytesRead);
        sizeLeft -= bytesRead;
        ret = fseek(reverseFile, sizeLeft, SEEK_SET);
        size_t bytesWritten = fwrite(buffer, sizeof(char), bytesRead, reverseFile);
        if (errno) {
            perror("error write in file");
            fclose(originFile);
            fclose(reverseFile);
            return MY_ERROR;
        }
    }
    fclose(originFile);
    fclose(reverseFile);
    return OK;
}

size_t createReverseDirectory(char* pathToDirectory, char* pathToReverseDirectory);

size_t fillDirectory(const char* pathToOriginDirectory, const char* pathToReverseDirectory) {
    struct dirent* dEntry;
    DIR* dir = opendir(pathToOriginDirectory);
    if (dir == NULL) {
        printf("%s\n", pathToOriginDirectory);
        perror("The file was not open");
        return MY_ERROR;
    }
    char* nameReverseDirectory = (char*)malloc(MAX_NAME_FILE_LENGTH * sizeof(char));
    char* pathToNewReverseDirectory = (char*)malloc(MAX_NAME_FILE_LENGTH + strlen(pathToReverseDirectory) * sizeof(char));
    char* pathToNewOriginDirectory = (char*)malloc((MAX_NAME_FILE_LENGTH + strlen(pathToOriginDirectory)) * sizeof(char));
    size_t returnValue = OK;
    while ((dEntry = readdir(dir)) != NULL) {
        if (!isDEntryNameLengthAvailable(strlen(dEntry->d_name))) {
            nameReverseDirectory = (char*)realloc(nameReverseDirectory, sizeof(char) * strlen(dEntry->d_name));
            pathToNewReverseDirectory = (char*)realloc(pathToNewReverseDirectory, sizeof(char) * (strlen(pathToNewReverseDirectory) +
                                        MAX_NAME_FILE_LENGTH));
        }
        memcpy(nameReverseDirectory, dEntry->d_name, strlen(dEntry->d_name));
        reverseLine(nameReverseDirectory, nameReverseDirectory);
        int newOriginDirectoryPathLength = sprintf(pathToNewOriginDirectory, "%s/%s", pathToOriginDirectory, dEntry->d_name);
        int newReverseDirectoryPathLength = sprintf(pathToNewReverseDirectory, "%s/%s", pathToReverseDirectory, nameReverseDirectory);
        if (newOriginDirectoryPathLength < 0 || newReverseDirectoryPathLength < 0) {
            free(nameReverseDirectory);
            free(pathToNewReverseDirectory);
            free(pathToNewOriginDirectory);
            closedir(dir);
            perror("error call sprintf ");
            return MY_ERROR;
        }
        //printf("newOriginPath: %s\nnewReversePath: %s\n", pathToNewOriginDirectory, pathToNewReverseDirectory);
        if (dEntry->d_type == DT_DIR && !isCurrentOrPreviouseDirectory(dEntry->d_name)) {
            returnValue = createReverseDirectory(pathToNewOriginDirectory, pathToNewReverseDirectory);
        }
        else if (dEntry->d_type == DT_REG) {
            returnValue = createReverseFile(pathToNewOriginDirectory, pathToNewReverseDirectory);
        }

        if (returnValue == MY_ERROR) {
            free(nameReverseDirectory);
            free(pathToNewReverseDirectory);
            free(pathToNewOriginDirectory);
            closedir(dir);
            return MY_ERROR;
        }
        memset(nameReverseDirectory, 0, strlen(nameReverseDirectory) * sizeof(char));
        memset(pathToNewReverseDirectory, 0, strlen(pathToNewReverseDirectory) * sizeof(char));
        memset(pathToNewOriginDirectory, 0, strlen(pathToNewOriginDirectory) * sizeof(char));
    }
    free(nameReverseDirectory);
    free(pathToNewReverseDirectory);
    free(pathToNewOriginDirectory);
    closedir(dir);
    return OK;
}

size_t createReverseDirectory(char* pathToDirectory, char* pathToReverseDirectory) {
    //mkdir(pathToReverseDirectory, 0777);
    _mkdir(pathToReverseDirectory);
    if (errno) {
        perror("error make directory");
        return MY_ERROR;
    }
    size_t returnValue = fillDirectory(pathToDirectory, pathToReverseDirectory);
    return returnValue;
}

size_t run(int argc, char* argv[]) {
    if (argc != CORRECT_NUMBER_ARGS) {
        fprintf(stderr, "use: %s path/to/directory\n", argv[1]);
        return MY_ERROR;
    }
    char* originDirectoryName = getOriginDirectoryName(argv[1]);
    //char* pathToOriginDirectory = getPathToOriginDirectory(argv[1], originDirectoryName);
    //printf("name: %s, path: %s\n", originDirectoryName, pathToOriginDirectory);
    int directoryNameLength = strlen(originDirectoryName);
    if (directoryNameLength == 0) {
        return MY_ERROR;
    }
    char* reverseDirectoryName = (char *)calloc(directoryNameLength, sizeof(char));
    reverseLine(originDirectoryName, reverseDirectoryName);
    size_t returnValue = createReverseDirectory(argv[1], reverseDirectoryName);
    //free(pathToOriginDirectory);
    free(reverseDirectoryName);
    return returnValue;
}

int main(int argc, char* argv[]) {
    size_t returnValue = run(argc, argv);
    return  (returnValue == MY_ERROR) ? 1 : 0;
}