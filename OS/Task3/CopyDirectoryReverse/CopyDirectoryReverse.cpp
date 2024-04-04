#define _CRT_SECURE_NO_WARNINGS
#include <direct.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

enum pos {
    INCREASE_POS = 1,
    START_POS = 0
};

enum constants {
    BUFFER_SIZE = 256,
    MAX_LEN_NAME_FILE = 256,
    MAX_NAME_FILE_LENGTH = 256,
    MY_ERROR = -1,
    MIN_NUMBER_ARGS = 2,
    CORRECT_NUMBER_ARGS = 2,
    OK = 0
};
///// \param sym_first
///// \param sym_second
void swap(char* sym_first, char* sym_second) {
    char sym_current = *sym_first;
    *sym_first = *sym_second;
    *sym_second = sym_current;
}
//
///// \param line
///// \param size
void reverse_line(char* line, const size_t size) {
    int mid_len = size / 2;
    for (int i = 0; i < mid_len; i++) {
        swap(&line[i], &line[size - i - INCREASE_POS]);
    }
}
//
///// \param dir
///// \return bool
//bool is_curr_or_prev_dir(char* dir) {
//    return (strcmp(dir, ".") == 0 || strcmp(dir, "..") == 0);
//}
//
///// \param path_origin_folder
///// \param path_to_folder
///// \param new_folder
///// \return STATUS ERROR/OK
//size_t find_name_folder(char* path_origin_folder, char** path_to_folder, char** new_folder) {
//    char* pos_slash = strrchr(path_origin_folder, '/');
//    *new_folder = (!pos_slash) ? (char*)path_origin_folder : pos_slash + INCREASE_POS;
//
//    size_t folder_len = pos_slash ? (size_t)(pos_slash - path_origin_folder + INCREASE_POS) : START_POS;
//
//    *path_to_folder = strncpy(*path_to_folder, path_origin_folder, folder_len);
//
//    if (!pos_slash) {
//        int ret = snprintf(*path_to_folder, sizeof(path_to_folder) + 2, "%s", "./");
//        if (ret == MY_ERROR) {
//            perror("An error occurred while calling snprintf");
//            return MY_ERROR;
//        }
//    }
//    return OK;
//}
//
///// \param input_path
///// \param output_path
///// \return STATUS ERROR/OK
//size_t create_reverse_file(const char* input_path, const char* output_path) {
//    FILE* input_file = fopen(input_path, "rb");
//    size_t ret = OK;
//
//    if (input_file == NULL) {
//        perror("The input file was not opened");
//        return MY_ERROR;
//    }
//
//    FILE* output_file = fopen(output_path, "wb");
//    if (output_file == NULL) {
//        perror("The output file was not opened");
//        fclose(input_file);
//        return MY_ERROR;
//    }
//
//    ret = fseek(input_file, 0, SEEK_END);
//    if (ret == MY_ERROR) {
//        fclose(input_file);
//        fclose(output_file);
//        perror("Error in call file seek");
//        return MY_ERROR;
//    }
//
//    long file_size = ftell(input_file);
//    ret = fseek(input_file, 0, SEEK_SET);
//    if (ret == MY_ERROR) {
//        fclose(input_file);
//        fclose(output_file);
//        perror("Error in call file seek");
//        return MY_ERROR;
//    }
//
//    char buffer[BUFFER_SIZE];
//    long bytes_left = file_size;
//
//    while (bytes_left > 0) {
//        long bytes_to_read = bytes_left < BUFFER_SIZE ? bytes_left : BUFFER_SIZE;
//
//        size_t bytes_read = fread(buffer, 1, bytes_to_read, input_file);
//        if (feof(input_file) || ferror(input_file)) {
//            perror("Failed to read input file when call file read");
//            fclose(input_file);
//            fclose(output_file);
//            return MY_ERROR;
//        }
//
//        reverse_line(buffer, bytes_read);
//        bytes_left -= bytes_read;
//
//        ret = fseek(output_file, bytes_left, SEEK_SET);
//        if (ret == MY_ERROR) {
//            fclose(input_file);
//            fclose(output_file);
//            perror("Error in call file seek");
//            return MY_ERROR;
//        }
//
//        size_t bytes_written = fwrite(buffer, 1, bytes_read, output_file);
//        if (bytes_written == MY_ERROR) {
//            perror("Error call write");
//            fclose(input_file);
//            fclose(output_file);
//            return MY_ERROR;
//        }
//    }
//
//    fclose(input_file);
//    fclose(output_file);
//    return OK;
//}
//
///// \param name_rev_folder
///// \param path_rev_folder
///// \param path_origin_folder
///// \param d_entry
///// \return
bool is_correct_len_folder(char** name_rev_folder, char** path_rev_folder, char** path_origin_folder,
    const struct dirent* d_entry) {
    char* name_new_rev_folder, * path_new_rev_folder, * path_new_origin_folder;

    if (strlen(d_entry->d_name) > MAX_LEN_NAME_FILE) {
        size_t relocation_size = strlen(d_entry->d_name) - MAX_LEN_NAME_FILE + INCREASE_POS;

        name_new_rev_folder = (char*)realloc(*name_rev_folder, relocation_size * sizeof(char));
        if (name_new_rev_folder == NULL) {
            perror("Memory allocation name_rev_folder error\n");
            return false;
        }

        *name_rev_folder = name_new_rev_folder;

        path_new_rev_folder = (char*)realloc(*path_rev_folder, relocation_size * sizeof(char));
        if (path_new_rev_folder == NULL) {
            perror("Memory allocation path_new_rev_folder error\n");
            return false;
        }
        *path_rev_folder = path_new_rev_folder;

        path_new_origin_folder = (char*)realloc(*path_origin_folder, relocation_size * sizeof(char));
        if (path_new_origin_folder == NULL) {
            perror("Error: failed to reallocate path_new_origin_folder memory\n");
            return false;
        }
        *path_origin_folder = path_new_origin_folder;
    }
    return true;
}

