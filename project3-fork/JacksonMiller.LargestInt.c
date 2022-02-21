///////////////////////////////////////////////////////////////////
// Student name: Jackson Miller
// Course: COSC 3503 - Operating Systems
// Assignment: #3 - Multiple Processes
// Source Code File name: JacksonMiller.LargestInt.c
// Program's Purpose: Provides the functionality that searches the list for the
//          maximum integer value with the help of 1 or more child
//          processes
// Program's Limitations: Because a child process can only return a value up
//       to 255, the largest possible integer in the list cannot exceed 255
// Development Computer: Lenovo Thinkpad P73
// Operating System: Arch Linux
// Compiler: GCC
// Build Instructions: See the driver module
// Program's Operational Status: Complete
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "listSearcher.h"

/* Given a list of integers and a range to search,
   return the largest integer in the range  */
static int findMaxInteger(int list[], int startingIndex, int numbrOfEntries);

/* Find the largest integer in an array, using multiple child processes
   to split of the load. Segment the array into the specified number of children
   as evenly as possible  */
void searchIntegerList(int list[], int nbrOfListEntries, int childrenCount) {
    int start = 0;
    // Divide the size of array across the children
    int pool_size = nbrOfListEntries / childrenCount;

    // Create the required number of children
    for (size_t i = 0; i < childrenCount; i++) {
        pid_t pid = fork();

        if (pid < 0) {  // fork() returns negative if error occurred
            fprintf(stderr, "Fork operation failed");
        } else if (pid == 0) {  // Child process code
            // In each child process, find the largest integer in subgroup
            int max = findMaxInteger(list, start, pool_size);
            printf("\n(Child process #%d) Searching indexes %d through %d ...\n",
                   getpid(), start, start + pool_size);
            exit(max);
        } else {  // Parent process code
            // Update starting index for next child
            start += pool_size;
            // For the last child, ensure that all remaining integers are searched
            if (i == (childrenCount - 2)) {
                if (start + pool_size < nbrOfListEntries) {
                    pool_size = nbrOfListEntries - start;
                }
            }
        }
    }

}  // End searchIntegerList

/* Given a list of integers and a range to search,
   return the largest integer in the range  */
static int findMaxInteger(int list[], int startingIndex, int nbrOfEntries) {
    int maxValue = 0;
    // Loop through the specified range of indices and determine the largest value
    for (size_t i = startingIndex; i < startingIndex + nbrOfEntries; i++) {
        if (list[i] > maxValue) {
            maxValue = list[i];
        }
    }

    return maxValue;
}  // End findMaxInteger
