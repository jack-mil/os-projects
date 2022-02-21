/*
 * Student name: Jackson Miller
 * Course: COSC 3503 - Operating Systems
 * Assignment: #3 - Multiple Processes
 * Source Code File name: max.c
 * Program's Purpose: Provides the functionality that searches the list for the
 *          maximum integer value with the help of 1 or more child
 *          processes
 * Program's Limitations: Because a child process can only return a value up
 *       to 255, the largest possible integer in the list cannot exceed 255
 * Development Computer: Lenovo Thinkpad P73
 * Operating System: Arch Linux
 * Compiler: GCC
 * Build Instructions: See the driver module
 * Program's Operational Status: Complete
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "max.h"

/* Given a list of integers and a range to search,
   return the largest integer in the range  */
static int max_value(int list[], size_t size, int start);

/* Find the largest integer in an array, using multiple child processes
   to split of the load. Segment the array into the specified number of children
   as evenly as possible. Values are returned from each child as exit status code.
   This unfortunately limits the max value to 8 bits (0 - 255).
   I think proper threads are the correct way to go about this.  */
void max_with_children(int list[], int size, int count) {
    int start = 0;
    // Divide the size of array across the children
    int pool_size = size / count;

    // Create the required number of children
    for (size_t i = 0; i < count; i++) {
        pid_t pid = fork();

        if (pid < 0) {  // fork() returns negative if error occurred
            fprintf(stderr, "Fork operation failed");
        } else if (pid == 0) {  // Child process code
            // In each child process, find the largest integer in subgroup
            int max = max_value(list, start, pool_size - 1);
            printf("\n(Child process #%d) Searching indexes %d through %d ...\n",
                   getpid(), start, start + pool_size - 1);
            exit(max);
        } else {  // Parent process code
            // Update starting index for next child
            start += pool_size;
            // For the last child, ensure that all remaining integers are searched
            if (i == (count - 2)) {
                if (start + pool_size < size) {
                    pool_size = size - start;
                }
            }
        }
    }

}  // End max_with_children

/* Given a list of integers and a range to search,
   return the largest integer in the range  */
static int max_value(int list[], size_t size, int start) {
    int max = 0;
    // Loop through the specified range of indices and determine the largest value
    for (size_t i = start; i < start + size; i++) {
        if (list[i] > max) {
            max = list[i];
        }
    }

    return max;
}  // End max_value
