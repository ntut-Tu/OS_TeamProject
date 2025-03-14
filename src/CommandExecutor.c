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
    char *cmdCopy = strdup(input); // malloc + strcpy
    if (!cmdCopy) {
        perror("strdup failed");
        return;
    }

    char *args[100]; // 假設最多 100 個參數
    int argCount = 0;

    char *token = strtok(cmdCopy, " ");
    while (token != NULL && argCount < 99) { // 最後一格留給 NULL
        args[argCount++] = token;
        token = strtok(NULL, " ");
    }
    args[argCount] = NULL; // exec 需要 NULL 結尾

    if (argCount == 0) {
        free(cmdCopy);
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        // child
        if (verboseMode) {
            runtimeCharPrint("Executing command: ", args);
        }
        execvp(args[0], args); // exec()
        perror("execvp failed"); // 若執行失敗才會跑到這行
        exit(1);
    } else {
        // parent
        waitpid(pid, NULL, 0);
    }

    free(cmdCopy);
}
