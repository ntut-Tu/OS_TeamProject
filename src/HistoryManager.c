//
// Created by nudle on 2025/3/15.
//
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/HistoryManager.h"

History history = { .count = 0, .start = 0 };

void addToHistory(const char *input) {
    if (strlen(input) == 0) return;

    if (history.count == MAX_HISTORY) {
        free(history.commands[history.start]);
        history.start = (history.start + 1) % MAX_HISTORY;
    } else {
        history.count++;
    }

    int index = (history.start + history.count - 1) % MAX_HISTORY;
    history.commands[index] = strdup(input);
}

void printHistory() {
    for (int i = 0; i < history.count; i++) {
        int index = (history.start + i) % MAX_HISTORY;
        printf("%d %s\n", i + 1, history.commands[index]);
    }
}

char *getHistoryCommand(const char *input) {
    if (strcmp(input, "!!") == 0) {
        if (history.count == 0) {
            printf("No commands in history.\n");
            return NULL;
        }
        return history.commands[(history.start + history.count - 1) % MAX_HISTORY];
    }
    if (input[0] == '!' && isdigit(input[1])) {
        int num = atoi(input + 1);
        if (num < 1 || num > history.count) {
            printf("No such command in history.\n");
            return NULL;
        }
        return history.commands[(history.start + num - 1) % MAX_HISTORY];
    }
    return NULL;
}
