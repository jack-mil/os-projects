///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503
// Assignment: #4 - POSIX Signals
// Source Code File Name: SignalHandlerDriver.c
// Program's Purpose: Provides the driver that tests the generation, detection,
//    and handling of various POSIX signals
// Assumptions:  The program contains statements that intentionally result in
//    the generation of signals such as alarm notification, floating point
//    error, segmentation fault, and program abort.  These signals,
//    when not explicitly handled, cause the program to abnormally terminate
// Program's Limitations: None known
//
// Development Computer: Dell
// Operating System: Windows 7 using CYGWIN
// Compiler: Gnu gcc
// Build Directions: gcc program9-driver.c
//                     OR
//                   gcc program9-driver.c student9.c
// Program's Operational Status: Fulfills the requirements
///////////////////////////////////////////////////////////////////

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "signal-handlers.h"

// Constants
#define FALSE 0
#define TRUE 1
#define MAX_SIGNAL_COUNT 4

// Function Prototypes
void checkCommandLine(int argc);
void startChildrenProcesses(void);
void performSignalTests(int argc, char *argv[]);
void promptForUserID(void);

// Program entry point
int main(int argc, char *argv[]) {
    pid_t childPID;

    // Register the signal handlers before doing any other processing

    registerSignalHandlers();

    printf("\n*** START OF PROGRAM (Process ID: %d, User ID: %d) ***\n\n",
           getpid(), getuid());

    checkCommandLine(argc);

    // Set the seed for the random number generator
    srand(time(0));

    startChildrenProcesses();

    performSignalTests(argc, argv);

    // Wait for any children that are still around
    do {
        childPID = wait(NULL);
    } while (childPID > 0);

    printf("\n\n*** END OF PROGRAM ***\n");

    return 0;
}  // End main

// Verify 2 positional command line arguments
void checkCommandLine(int argc) {
    sigset_t signalSet;

    // Check the number of arguments on the command line
    if (argc != 3) {
        fprintf(stderr, "\nUsage: file  <dividend>  <divisor>\n");
        fprintf(stderr, "       <dividend>     : integer value\n");
        fprintf(stderr, "       <divisor>      : integer value\n\n");
    }  // End if

    fprintf(stderr, "\n### FIRST ASSERT TEST: ARGUMENT COUNT ###\n\n");

    assert(argc == 3);

    fprintf(stderr, "\nPassed the first assert test.\n\n");

}  // End checkCommandLine

// Start a random number or children with a random lifetime.
// When they terminate, the SIGCHLD handler will catch the signal
void startChildrenProcesses(void) {
    const int MAX_CHILDREN = 5;
    const int MIN_SECONDS = 5;
    const int MAX_SECONDS = 15;
    pid_t childPID;
    int nbrOfChildren;
    int secondsToSleep;
    int i;

    nbrOfChildren = rand() % MAX_CHILDREN;

    // Zero or more children may get started

    for (i = 1; i <= nbrOfChildren; i++) {
        secondsToSleep = (rand() % MAX_SECONDS) + 1;
        if (secondsToSleep < MIN_SECONDS)
            secondsToSleep = MIN_SECONDS;

        childPID = fork();
        if (childPID < 0) {
            fprintf(stderr, "Error creating a child process\n");
            exit(1);
        }                        // End if
        else if (childPID == 0)  // This is the child
        {
            sleep(secondsToSleep);
            exit(0);
        }     // End else if
        else  // This is the parent
        {
            fprintf(stderr, "\nStarted child process #%d for %d seconds\n",
                    childPID, secondsToSleep);
        }  // End else
    }      // End for

}  // End startChildrenProcesses

// Perform a series of steps that trigger signals. Signal handlers are assigned in other module
void performSignalTests(int argc, char *argv[]) {
    sigset_t signalSet;
    int signalCount;
    int signalNbr;
    int status;
    int dividend;
    int divisor;
    int quotient;
    int *wordPtr = (int *)999999;

    fprintf(stderr, "\n### SIGALRM TEST ###\n\n");

    promptForUserID();

    fprintf(stderr, "\n### SIGFPE TEST ###\n\n");

    // Perform calculations
    dividend = atoi(argv[1]);
    divisor = atoi(argv[2]);
    quotient = dividend / divisor;  // When the divisor is zero, this line should cause
                                    // a floating point error

    // Display results of calculations
    fprintf(stderr, "\n%d / %d = %d\n", dividend, divisor, quotient);

    fprintf(stderr, "\n### SECOND ASSERT TEST: QUOTIENT > 0 ###\n\n");

    assert(quotient > 0);
    fprintf(stderr, "\nPassed the second assert test.\n\n");

    fprintf(stderr, "\n### ANY SIGNAL PAUSE TEST FOR PID #%d (BE SURE TO TRY SIGINT and SIGTERM) ###\n\n", getpid());

    // Wait for a signal from outside of the process
    fprintf(stderr, "\nPausing for a signal...\n");

    pause();  // This function causes the process to pause for any unblocked signal

    if (rand() % 2 == 0)  // This condition is true only 50% of the time
    {
        fprintf(stderr, "\n### SIGSEGV TEST ###\n\n");

        fprintf(stderr, "\nEnter a word: ");

        scanf("%s", wordPtr);  // This statement should cause a segmentation fault

        fprintf(stderr, "\nYou entered %s\n", wordPtr);
    }  // End if

    fprintf(stderr, "\n### SIGNAL SUSPEND TEST ###\n\n");

    fprintf(stderr, "\nUnblocking all signals and suspending until some signal is received ...\n\n");

    // Block no signals
    sigemptyset(&signalSet);
    sigsuspend(&signalSet);

    fprintf(stderr, "\nAn unblocked signal has been received\n\n");

    fprintf(stderr, "\n### SIGNAL WAIT TEST ###\n\n");

    fprintf(stderr, "\nBlocking all signals...\n");

    // Block all signals
    sigfillset(&signalSet);
    sigprocmask(SIG_BLOCK, &signalSet, NULL);

    fprintf(stderr, "\nWaiting until any blocked signal is received (PID #%d)...\n",
            getpid());

    signalCount = 0;
    while (signalCount < MAX_SIGNAL_COUNT) {
        // signalNbr = sigwait(&signalSet);          // Default version
        status = sigwait(&signalSet, &signalNbr);  // POSIX version

        if (status == -1) {
            perror("Failed to wait using sigwait");
            exit(1);
        }  // End if
        else
            fprintf(stderr, "\nReceived blocked signal #%d\n", signalNbr);
        signalCount++;
    }  // End while

    fprintf(stderr, "\nFinished waiting for blocked signals\n\n");

}  // End performSignalTests

// #############################################################################
void promptForUserID(void) {
    int userID = getuid();
    int userInput;

    // Prompt for and read in the user ID; time out in five seconds if
    // no entry occurs

    alarm(5);
    fprintf(stderr, "Enter your numeric user ID: ");
    scanf("%d", &userInput);
    alarm(0);

    if (userInput == userID) {
        fprintf(stderr, "\nYou have successfully signed into the signal test program\n");
        return;
    }  // End if
    else {
        fprintf(stderr, "\nYour entry is not correct\n");
        exit(1);
    }  // End else

}  // End promptForUserID
