* Project 1: UNIX Shell
  - This project consists of designing a C program to serve as a shell interface that accepts user commands and then executes each command in a separate process
    - Your implementation will support input and output redirection, as well as pipes as a form of IPC between a pair of commands
  - Completing this project will involve using the UNIX fork(), exec(), wait(), dup2(), and pipe() system calls and can be completed on any Linux, UNIX, or macOS system
    - I. Overview
    - II. Executing command in a child process
    - III. Creating a history feature
    - IV. Redirecting input and output
    - V. Communication via a pipe


test cases:
```bash
echo "hello world" > input.txt
ls -l | grep .c
cat input.txt | grep foo > output.txt
ls > output.txt
cat output.txt
sleep 5 &
history 
```