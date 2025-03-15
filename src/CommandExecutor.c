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
#include <fcntl.h>

Command *parseCommand(const char *input, int *cmdCount) {
    char *inputCopy = strdup(input);    // malloc + strcpy
    if (!inputCopy) {
        perror("strdup failed");
        return NULL;
    }

    Command *commands = malloc(sizeof(Command) * 10);
    for (int i = 0; i < 10; i++) {
        commands[i].cmd = NULL;
        commands[i].args = malloc(sizeof(char *) * 100);
        commands[i].inputFile = NULL;
        commands[i].outputFile = NULL;
        commands[i].appendMode = 0;
        commands[i].background = 0;
    }// 重新初始化，malloc到新的記憶體有時會包含亂碼的內容

    *cmdCount = 0;

    char *token = strtok(inputCopy, " ");
    int argCount = 0;
    commands[*cmdCount].args = malloc(sizeof(char *) * 100);

    while (token != NULL) {
        if (strcmp(token, "|") == 0) { // 管線
            commands[*cmdCount].args[argCount] = NULL;
            (*cmdCount)++;
            commands[*cmdCount].args = malloc(sizeof(char *) * 100);
            argCount = 0;
        } else if (strcmp(token, "<") == 0) { // 輸入重定向
            token = strtok(NULL, " ");
            commands[*cmdCount].inputFile = strdup(token);
        } else if (strcmp(token, ">") == 0) { // 輸出重定向
            token = strtok(NULL, " ");
            commands[*cmdCount].outputFile = strdup(token);
            commands[*cmdCount].appendMode = 0;
        } else if (strcmp(token, ">>") == 0) { // 追加模式
            token = strtok(NULL, " ");
            commands[*cmdCount].outputFile = strdup(token);
            commands[*cmdCount].appendMode = 1;
        } else if (strcmp(token, "&") == 0) { // 背景執行
            commands[*cmdCount].background = 1;
        } else { // 普通參數
            commands[*cmdCount].args[argCount++] = strdup(token);
        }
        token = strtok(NULL, " ");
    }
    commands[*cmdCount].args[argCount] = NULL; // NULL 結尾，否則使用 | 時會出現crash
    (*cmdCount)++;

    free(inputCopy);
    return commands;
}

void executeCommand(Command *commands, int cmdCount, int verboseMode) {
    if (verboseMode) {
        printDivider(50);
        runtimePrint("Executing %d commands:", cmdCount);
        for (int i = 0; i < cmdCount; i++) {
            runtimePrint("Command %d:", i);
            runtimeCharPrint("    args: ", commands[i].args);
            if (commands[i].inputFile)
                runtimePrint("    input redirection: %s", commands[i].inputFile);
            if (commands[i].outputFile)
                runtimePrint("    output redirection: %s (append: %d)", commands[i].outputFile, commands[i].appendMode);
            if (commands[i].background)
                runtimePrint("    background: YES");
        }
        printDivider(50);
    }

    int pipes[cmdCount - 1][2];

    for (int i = 0; i < cmdCount; i++) {
        if (i < cmdCount - 1) pipe(pipes[i]);

        pid_t pid = fork();
        if (pid == 0) { // 子進程
            if (verboseMode) runtimePrint("Process %d executing: %s", getpid(), commands[i].args[0]);

            // 確保 args 以 NULL 結尾
            int j = 0;
            while (commands[i].args[j] != NULL) j++;
            commands[i].args[j] = NULL;

            // 處理 `<` 輸入重定向
            if (commands[i].inputFile) {
                int fd = open(commands[i].inputFile, O_RDONLY);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            // 處理 `>` 或 `>>` 輸出重定向
            if (commands[i].outputFile) {
                int flags = O_WRONLY | O_CREAT | (commands[i].appendMode ? O_APPEND : O_TRUNC);
                int fd = open(commands[i].outputFile, flags, 0644);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            // 處理 `|` 管線
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
            if (i < cmdCount - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
                close(pipes[i][0]);
            }

            execvp(commands[i].args[0], commands[i].args);
            perror("execvp failed");
            exit(1);
        }

        if (pid > 0) { // 父進程
            if (i > 0) {
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
            if (!commands[i].background) wait(NULL);
        }
    }
}

void freeCommands(Command *commands, int cmdCount) {
    for (int i = 0; i < cmdCount; i++) {
        free(commands[i].cmd);
        for (int j = 0; commands[i].args[j] != NULL; j++) {
            free(commands[i].args[j]);
        }
        free(commands[i].args);
        if (commands[i].inputFile) free(commands[i].inputFile);
        if (commands[i].outputFile) free(commands[i].outputFile);
    }
    free(commands);
}
