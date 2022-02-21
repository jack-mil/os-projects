/*
 * Student name: Jackson Miller
 * Course: COSC 3503 - Operating Systems
 * Assignment: #3 - Multiple Processes
 * Source Code File name: max.h
 * Program's Purpose: Contains the prototype for the function that 
 *          implements the search of the entry list for the
 *          max integer value using 1 or more children processes
 * Program's Limitations: See the comments in the driver source code file
 * Development Computer: Lenovo Thinkpad P73 
 * Operating System: Arch Linux
 * Compiler: GCC
 * Build Instructions: See the driver module
 * Program's Operational Status: Fulfills the requirements
*/

#ifndef LIST_SEARCHER_HEADER
#define LIST_SEARCHER_HEADER

/* Find the largest integer in an array, using multiple child processes
   to split of the load. Segment the array into the specified number of children
   as evenly as possible. Values are returned from each child as exit status code.
   This unfortunately limits the max value to 8 bits (0 - 255).
   I think proper threads are the correct way to go about this.  */
void max_with_children(int list[], int size, int count);

#endif

