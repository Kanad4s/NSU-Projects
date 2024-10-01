#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
void makeHardLink(const char* fileName);
void removeHardLink(const char* link);
void printRights(const char* fileName);
void changeRights(const char* fileName);