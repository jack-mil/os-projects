///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503
// Assignment: Demonstration
// Source Code File Name: semaphoreDemo.c
// Program's Purpose: Demonstrates how to use a semaphore to synchronize
//    the passage of cars over a one-lane bridge on a road that runs east
//    and west
// Program's Limitations: None known
// Development Computer: Dell
// Operating System: Windows 7 using CYGWIN
// Compiler: Gnu gcc
// Build Instructions: gcc semaphoreDemo.c -lpthread
// Program's Operation Status: All requirements have been met
///////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NUMBER_OF_CARS 6
#define MAX_CAR_ARRIVAL_INTERVAL 5
#define TIME_NEEDED_TO_CROSS_BRIDGE 4

// Global constants
int HEADING_EAST = 0;
int HEADING_WEST = 1;

// Global variables
sem_t semaphore;
int carCreationCounter = 0;
int carCompletionCounter = 0;

// Function Prototypes
void manageBridgeTraffic(void);
void *createCar(void *directionPtr);

int main(void) {
    manageBridgeTraffic();

    return 0;
}  // End main

void manageBridgeTraffic(void) {
    pthread_t carsHeadingEast;
    pthread_t carsHeadingWest;
    int i;
    int status;

    printf("\n");
    printf("Simulation of Traffic Over a One-Lane Bridge\n");
    printf("--------------------------------------------\n\n");

    srand(time(NULL));

    // This semaphore is shared among threads and initialized to 1
    sem_init(&semaphore, 0, 1);

    // Start up the two threads
    status = pthread_create(&carsHeadingEast, NULL, createCar,
                            &HEADING_EAST);
    if (status != 0) {
        printf("Failed to create thread for cars heading east\n");
        return;
    }  // End if

    status = pthread_create(&carsHeadingWest, NULL, createCar,
                            &HEADING_WEST);
    if (status != 0) {
        printf("Failed to create thread for cars heading west\n");
        return;
    }  // End if

    // Continually check to see if the maximum number of cars has
    // crossed the bridge
    while (carCompletionCounter < MAX_NUMBER_OF_CARS) {
    };

    printf("\nThe maximum number of simulated cars has been reached\n\n");

    // Stop the threads
    status = pthread_cancel(carsHeadingEast);
    if (status != 0) {
        printf("Failed to cancel thread for cars heading east\n");
        return;
    }  // End if

    status = pthread_cancel(carsHeadingWest);
    if (status != 0) {
        printf("Failed to cancel thread for cars heading west\n");
        return;
    }  // End if

    printf("\nThe car creation threads have been terminated\n");

    // Wait for the two threads
    status = pthread_join(carsHeadingEast, NULL);
    if (status != 0)
        printf("Failed to join thread for cars heading east\n");

    status = pthread_join(carsHeadingWest, NULL);
    if (status != 0)
        printf("Failed to join thread for cars heading west\n");

    printf("\n*** End of Bridge Traffic Simulation ***\n");

}  // manageBridgeTraffic

// ****************************************************************
void *createCar(void *directionPtr) {
    int carDirection;
    int carArrivalInterval;
    int carID;

    // Cast to int pointer and dereference
    carDirection = *(int *)directionPtr;

    // loop forever
    while (1) {
        carArrivalInterval = rand() & MAX_CAR_ARRIVAL_INTERVAL + 1;
        sleep(carArrivalInterval);
        carCreationCounter++;
        carID = carCreationCounter;

        if (carDirection == HEADING_EAST)
            printf("---> Car #%d arrived going east and is waiting to cross the bridge\n\n", carID);
        else if (carDirection == HEADING_WEST)
            printf("Car #%d arrived going west and is waiting to cross the bridge <---\n\n", carID);
        else
            printf("Car #%d arrived from an unknown direction\n", carID);

        sem_wait(&semaphore);  // WAIT

        if (carDirection == HEADING_EAST)
            printf("\n---> Car #%d is crossing the bridge --->\n\n", carID);
        else if (carDirection == HEADING_WEST)
            printf("\n<--- Car #%d is crossing the bridge <---\n\n", carID);
        else
            printf("\nERROR Car #%d is crossing the bridge ERROR\n\n", carID);

        sleep(TIME_NEEDED_TO_CROSS_BRIDGE);

        sem_post(&semaphore);  // SIGNAL

        printf("\nCar #%d has crossed the bridge\n\n", carID);

        carCompletionCounter++;

    }  // End while

}  // End createCar
