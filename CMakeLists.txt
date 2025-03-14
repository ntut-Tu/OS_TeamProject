cmake_minimum_required(VERSION 3.27)
project(MyShell C)

set(CMAKE_C_STANDARD 11)

add_executable(MyShell main.c
        include/PrintUtils.h
        src/PrintUtils.c
        src/ShellBuilder.c
        src/CommandExecutor.c
        src/ErrorHandler.c
        src/ShellBuilder.c
        include/ErrorHandler.h)
