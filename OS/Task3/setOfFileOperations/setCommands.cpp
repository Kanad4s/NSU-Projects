#include "setCommands.h"

enum constant_read {
    BUFFER_SIZE = 256,
};

enum permitted {
    DEFAULT_DIR = 0775,
    ALL_FOR_ALL = 0777
};

void makeDirectory(const char* directoryName) {
    mkdir(directoryName, DEFAULT_DIR);
    if (errno) {
        perror("Error creating directory");
    }
}

void printDirectory(const char* pathToDirectory) {

    DIR* dir = opendir(pathToDirectory);
    if (dir == NULL) {
        printf("Directory %s ", pathToDirectory);
        perror("was not open\n");
        return;
    }
    struct dirent* dEntry = readdir(dir);
    while (dEntry != NULL) {
        printf("%s\n", dEntry->d_name);
        dEntry = readdir(dir);
    }
    closedir(dir);
}

void removeDirectory(const char* directoryName) {
    rmdir(directoryName);
    if (errno) {
        perror("Error removing directory");
    }
}

void touchFile(const char* fileName) {
    FILE* ptr_file = fopen(fileName, "a+");
    if (ptr_file == NULL) {
        perror("Error touching file");
        return;
    }
    fclose(ptr_file);
    if (errno) {
        perror("Error closing file");
    }
}

enum returnStatus is_file_reg(const char* name_file) {
    struct stat stat_file;

    int ret = stat(name_file, &stat_file);
    if (ret == MY_ERROR) {
        perror("Error in call stat\n");
        return MY_ERROR;
    }

    if (!S_ISREG(stat_file.st_mode)) {
        perror("The file is not a reg\n");
        return MY_ERROR;
    }
    return OK;
}

enum returnStatus write_to_file_implements(const char* buffer, const size_t size_written, FILE* stream) {
    size_t sum_written = 0;

    while (sum_written < size_written) {
        int written = fwrite(buffer + sum_written, sizeof(char), size_written - sum_written, stream);

        if (written <= MY_ERROR) {
            perror("Error in call fwrite");
            return MY_ERROR;
        }
        sum_written += written;
    }
    return OK;
}

enum returnStatus write_file(FILE* ptr_file) {
    long file_size = ftell(ptr_file);
    printf("%ld", file_size);
    size_t ret = fseek(ptr_file, 0, SEEK_SET);

    if (ret == MY_ERROR) {
        fclose(ptr_file);
        perror("Error in call file seek\n");
        return MY_ERROR;
    }

    char buffer[BUFFER_SIZE];
    long bytes_left = file_size;
    while (bytes_left > 0) {
        long bytes_to_read = bytes_left < BUFFER_SIZE ? bytes_left : BUFFER_SIZE;

        size_t bytes_read = fread(buffer, 1, bytes_to_read, ptr_file);
        if (feof(ptr_file) || ferror(ptr_file)) {
            perror("Failed to read input file when call file read\n");
            return MY_ERROR;
        }
        bytes_left -= bytes_read;

        enum returnStatus writing_status = write_to_file_implements(buffer, bytes_read, stdout);
        if (writing_status == MY_ERROR) {
            return MY_ERROR;
        }
    }

    return OK;
}

void printFile(const char* filePath) {
    if (is_file_reg(filePath) == MY_ERROR)
        return MY_ERROR;

    FILE* ptr_file = fopen(filePath, "a+");
    if (ptr_file == NULL) {
        perror("Error occurred while open file\n");
        return MY_ERROR;
    }

    enum returnStatus ret = write_file(ptr_file);
    fclose(ptr_file);
    return ret;
}

enum returnStatus removeFile(const char* path_name_file) {
    int ret = remove(path_name_file);
    if (ret == MY_ERROR) {
        perror("Error occurred while remove file\n");
        return MY_ERROR;
    }
    return OK;
}

enum returnStatus is_sym_link(const char* sym_link) {
    struct stat stat_link;

    int ret = stat(sym_link, &stat_link);
    if (ret == MY_ERROR) {
        perror("Error in call stat\n");
        return MY_ERROR;
    }

    if (!S_ISLNK(stat_link.st_mode)) {
        perror("The file is not a symbolic link\n");
        return MY_ERROR;
    }
    return OK;
}

