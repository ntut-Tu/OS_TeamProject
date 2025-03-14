//
// Created by quinndu0 on 2025/3/14.
//
#include "../include/PrintUtils.h"

void runtimePrint(const char *message, ...) {
    printf("\033[33m[Runtime Debug]: ");
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    printf("\033[0m\n");
}

int getLongestLine(const char *str) {
    int maxLen = 0, currentLen = 0;
    int inEscape = 0;

    while (*str) {
        if (*str == '\033') {
            inEscape = 1;
        } else if (inEscape && *str == 'm') {
            inEscape = 0;
        } else if (!inEscape) {
            if (*str == '\n') {
                if (currentLen > maxLen) maxLen = currentLen;
                currentLen = 0;
            } else {
                currentLen++;
            }
        }
        str++;
    }
    if (currentLen > maxLen) maxLen = currentLen;
    return maxLen;
}

void printDivider(int n) {
    for(int i = 0; i < n; i++) {
        printf("=");
    }
    printf("\n");
}