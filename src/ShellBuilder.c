//
// Created by quinndu0 on 2025/3/14.
//
#include "../include/ShellBuilder.h"
#include "../include/PrintUtils.h"
#include "../include/CommandExecutor.h"
#include "../include/ErrorHandler.h"
#include "../include/HistoryManager.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define IS_VERBOSE(arg) ((arg) != NULL && (strcmp((arg), "--verbose") == 0 || strcmp((arg), "-v") == 0))

static int verboseMode = 0;

void printWelcome() {
    char *welcome = "Welcome to myShell!\nType \033[33mbye\033[0m to exit.\nUse \033[33m--verbose\033[0m or \033[33m-v\033[0m in argv[] before build to enable full logging.\n";
    printDivider( getLongestLine(welcome));
    printf("%s", welcome);
    printDivider( getLongestLine(welcome));
}

void shellInit(int argc, char *argv[]) {
    printWelcome();

    if (argc > 1 && IS_VERBOSE(argv[1])) {
        verboseMode = 1;
        runtimePrint("Verbose mode enabled.");
    }
}

void shellRun() {
    char input[256];

    while (1) {
        printf("myShell> ");
        if (!fgets(input, 256, stdin)) continue;
        if (input[strlen(input) - 1] == '\n') input[strlen(input) - 1] = '\0';
        if (strcmp(input, "bye") == 0) break;

        // history
        char *historyCmd = getHistoryCommand(input);
        if (historyCmd) {
            printf("%s\n", historyCmd);
            strcpy(input, historyCmd);
        }
        if (strcmp(input, "history") == 0) {
            printHistory();
            continue;
        }
        addToHistory(input);

        //  normal command
        int cmdCount;
        Command *commands = parseCommand(input, &cmdCount);
        if (commands) {
            executeCommand(commands, cmdCount, verboseMode);
            freeCommands(commands, cmdCount);
        }
    }
}
