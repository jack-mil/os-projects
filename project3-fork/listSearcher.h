///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503 - Operating Systems
// Assignment: #3 - Multiple Processes
// Source Code File name: listSearcher.h
// Program's Purpose: Contains the prototype for the function that 
//          implements the search of the entry list for the
//          max integer value using 1 or more children processes
// Program's Limitations: See the comments in the driver source code file
// Development Computer: Dell 
// Operating System: Windows 7 using CYGWIN
// Compiler: Gnu C
// Build Instructions: See the driver module
// Program's Operational Status: Fulfills the requirements
///////////////////////////////////////////////////////////////////

#ifndef LIST_SEARCHER_HEADER
#define LIST_SEARCHER_HEADER

/* Find the largest integer in an array, using multiple child processes
   to split of the load. Segment the array into the specified number of children
   as evenly as possible  */
void searchIntegerList(int list[], int nbrOfListEntries, int childrenCount);

#endif

