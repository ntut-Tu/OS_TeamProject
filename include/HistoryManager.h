//
// Created by nudle on 2025/3/15.
//

#ifndef MYSHELL_HISTORYMANAGER_H
#define MYSHELL_HISTORYMANAGER_H

#define MAX_HISTORY 10

typedef struct {
    char *commands[MAX_HISTORY];
    int count;
    int start; // circular buffer start index
} History;

void addToHistory(const char *input);
void printHistory();
char *getHistoryCommand(const char *input);

#endif //MYSHELL_HISTORYMANAGER_H
