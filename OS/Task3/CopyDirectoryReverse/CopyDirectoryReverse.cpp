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
//bool is_correct_len_folder(char** name_rev_folder, char** path_rev_folder, char** path_origin_folder,
//    const struct dirent* d_entry) {
//    char* name_new_rev_folder, * path_new_rev_folder, * path_new_origin_folder;
//
//    if (strlen(d_entry->d_name) > MAX_LEN_NAME_FILE) {
//        size_t relocation_size = strlen(d_entry->d_name) - MAX_LEN_NAME_FILE + INCREASE_POS;
//
//        name_new_rev_folder = (char*)realloc(*name_rev_folder, relocation_size * sizeof(char));
//        if (name_new_rev_folder == NULL) {
//            perror("Memory allocation name_rev_folder error\n");
//            return false;
//        }
//
//        *name_rev_folder = name_new_rev_folder;
//
//        path_new_rev_folder = (char*)realloc(*path_rev_folder, relocation_size * sizeof(char));
//        if (path_new_rev_folder == NULL) {
//            perror("Memory allocation path_new_rev_folder error\n");
//            return false;
//        }
//        *path_rev_folder = path_new_rev_folder;
//
//        path_new_origin_folder = (char*)realloc(*path_origin_folder, relocation_size * sizeof(char));
//        if (path_new_origin_folder == NULL) {
//            perror("Error: failed to reallocate path_new_origin_folder memory\n");
//            return false;
//        }
//        *path_origin_folder = path_new_origin_folder;
//    }
//    return true;
//}
//
//size_t create_reverse_folder(const char*, const char*);

size_t fillDirectory(const char* pathOriginDirectory, const char* pathReverseDirectory) {
    struct dirent* dEntry;
    DIR* dir = opendir(pathOriginDirectory);
    if (dir == NULL) {
        perror("The file was not open dir");
        return MY_ERROR;
    }

    char* nameReverseDirectory = (char*)malloc(MAX_LEN_NAME_FILE * sizeof(char));
    if (nameReverseDirectory == NULL) {
        closedir(dir);
        perror("Error: failed to reallocate memory name_rev_folder\n");
        return MY_ERROR;
    }

    char* path_new_rev_folder = (char*)malloc(MAX_LEN_NAME_FILE + strlen(pathReverseDirectory) * sizeof(char));
    if (path_new_rev_folder == NULL) {
        closedir(dir);
        free(nameReverseDirectory);
        perror("Error: failed to reallocate memory path_new_rev_folder\n");
        return MY_ERROR;
    }
    char* path_new_origin_folder = (char*)malloc((MAX_LEN_NAME_FILE + strlen(pathOriginDirectory)) * sizeof(char));
    if (path_new_origin_folder == NULL) {
        closedir(dir);
        free(nameReverseDirectory);
        free(path_new_rev_folder);
        perror("Error: failed to reallocate memory path_new_origin_folder\n");
        return MY_ERROR;
    }

    size_t ret = OK;

    while ((dEntry = readdir(dir)) != NULL) {

        if (!is_correct_len_folder(&nameReverseDirectory,
            &path_new_rev_folder,
            &path_new_origin_folder,
            dEntry)) {
            free(nameReverseDirectory);
            free(path_new_rev_folder);
            free(path_new_origin_folder);
            closedir(dir);
            return MY_ERROR;
        } 

        nameReverseDirectory = strncpy(nameReverseDirectory, dEntry->d_name, strlen(dEntry->d_name));
        reverse_line(nameReverseDirectory, strlen(nameReverseDirectory));


        int ret_reverse = sprintf(path_new_rev_folder, "%s/%s", pathReverseDirectory, nameReverseDirectory);
        int ret_origin = sprintf(path_new_origin_folder, "%s/%s", pathOriginDirectory, dEntry->d_name);

        if (ret_reverse < 0 || ret_origin < 0) {
            closedir(dir);
            free(nameReverseDirectory);
            free(path_new_rev_folder);
            free(path_new_origin_folder);
            perror("Error call sprintf");
            return MY_ERROR;
        }

        if (dEntry->d_type == DT_DIR && !is_curr_or_prev_dir(dEntry->d_name)) {
            ret = create_reverse_folder(path_new_origin_folder, path_new_rev_folder);
        }
        else if (dEntry->d_type == DT_REG) {
            ret = create_reverse_file(path_new_origin_folder, path_new_rev_folder);
        }

        if (ret == MY_ERROR ||
            !(dEntry->d_type == DT_DIR || dEntry->d_type == DT_REG)) {
            free(nameReverseDirectory);
            free(path_new_rev_folder);
            free(path_new_origin_folder);
            closedir(dir);
            return MY_ERROR;
        }

        memset(nameReverseDirectory, 0, strlen(path_new_origin_folder) * sizeof(char));
        memset(path_new_rev_folder, 0, strlen(path_new_origin_folder) * sizeof(char));
        memset(path_new_origin_folder, 0, strlen(path_new_origin_folder) * sizeof(char));
    }

    free(nameReverseDirectory);
    free(path_new_rev_folder);
    free(path_new_origin_folder);
    closedir(dir);
    return OK;
}

char* findOriginDirectoryName(char* pathToDirectory) {
    char* originDirectoryName = strrchr(pathToDirectory, '/');
    if (originDirectoryName == NULL) {
        originDirectoryName = pathToDirectory;
    }
    else {
        originDirectoryName += INCREASE_POS;
    }
    return originDirectoryName;
}

void reverseLine(char* lineToReverse, char* reverseLine)
{
    int nameLength = strlen(lineToReverse);
    for (int i = 0; i < nameLength; i++) {
        reverseLine[i] = lineToReverse[nameLength - 1 - i];
    }
}

void fillDirectory(char* pathToOriginDirectory, char* pathToReverseDirectory)
{

}

size_t createReverseDirectory(char* directoryName, char* pathToDirectory)
{
    //int ret = mkdir(directoryName, 0666);
    int ret = _mkdir(directoryName);
    if (ret == -1) {
        perror("error ");
        return MY_ERROR;
    }
    fillDirectory(pathToDirectory, pathToDirectory);
    //fill_folder(path_origin_folder, path_reverse_folder);
    return OK;
}

size_t run1(int argc, char* argv[])
{
    if (argc != CORRECT_NUMBER_ARGS) {
        fprintf(stderr, "use: %s path/to/directory\n", argv[1]);
        return MY_ERROR;
    }
    char* originDirectoryName = findOriginDirectoryName(argv[1]);

    int directoryNameLength = strlen(originDirectoryName);
    if (directoryNameLength == 0) {
        return MY_ERROR;
    }
    char* reverseDirectoryName = (char *)calloc(directoryNameLength, sizeof(char));
    reverseLine(originDirectoryName, reverseDirectoryName);
    int ret = createReverseDirectory(reverseDirectoryName, reverseDirectoryName);
    //std::cout << lengt << std::endl;
    //createReverseDirectory()
}

int main(int argc, char* argv[]) {
    size_t ret = run1(argc, argv);
    return  (ret == MY_ERROR) ? 1 : 0;
}