/*
 * Student name: jackmil
 * Course: COSC 3503 - Operating Systems
 * Assignment: #5 - Signal Handlers
 * Source Code File name: handlers.c
 * Program's Purpose: Defines signal handler functions for various signals
 *      caught in the driver code
 */

#include "handlers.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void catchABRT(int sigNbr) {
    char msg[] = "\n(SIGABRT) Assertion failed. Program terminating\n";
    write(STDERR_FILENO, msg, strlen(msg));
    exit(1);
}
void catchALRM(int sigNbr) {
    char msg[] = "\n(SIGALRM) User input timed out. Program terminating\n";
    write(STDERR_FILENO, msg, strlen(msg));
    exit(1);
}
void catchCHLD(int sigNbr) {
    pid_t pid = wait(NULL);
    char *msg;
    asprintf(&msg, "\n(SIGCHLD) Child PID #%d sent wait and terminated\n", pid);
    write(STDERR_FILENO, msg, strlen(msg));
    free(msg);
    return;
}
void catchFPE(int sigNbr) {
    char msg[] = "\n(SIGFPE) Critical arithmetic error occurred. Program terminating\n";
    write(STDERR_FILENO, msg, strlen(msg));
    exit(1);
}
void catchINT(int sigNbr) {
    char msg[] = "\n(SIGINT) <CTRL-C> detected. Program continuing\n";
    write(STDERR_FILENO, msg, strlen(msg));
    return;
}
void catchSEGV(int sigNbr) {
    char msg[] = "\n(SIGSEGV) Critical Memory access error occurred. Program terminating\n";
    write(STDERR_FILENO, msg, strlen(msg));
    exit(1);
}
void catchTERM(int sigNbr) {
    char msg[] = "\n(SIGTERM) Received request to terminal. Shutting down now\n";
    write(STDERR_FILENO, msg, strlen(msg));
    exit(1);
}

void registerSignalHandlers(void) {
    signal(SIGABRT, catchABRT);
    signal(SIGALRM, catchALRM);
    signal(SIGCHLD, catchCHLD);
    signal(SIGFPE, catchFPE);
    signal(SIGINT, catchINT);
    signal(SIGSEGV, catchSEGV);
    signal(SIGTERM, catchTERM);
}
