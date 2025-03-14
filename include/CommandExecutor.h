//
// Created by quinndu0 on 2025/3/14.
//

#ifndef MYSHELL_COMMANDEXECUTOR_H
#define MYSHELL_COMMANDEXECUTOR_H

typedef struct {
    char *cmd;
    char **args;
    char *inputFile;    // <
    char *outputFile;   // >
    int appendMode;  // >>
    int background;  // &
} Command;

void executeCommand(Command *commands, int cmdCount, int verboseMode);
Command *parseCommand(const char *input, int *cmdCount);
void freeCommands(Command *commands, int cmdCount);

#endif //MYSHELL_COMMANDEXECUTOR_H
