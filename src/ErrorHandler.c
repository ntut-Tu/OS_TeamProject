//
// Created by quinndu0 on 2025/3/14.
//
#include "../include/ErrorHandler.h"
#include "../include/PrintUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void checkError() {
    if (errno) {
//        printf("[Error]: %s\n", strerror(errno));
        printDivider(50);
        runtimePrint("Error: %d", errno);
        printDivider(50);
        errno = 0;
        exit(1);
    }
}
