/* Student name: Jackson Miller
 * Course: COSC 3503
 * Assignment: #5 - Traffic Simulation
 * Source Code File name: JacksonMiller5.c
 * Program's Purpose: Uses a semaphore and threads
 *    to simulate traffic synchronization at an n-way stop intersection
 * Development Computer: Lenovo P73
 * Operating System: Arch Linux
 * Compiler: GCC
 * Build Instructions: Make file provided
 * Program's Operation Status: Complete
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

// Global variables
sem_t semaphore;
int carCreationCounter = 0;
int carCompletionCounter = 0;

// Function Prototypes
int parseCommandLine(int argc, char *argv[], int *nbrOfStopSignsPtr,
                     int *maximumCarsPtr, int *maximumTraversalTimePtr);
void displayUsageMessage(char *);
void manageTrafficSimulation(int nbrOfStopSigns, int maximumCars,
                             int maximumTraversalTime);
void *createCar(void *maxTraversalTimePtr);

int main(int argc, char *argv[]) {
    int nbrOfStopSigns;
    int maximumCars;
    int maximumTraversalTime;

    if (!parseCommandLine(argc, argv, &nbrOfStopSigns,
                          &maximumCars, &maximumTraversalTime))
        return 1;

    manageTrafficSimulation(nbrOfStopSigns, maximumCars, maximumTraversalTime);

    return 0;
}  // End main

// ****************************************************************
int parseCommandLine(int argc, char *argv[], int *nbrOfStopSignsPtr,
                     int *maximumCarsPtr, int *maximumTraversalTimePtr) {
    int nbrOfStopSigns;
    int maximumCars;
    int maximumTraversalTime;

    if (argc != 4) {
        displayUsageMessage(argv[0]);
        return FALSE;
    } else {
        nbrOfStopSigns = atoi(argv[1]);
        maximumCars = atoi(argv[2]);
        maximumTraversalTime = atoi(argv[3]);
    }  // End else

    if ((nbrOfStopSigns <= 0) || (maximumCars <= 0) ||
        (maximumTraversalTime <= 0)) {
        displayUsageMessage(argv[0]);
        return FALSE;
    }  // End else
    else {
        *nbrOfStopSignsPtr = nbrOfStopSigns;
        *maximumCarsPtr = maximumCars;
        *maximumTraversalTimePtr = maximumTraversalTime;
        return TRUE;
    }  // End else

}  // End parseCommandLine

// ****************************************************************
void displayUsageMessage(char *name) {
    printf("\nUsage: %s <#stop signs> <#cars> <traversal time>\n\n", name);
    printf("   <#stop signs>    - Number of stop signs at the intersection\n");
    printf("   <#cars>          - Maximum number of cars to traverse the intersection\n");
    printf("   <traversal time> - Maximum time (in secs) to traverse the intersection\n");
    printf("\n");
    printf("Note: All values should be greater than zero (0)\n");
}  // End displayUsageMessage

// *****  MAKE NO SOURCE CODE CHANGES ABOVE THIS LINE *****

// ****************************************************************
void manageTrafficSimulation(int nbrOfStopSigns, int maximumCars,
                             int maximumTraversalTime) {
    pthread_t dirs[nbrOfStopSigns];
    // Print heading
    printf("Traffic Simulation at a %d-Way Stop Intersection\n", nbrOfStopSigns);
    printf("--------------------------------------------\n\n");

    // Initilize semaphore shared among this process' threads
    sem_init(&semaphore, 0, 1);

    srand(time(NULL));  // Seed RNG

    int status;
    for (size_t i = 0; i < nbrOfStopSigns; i++) {
        status = pthread_create(&dirs[i], NULL, createCar, &maximumTraversalTime);
        if (status != 0) {
            printf("Failed to create thread for direction %lu\n", i);
            return;
        }
    }

    // Main thread wait till simulation finished
    while (carCompletionCounter < maximumCars) {
    };

    printf("\nMaximum number of simulated cars have passed intersection\n\n");

    for (size_t i = 0; i < nbrOfStopSigns; i++) {
        status = pthread_cancel(dirs[i]);
        if (status != 0) {
            printf("Failed to cancel thread for direction %lu\n", i);
            return;
        }
    }

    printf("\nTerminated all of the car creation threads\n\n");

    for (size_t i = 0; i < nbrOfStopSigns; i++) {
        status = pthread_join(dirs[i], NULL);
        if (status != 0) {
            printf("Failed to join thread for direction %lu\n", i);
        }
    }

    printf("\n*** END OF TRAFFIC SIMULATION ***\n");

}  // manageTrafficSimulation

// ****************************************************************
void *createCar(void *maxTraversalTimePtr) {
    int traversalTime;
    int carID;
    int nbrOfWaitingCars;
    // Cast to int pointer and dereference
    int maxTraversalTime = *(int *)maxTraversalTimePtr;

    // loop forever
    while (1) {
        // Wait for a random time to simulate arrival time
        traversalTime = rand() % maxTraversalTime + 1;
        sleep(traversalTime);

        // A new car has arrived with ID
        carCreationCounter++;
        carID = carCreationCounter;
        nbrOfWaitingCars = carCreationCounter - carCompletionCounter - 1;

        printf("Car #%d has arrived and is waiting with %d previous cars (STOP)\n", carID, nbrOfWaitingCars);

        // Critical section (only one car at a time can cross)
        sem_wait(&semaphore);
        printf("\nCar #%d is going through the intersection -->\n\n", carID);
        sleep(traversalTime);
        sem_post(&semaphore);

        // Exit section
        printf("\n--> Car #%d traversed the intersection in %d seconds", carID, traversalTime);
        carCompletionCounter++;
    }

}  // End createCar
