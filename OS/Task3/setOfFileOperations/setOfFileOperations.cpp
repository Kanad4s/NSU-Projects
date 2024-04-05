#include "setCommands.h"
#include <string.h>

enum constants {
    CORRECT_NUMBER_ARGS = 2,
};

char* getCommandName(char* argv) {
    char* command = strrchr(argv, '/');
    if (command == NULL) {
        command = argv;
    }
    else {
        command += 1;
    }
    return command;
}

void executeCommand(const char* name_command, const char* name_util) {
    if (strcmp(name_command, "makeDir") == 0) {
        makeDirectory(name_util); //a
    }
    else if (strcmp(name_command, "printDir") == 0) {
        printDirectory(name_util); //b
    }
    else if (strcmp(name_command, "removeDir") == 0) {
        removeDirectory(name_util); //c
    }
    else if (strcmp(name_command, "touchFile") == 0) {
        touchFile(name_util); //d
    }
    else if (strcmp(name_command, "printFile") == 0) {
        printFile(name_util);//e
    }
    else if (strcmp(name_command, "removeFile") == 0) {
        removeFile(name_util); //f
    }
    else if (strcmp(name_command, "makeSymbolLink") == 0) {
        makeSymbolicLink(name_util); //g
    }
    else if (strcmp(name_command, "printSymLink") == 0) {
        printSymbolicLink(name_util); //h
    }
    else if (strcmp(name_command, "printFileFromSymLink") == 0) {
        printFileFromSymbolicLink(name_util); //i
    }
    else if (strcmp(name_command, "removeSymLink") == 0) {
        removeSymbolicLink(name_util);  //j
    }
    else if (strcmp(name_command, "makeHardLink") == 0) {
        makeHardLink(name_util); //k
    }
    else if (strcmp(name_command, "removeHardLink") == 0) {
        removeHardLink(name_util); //l
    }
    else if (strcmp(name_command, "printRights") == 0) {
        printRights(name_util); //m
    }
    else if (strcmp(name_command, "changeRights") == 0) {
        changeRights(name_util); //n
    }
    else {
        fprintf(stderr, "Command was not found\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != CORRECT_NUMBER_ARGS) {
        perror("Error wrong number of args\n");
        return 0;
    }
    executeCommand(getCommandName(argv[0]), argv[1]);
    return 0;
}
