#include "setCommands.h"

enum constant_read {
    BUFFER_SIZE = 256,
    LINK_EXTRA_LENGTH = 4,
    SYMBOLIC_LINK_EXTRA_LENGTH = 7
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
        perror("error openning directory");
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

int isFileRegular(const char* fileName) {
    struct stat fileStat;
    stat(fileName, &fileStat);
    if (errno) {
        perror("Error getting file statistic");
        return 0;
    }
    return S_ISREG(fileStat.st_mode);
}

void printFile(const char* filePath) {
    if (!isFileRegular(filePath)) {
        printf("file is not regular\n");
        return;
    }
    FILE* file = fopen(filePath, "a+");
    if (file == NULL) {
        perror("Error openning file");
        return;
    }
    struct stat fileStat;
    stat(filePath, &fileStat);
    size_t fileSize = fileStat.st_size;
    char buffer[BUFFER_SIZE];
    size_t bytesRead = BUFFER_SIZE;
    while (bytesRead == BUFFER_SIZE) {
        bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        if (errno) {
            perror("Error reading file");
            return;
        }
        fwrite(buffer, sizeof(char), bytesRead, stdout);
    }
    fclose(file);
}

void removeFile(const char* fileName) {
    int ret = remove(fileName);
    if (errno) {
        perror("Error removing file");
    }
}

int isSymbolicLink(const char* link) {
    struct stat statLink;
    stat(link, &statLink);
    if (errno) {
        perror("Error getting link statistic");
        return 0;
    }
    return S_ISLNK(statLink.st_mode);
}

void makeSymbolicLink(const char* fileName) {
    char* linkName = (char*)malloc(sizeof(char) * (strlen(fileName) + SYMBOLIC_LINK_EXTRA_LENGTH));
    sprintf(linkName, "%sSymLink", fileName);
    symlink(fileName, linkName);
    if (errno) {
        free(linkName);
        perror("Error making symbolic link");
        return;
    }
    free(linkName);
}

void printSymbolicLink(const char* link) {
    if (!isSymbolicLink(link)) {
        printf("File is not symbolic link\n");
        return;
    }
    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    size_t bytesRead = readlink(link, buffer, BUFFER_SIZE);
    if (errno) {
        free(buffer);
        perror("Error reading link");
        return;
    }
    int i = 2;
    while (bytesRead == BUFFER_SIZE) {
        buffer = (char*)realloc(buffer, sizeof(char) * BUFFER_SIZE * i);
        bytesRead = readlink(link, buffer, BUFFER_SIZE);
        if (errno) {
            free(buffer);
            perror("Error reading link");
            return;
        }
        i++;
    }
    fwrite(buffer, sizeof(char), bytesRead, stdout);
    free(buffer);
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
    enum returnStatus ret = isSymbolicLink(sym_link);
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