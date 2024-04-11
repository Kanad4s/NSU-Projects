#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

enum  returnStatus {
    MY_ERROR = 1,
    OK = 0
};

void makeDirectory(const char* directoryName);
void printDirectory(const char* pathToDirectory);
void removeDirectory(const char* directoryName);
void touchFile(const char* fileName);
void printFile(const char* filePath);
void removeFile(const char* fileName);
void makeSymbolicLink(const char* fileName);
void printSymbolicLink(const char* link);
void printFileFromSymbolicLink(const char* link);
void removeSymbolicLink(const char* link);
enum returnStatus makeHardLink(const char* hard_link);
enum returnStatus removeHardLink(const char* hard_link);
enum returnStatus printRights(const char* name_file);
enum returnStatus changeRights(const char* name_file);