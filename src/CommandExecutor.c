//
// Created by quinndu0 on 2025/3/14.
//
#include "../include/CommandExecutor.h"
#include "../include/PrintUtils.h"
#include "../include/ErrorHandler.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void executeCommand(const char *input, int verboseMode) {
    char *command = strtok(strdup(input), " ");
    char *args = strtok(NULL, " ");

    if (fork() == 0) {
        if (verboseMode) {
            runtimePrint("Executing command: %s %s", command, args ? args : "");
        }
        execlp(command, command, args, NULL);
        fileCheckError();
        exit(1);
    } else {
        wait(NULL);
    }
}