enum returnStatus makeSymbolicLink(const char* name_file) {
    int ret = symlink(name_file, "symbol_link");
    if (ret == MY_ERROR) {
        perror("Error occurred while symlink\n");
        return MY_ERROR;
    }
    return OK;
}

enum returnStatus printSymbolicLink(const char* sym_link) {
    if (is_sym_link(sym_link) == MY_ERROR) {
        return MY_ERROR;
    }

    char buffer[BUFFER_SIZE];
    size_t sum_written = readlink(sym_link, buffer, BUFFER_SIZE);
    if (sum_written == MY_ERROR) {
        perror("Error in call readlink");
        return MY_ERROR;
    }

    enum returnStatus writing_status = write_to_file_implements(buffer, sum_written, stdout);
    if (writing_status == MY_ERROR) {
        return MY_ERROR;
    }
    return OK;
}

enum returnStatus printFileFromSymbolicLink(const char* sym_link) {
    if (is_sym_link(sym_link) == MY_ERROR) {
        return MY_ERROR;
    }

    char buffer[BUFFER_SIZE];
    size_t ret = readlink(sym_link, buffer, BUFFER_SIZE);
    if (ret == MY_ERROR) {
        perror("Error in call readlink");
        return MY_ERROR;
    }

    FILE* ptr_file = fopen(buffer, "a+"); //TODO change rb
    if (ptr_file == NULL) {
        perror("Error in call fopen");
        return MY_ERROR;
    }

    ret = write_file(ptr_file);
    fclose(ptr_file);
    return ret;
}

enum returnStatus removeSymbolicLink(const char* sym_link) {
    enum returnStatus ret = is_sym_link(sym_link);
    if (ret == MY_ERROR)
        return MY_ERROR;

    return removeFile(sym_link);
}

enum returnStatus makeHardLink(const char* hard_link) {
    int ret = link(hard_link, "hard_link");
    if (ret == MY_ERROR) {
        perror("Error in call link\n");
        return MY_ERROR;
    }
    return OK;
}

enum returnStatus removeHardLink(const char* hard_link) {
    int ret = unlink(hard_link);
    if (ret == MY_ERROR) {
        perror("Error in call unlink\n");
        return MY_ERROR;
    }
    return OK;
}

char* get_permissions(struct stat* file_stat) {
    char* permissions = (char*)malloc(10);
    permissions[0] = (S_ISDIR(file_stat->st_mode) ? 'd' : '-');
    permissions[0] = (file_stat->st_mode & S_IRUSR ? 'r' : '-');
    permissions[1] = (file_stat->st_mode & S_IWUSR ? 'w' : '-');
    permissions[2] = (file_stat->st_mode & S_IXUSR ? 'x' : '-');
    permissions[3] = (file_stat->st_mode & S_IRGRP ? 'r' : '-');
    permissions[4] = (file_stat->st_mode & S_IWGRP ? 'w' : '-');
    permissions[5] = (file_stat->st_mode & S_IXUSR ? 'x' : '-');
    permissions[6] = (file_stat->st_mode & S_IROTH ? 'r' : '-');
    permissions[7] = (file_stat->st_mode & S_IWOTH ? 'w' : '-');
    permissions[8] = (file_stat->st_mode & S_IXOTH ? 'x' : '-');
    permissions[9] = (file_stat->st_mode & S_ISVTX ? 't' : ' ');

    return permissions;
}

enum returnStatus printRights(const char* name_file) {
    struct stat buff;

    int ret = stat(name_file, &buff);
    if (ret == MY_ERROR) {
        perror("Error in call stat\n");
        return MY_ERROR;
    }
    char* permissions = get_permissions(&buff);
    printf("permissions: %s\n", permissions);
    printf("count hard links: %ld\n", buff.st_nlink);

    free(permissions);
    return OK;
}

enum returnStatus changeRights(const char* name_file) {
    struct stat buff;
    int ret = stat(name_file, &buff);
    if (ret == MY_ERROR) {
        perror("Error in call stat\n");
        return MY_ERROR;
    }

    mode_t new_mode = (mode_t)(rand() % ALL_FOR_ALL);

    ret = chmod(name_file, new_mode);
    if (ret == MY_ERROR) {
        perror("Error changing file permissions\n");
        return MY_ERROR;
    }
    return OK;
}