bool isDEntryNameLengthAvailable(int nameLength) {
    if (nameLength > MAX_NAME_FILE_LENGTH) {
        return false;
    }
    return true;
}

void reverseLine(char* lineToReverse, char* reverseLine) {
    int lineLength = strlen(lineToReverse);
    char* bufferLine = (char*)malloc(sizeof(char) * strlen(lineToReverse));
    memcpy(bufferLine, lineToReverse, lineLength);
    for (int i = 0; i < lineLength; i++) {
        reverseLine[i] = bufferLine[lineLength - 1 - i];
    }
}

bool isCurrentOrPreviouseDirectory(char* directory) {
    return (strcmp(directory, ".") == 0 || strcmp(directory, "..") == 0);
}

size_t createReverseFile() {

}

size_t createReverseDirectory(char* pathToDirectory, char* pathToReverseDirectory);

size_t fillDirectory(const char* pathToOriginDirectory, const char* pathToReverseDirectory) {
    struct dirent* dEntry;
    DIR* dir = opendir(pathToOriginDirectory);
    if (dir == NULL) {
        perror("The file was not open");
        return MY_ERROR;
    }

    char* nameReverseDirectory = (char*)malloc(MAX_LEN_NAME_FILE * sizeof(char));
    char* pathToNewReverseDirectory = (char*)malloc(MAX_LEN_NAME_FILE + strlen(pathToReverseDirectory) * sizeof(char));
    char* pathToNewOriginDirectory = (char*)malloc((MAX_LEN_NAME_FILE + strlen(pathToOriginDirectory)) * sizeof(char));
    size_t ret = OK;
    int i = 0;
    while ((dEntry = readdir(dir)) != NULL) {
        i++;
        printf("%d %s\n", i, dEntry->d_name);
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
            closedir(dir);
            free(nameReverseDirectory);
            free(pathToNewReverseDirectory);
            free(pathToNewOriginDirectory);
            perror("error call sprintf ");
            return MY_ERROR;
        }
        printf("newOriginPath: %s\nnewReversePath: %s\n", pathToNewOriginDirectory, pathToNewReverseDirectory);
        if (dEntry->d_type == DT_DIR && !isCurrentOrPreviouseDirectory(dEntry->d_name)) {
            ret = createReverseDirectory(pathToNewOriginDirectory, pathToNewReverseDirectory);
        }
        else if (dEntry->d_type == DT_REG) {
            ret = createReverseFile();
        }

        memset(nameReverseDirectory, 0, strlen(nameReverseDirectory) * sizeof(char));
        memset(pathToNewReverseDirectory, 0, strlen(pathToNewReverseDirectory) * sizeof(char));
        memset(pathToNewOriginDirectory, 0, strlen(pathToNewOriginDirectory) * sizeof(char));
    }

    return MY_ERROR;
    /*while ((dEntry = readdir(dir)) != NULL) {


       /* if (dEntry->d_type == DT_DIR && !is_curr_or_prev_dir(dEntry->d_name)) {
            ret = create_reverse_folder(path_new_origin_folder, pathToNewReverseDirectory);
        }
        else if (dEntry->d_type == DT_REG) {
            ret = create_reverse_file(path_new_origin_folder, pathToNewReverseDirectory);
        }

        if (ret == MY_ERROR ||
            !(dEntry->d_type == DT_DIR || dEntry->d_type == DT_REG)) {
            free(nameReverseDirectory);
            free(pathToNewReverseDirectory);
            free(path_new_origin_folder);
            closedir(dir);
            return MY_ERROR;
        }

        memset(nameReverseDirectory, 0, strlen(path_new_origin_folder) * sizeof(char));
        memset(pathToNewReverseDirectory, 0, strlen(path_new_origin_folder) * sizeof(char));
        memset(path_new_origin_folder, 0, strlen(path_new_origin_folder) * sizeof(char));
    }

    free(nameReverseDirectory);
    free(pathToNewReverseDirectory);
    free(path_new_origin_folder);
    closedir(dir);
    return OK;*/
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

size_t createReverseDirectory(char* pathToDirectory, char* pathToReverseDirectory) {
    //int ret = mkdir(pathToReverseDirectory, 0666);
    int ret = _mkdir(pathToReverseDirectory);
    if (ret == -1) {
        perror("error ");
        return MY_ERROR;
    }
    ret = fillDirectory(pathToDirectory, pathToReverseDirectory);
    //fill_folder(path_origin_folder, path_reverse_folder);
    return OK;
}

size_t run1(int argc, char* argv[]) {
    if (argc != CORRECT_NUMBER_ARGS) {
        fprintf(stderr, "use: %s path/to/directory\n", argv[1]);
        return MY_ERROR;
    }
    char* originDirectoryName = getOriginDirectoryName(argv[1]);
    char* pathToOriginDirectory = getPathToOriginDirectory(argv[1], originDirectoryName);
    //printf("name: %s, path: %s\n", originDirectoryName, pathToOriginDirectory);
    int directoryNameLength = strlen(originDirectoryName);
    if (directoryNameLength == 0) {
        return MY_ERROR;
    }
    char* reverseDirectoryName = (char *)calloc(directoryNameLength, sizeof(char));
    reverseLine(originDirectoryName, reverseDirectoryName);
    int ret = createReverseDirectory(originDirectoryName, pathToOriginDirectory);
    //std::cout << lengt << std::endl;
    //createReverseDirectory()
}

int main(int argc, char* argv[]) {
    size_t ret = run1(argc, argv);
    return  (ret == MY_ERROR) ? 1 : 0;
}