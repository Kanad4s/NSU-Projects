#include "setCommands.h"

enum constant_read {
    BUFFER_SIZE = 256,
    LINK_EXTRA_LENGTH = 4,
    SYMBOLIC_LINK_EXTRA_LENGTH = 7,
    HARD_LINK_EXTRA_LENTGH = 8
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
    lstat(link, &statLink);
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
    fwrite("\n", sizeof(char), 1, stdout);
    free(buffer);
}

void printFileFromSymbolicLink(const char* link) {
    printFile(link);
}

void removeSymbolicLink(const char* link) {
    if (isSymbolicLink(link)) {
        removeFile(link);
    }
    else {
        fprintf(stdout, "File is not symbolic link\n");
    }
}

void makeHardLink(const char* fileName) {
    char* linkName = (char*)malloc(sizeof(char) * (strlen(fileName) + HARD_LINK_EXTRA_LENTGH));
    sprintf(linkName, "%sHardLink", fileName);
    link(fileName, linkName);
    if (errno) {
        free(linkName);
        perror("Error making link");
        return;
    }
    free(linkName);
}

void removeHardLink(const char* link) {
    unlink(link);
    if (errno) {
        perror("Error removing link");
    }
}

char* getRights(struct stat* fileStat) {
    char* permissions = (char*)malloc(10);
    permissions[0] = '-';
    if (S_ISLNK(fileStat->st_mode)) {
        permissions[0] = 'l';
    }
    else if (S_ISDIR(fileStat->st_mode)) {
        permissions[0] = 'd';
    }
    permissions[1] = (fileStat->st_mode & S_IRUSR ? 'r' : '-');
    permissions[2] = (fileStat->st_mode & S_IWUSR ? 'w' : '-');
    permissions[3] = (fileStat->st_mode & S_IXUSR ? 'x' : '-');
    permissions[4] = (fileStat->st_mode & S_IRGRP ? 'r' : '-');
    permissions[5] = (fileStat->st_mode & S_IWGRP ? 'w' : '-');
    permissions[6] = (fileStat->st_mode & S_IXUSR ? 'x' : '-');
    permissions[7] = (fileStat->st_mode & S_IROTH ? 'r' : '-');
    permissions[8] = (fileStat->st_mode & S_IWOTH ? 'w' : '-');
    permissions[9] = (fileStat->st_mode & S_IXOTH ? 'x' : '-');
    return permissions;
}

void printRights(const char* fileName) {
    struct stat fileState;
    stat(fileName, &fileState);
    if (errno) {
        perror("Error getting file statistic");
        return;
    }
    char* permissions = getRights(&fileState);
    printf("rights: %s\n", permissions);
    printf("hard links: %ld\n", fileState.st_nlink);
    free(permissions);
}

void changeRights(const char* fileName) {
    struct stat fileStat;
    stat(fileName, &fileStat);
    if (errno) {
        perror("Error getting file statistic");
        return;
    }
    mode_t new_mode = (mode_t)(rand() % ALL_FOR_ALL);
    chmod(fileName, new_mode);
    if (errno) {
        perror("Error changing rights");
    }
    //mode of 'as' changed from 0666 (rw - rw - rw - ) to 0644 (rw - r--r--)
}

//Each MODE is of the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.