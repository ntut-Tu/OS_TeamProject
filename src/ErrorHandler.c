//
// Created by quinndu0 on 2025/3/14.
//
#include "../include/ErrorHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void fileCheckError() {
    if (errno) {
        printf("[Error]: %s\n", strerror(errno));
        errno = 0;
        exit(1);
    }
}
