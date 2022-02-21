///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503 - Operating Systems
// Assignment: #3 - Multiple Processes
// Source Code File name: largestInteger-driver.c
// Purpose: Serves as the driver for a program that uses one or more child
//          processes to find the largest integer in each child's assigned
//          portion of an array.  As each child exits, it returns the largest
//          integer that it found.  The parent waits for each child and keeps
//          track of the largest integer value found so far.  When all of the
//          children have exited, the parent prints the result.
// Program's Limitations: Because a child process can only return a nonnegative
//          value up to 255, the values in the integer array range from 1 to 255
//          inclusive
// Development Computer: Dell
// Operating System: Windows 7 and CYGWIN
// Compiler: Gnu gcc
// Build Directions: gcc JacksonMiller.LargestInt.c largestInteger-driver.c
// Operational Status: Fulfills all requirements
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "listSearcher.h"  // This is the header file for student5.c

#define LARGEST_ALLOWABLE_INTEGER 255
#define MIN_CHILDREN 1
#define MAX_CHILDREN 20
#define MIN_ENTRIES 100
#define MAX_ENTRIES 10000
#define FALSE 0
#define TRUE 1

// Function Prototypes
int parseCommandLine(int argc, char *argv[], int *nbrOfChildrenPtr, int *nbrOfListEntriesPtr);
void generateListEntries(int list[], int nbrOfListEntries);
int compareResponsesFromChildren(void);

int main(int argc, char *argv[]) {
    int nbrOfChildren;
    int nbrOfListEntries;
    int commandLineIsValid = FALSE;
    int integerList[MAX_ENTRIES];
    int maxInteger;

    // Parse the command line
    commandLineIsValid = parseCommandLine(argc, argv, &nbrOfChildren, &nbrOfListEntries);

    if (!commandLineIsValid) {
        printf("\nUsage: %s  <#children>  <#entries>\n\n", argv[0]);
        printf("   <#children> - the number of children processes ranging from %d to %d\n",
               MIN_CHILDREN, MAX_CHILDREN);
        printf("   <#entries> - the number of entries in the list ranging from %d to %d\n",
               MIN_ENTRIES, MAX_ENTRIES);
        exit(1);
    }  // End if

    printf("\n(Parent) Starting the maximum integer detection program (Process ID: %d)\n",
           getpid());

    generateListEntries(integerList, nbrOfListEntries);

    printf("\n(Parent) Began searching the list using %d child ", nbrOfChildren);

    if (nbrOfChildren == 1)
        printf("process\n");
    else
        printf("processes\n");

    searchIntegerList(integerList, nbrOfListEntries, nbrOfChildren);

    maxInteger = compareResponsesFromChildren();

    printf("\n(Parent) The maximum integer found by the children was %d\n", maxInteger);

    printf("\n(Parent) Ending the maximum integer detection program \n");

    return 0;
}  // End main

// ################################################################
int parseCommandLine(int argc, char *argv[], int *nbrOfChildrenPtr, int *nbrOfListEntriesPtr) {
    int childrenCount;
    int entryCount;

    if (argc != 3)
        return FALSE;

    childrenCount = atoi(argv[1]);

    if ((childrenCount < MIN_CHILDREN) || (childrenCount > MAX_CHILDREN)) {
        return FALSE;
    }  // End if
    else
        *nbrOfChildrenPtr = childrenCount;

    entryCount = atoi(argv[2]);

    if ((entryCount < MIN_ENTRIES) || (entryCount > MAX_ENTRIES)) {
        return FALSE;
    }  // End if
    else
        *nbrOfListEntriesPtr = entryCount;

    return TRUE;
}  // End parseCommandLine

// ################################################################
void generateListEntries(int list[], int nbrOfListEntries) {
    int i;
    int maxValue = 0;

    srand(time(NULL));  // Seed the random number generator

    // Fill the integer array with randomly-generated positive integers. In
    // doing so, ensure that no value exceeds the largest allowable integer value
    for (i = 0; i < nbrOfListEntries; i++) {
        list[i] = (rand() % LARGEST_ALLOWABLE_INTEGER) + 1;

        // Identify the maximum integer value now to compare later to the
        // maximum integer value found by the children
        if (list[i] > maxValue)
            maxValue = list[i];
    }  // End for

    printf("\n(Parent) Filled the list with %d random entries (Maximum value is %d)\n",
           nbrOfListEntries, maxValue);

}  // End generateListEntries

// Collect child process exit codes (largest integer found by child)
// and return the overall maximum value
int compareResponsesFromChildren(void) {
    pid_t processID;
    int childStatus;
    int tempMaxValue;
    int maxValue;

    processID = wait(&childStatus);

    // Set the initial maximum value
    maxValue = 0;

    while (processID > 0) {
        // If child terminated normally
        if (WIFEXITED(childStatus)) {
            // The exit status is the max value found by the child
            tempMaxValue = WEXITSTATUS(childStatus);
            printf("\n(Parent) Received %d from child process #%d\n",
                   tempMaxValue, processID);
            // Check new value with old maximum, and update if necessary
            if (tempMaxValue > maxValue)
                maxValue = tempMaxValue;

            // Wait for another child
            processID = wait(&childStatus);
        } else {
            fprintf(stderr, "(Parent) Child #%d did not terminate normally\n",
                    processID);
            exit(1);
        }
    }  // End while

    return maxValue;
}  // End compareResponsesFromChildren
