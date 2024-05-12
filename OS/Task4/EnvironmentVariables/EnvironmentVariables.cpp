#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setEnvValue(char nameEnv[], char* newEnvValue) {
    char* env = (char*)malloc(256 * sizeof(char));
    strcat(env, nameEnv);
    strcat(env, "=");
    strcat(env, newEnvValue);
    if (putenv(env))
        perror("Error changing value of envVar");
    //setenv(getenv(nameEnv), newEnvValue, 1);
}

void printEnv(char nameEnv[]) {
    //char* env = getenv(nameEnv);
    printf("Value of %s: %s\n", nameEnv, getenv(nameEnv));
}

void runTask(char* envName) {
    printEnv(envName);
    setEnvValue(envName, "TESTENV");
    printEnv(envName);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: wrong number of args\n");
        return 0;
    }
    runTask(argv[1]);
    return 0;
}