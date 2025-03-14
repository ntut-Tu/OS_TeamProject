//
// Created by quinn on 2025/3/13.
//
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

#include "include/PrintUtils.h"

#define IS_VERBOSE(arg) ((arg) != NULL && (strcmp((arg), "--verbose") == 0 || strcmp((arg), "-v") == 0))

void fileCheckError() {
    if (errno) {
        printf("[Error]: %s\n", strerror(errno));
        errno = 0;
        exit(1);
    }
}

void printWelcome() {
    char *welcome = "Welcome to myShell!\nType \033[33mbye\033[0m to exit.\nUse \033[33m--verbose\033[0m or \033[33m-v\033[0m in argv[] to enable full logging.\n";
    printDivider( getLongestLine(welcome));
    printf("%s", welcome);
    printDivider( getLongestLine(welcome));
}

int main(int argc, char *argv[]) {
    char input[256];
    printWelcome();
    if(IS_VERBOSE(argv[1])) {
        runtimePrint("Verbose mode enabled.");
    }
    while(1){
        printf("myShell> ");
        fgets(input, 256, stdin);

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0'; // 要補\0否則需要重新清理字元陣列
        }

        char *command;
        command = strtok(input, " ");
        char *args;
        args = strtok(NULL, " ");

        if(strcmp(command, "bye") == 0){
            break;
        }

        if(fork()==0){
            if(IS_VERBOSE(argv[1])) {
                runtimePrint("Executing command: %s %s\n", command, args);
            }
            execlp(command, args, NULL);
            fileCheckError();
            exit(0);
        }else{
            wait(NULL);
        }
    }
    return 0;
}