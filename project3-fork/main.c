/*
* Student name: Jackson Miller
* Course: COSC 3503 - Operating Systems
* Assignment: #3 - Multiple Processes
* Source Code File name: main.c
* Purpose: Serves as the driver for a program that uses one or more child
*          processes to find the largest integer in each child's assigned
*          portion of an array.  As each child exits, it returns the largest
*          integer that it found.  The parent waits for each child and keeps
*          track of the largest integer value found so far.  When all of the
*          children have exited, the parent prints the result.
* Program's Limitations: Because a child process can only return a nonnegative
*          value up to 255, the values in the integer array range from 1 to 255
*          inclusive
* Development Computer: Lenovo Thinkpad P73
* Operating System: Arch Linux
* Compiler: GCC
* Build Directions: gcc main.c max.c
* Operational Status: Fulfills all requirements
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "max.h"  // This is the header file for student5.c

#define LARGEST_ALLOWABLE_INTEGER 255
#define MIN_CHILDREN 1
#define MAX_CHILDREN 20
#define MIN_ENTRIES 100
#define MAX_ENTRIES 10000
#define FALSE 0
#define TRUE 1

// Function Prototypes
int parse_args(int argc, char *argv[], int *child_count_ptr, int *array_size_ptr);
void rand_array(int list[], size_t size, int max_value);
int get_result_from_children(void);

int main(int argc, char *argv[]) {
    int int_array[MAX_ENTRIES];

    // Parse the command line
    int child_count, array_size;
    int valid = parse_args(argc, argv, &child_count, &array_size);

    // Print help message in case of invalid arguments
    if (!valid) {
        printf("\nUsage: %s  [#children]  [#entries]\n\n", argv[0]);
        printf("   [#children] - the number of children processes ranging from %d to %d\n",
               MIN_CHILDREN, MAX_CHILDREN);
        printf("   [#entries] - the number of entries in the list ranging from %d to %d\n",
               MIN_ENTRIES, MAX_ENTRIES);
        exit(1);
    }

    printf("\n(Parent) Starting the maximum integer detection program (Process ID: %d)\n",
           getpid());

    // Fill array with random integers
    rand_array(int_array, array_size, LARGEST_ALLOWABLE_INTEGER);

    printf("\n(Parent) Began searching the list using %d child ", child_count);

    if (child_count == 1)
        printf("process\n");
    else
        printf("processes\n");

    // Spawn child processes to divide and conquer the array search
    max_with_children(int_array, array_size, child_count);

    // Wait for children to return their max values, and get an overall result
    int max_value = get_result_from_children();

    printf("\n(Parent) The maximum integer found by the children was %d\n", max_value);

    printf("\n(Parent) Ending the maximum integer detection program \n");

    return 0;
}  // End main

/* Parse and validate command line arguments. Limits set with
   MIN_CHILDREN, MAX_CHILDREN, MIN_ENTRIES, MAX_ENTRIES definitions
   Returns true on valid arguments. Parsed values put in *child_count_ptr and *array_size_ptr  */
int parse_args(int argc, char *argv[], int *child_count_ptr, int *array_size_ptr) {
    int count;
    int size;

    // Two (2) positional arguments
    if (argc != 3)
        return FALSE;

    // First argument is amount of children
    count = atoi(argv[1]);

    if ((count < MIN_CHILDREN) || (count > MAX_CHILDREN)) {
        return FALSE;
    } else
        *child_count_ptr = count;

    // Second argument is size of array
    size = atoi(argv[2]);

    if ((size < MIN_ENTRIES) || (size > MAX_ENTRIES)) {
        return FALSE;
    } else
        *array_size_ptr = size;

    return TRUE;
}  // End parse_args

/* Fill an array with random integers <= max_value */
void rand_array(int list[], size_t size, int max_value) {
    int max = 0;

    srand(time(NULL));  // Seed the random number generator

    // Fill the integer array with randomly-generated positive integers. In
    // doing so, ensure that no value exceeds the largest allowable integer value
    for (size_t i = 0; i < size; i++) {
        list[i] = (rand() % max_value) + 1;

        // Identify the maximum integer value now to compare later to the
        // maximum integer value found by the children
        if (list[i] > max)
            max = list[i];
    }

    printf("\n(Parent) Filled the list with %ld random entries (Maximum value is %d)\n",
           size, max);

}  // End rand_array

/* Collect child process exit codes (largest integer found by child)
   and return the overall maximum value */
int get_result_from_children(void) {
    int child_status;
    pid_t child_pid = wait(&child_status);

    // Set the initial maximum value
    int total_max = 0;

    int temp;
    while (child_pid > 0) {
        // If child terminated normally
        if (WIFEXITED(child_status)) {
            // The exit status is the max value found by the child
            temp = WEXITSTATUS(child_status);
            printf("\n(Parent) Received %d from child process #%d\n",
                   temp, child_pid);
            // Check new value with old maximum, and update if necessary
            if (temp > total_max)
                total_max = temp;

            // Wait for another child
            child_pid = wait(&child_status);
        } else {
            fprintf(stderr, "(Parent) Child #%d did not terminate normally\n",
                    child_pid);
            exit(1);
        }
    }

    return total_max;
}  // End get_result_from_children
