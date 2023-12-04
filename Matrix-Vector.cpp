#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <fstream>

using namespace std;

/*-----------------------GLOBAL VARIABLES DECLARATION --------------------------*/
int THREADS = 32;
int SAMPLES = 1;
int matrixSize = 1;
// Dynamic memory allocation for matrices
double** matrixA;
double* vectorB;
double* vectorC;

/* ------------------------- FUNCTION PROTOTYPES ----------------------------- */
void generate_matrix(int n); // Function to generate a matrix with random values
double serial_multiplication(int n); // This function performs serial matrix-vector multiplication.
double parallel_for_multiplication(int n); // This function performs serial matrix-vector multiplication
void control_menu(); // Function to control the main menu for program selection
void serial_Selection(); // Function to perform operations in the serial selection mode
void parallel_Selection(); // Function to perform operations in the parallel selection mode


/* ------------------------- CODE FOR EACH FUNCTION----------------------------- */

/*
--------This function generates a random matrix (matrixA) and vectors (vectorB, transposeB, vectorC).--------
It uses a different seed for each experiment to ensure different random values.
Parameters:
- n: Size of the square matrix and vectors.
*/
void generate_matrix(int n) {
    // Use a different seed for each experiment to get different random values
    srand(time(NULL));

    // Allocate memory for the matrix and vectors
    matrixA = new double*[n];
    for (int i = 0; i < n; i++) {
        matrixA[i] = new double[n];
    }

    vectorB = new double[n];
    vectorC = new double[n];

    // Generate random values for the matrix and vectors
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Assign random values in the range [-500, 500] to matrix elements
            matrixA[i][j] = rand() % 1000 - 500;
        }
        // Assign random values in the range [-500, 500] to vectorB elements
        vectorB[i] = rand() % 1000 - 500;
    }
}


/*
-------- This function performs serial matrix-vector multiplication. ---------
Parameters:
- n: Size of the square matrix and vectors.
Returns:
- The execution time of the matrix-vector multiplication in seconds.
*/
double serial_multiplication(int n) {
    // Required variable initialization
    int i, j;

    // Start time measurement using the wall clock
    double startTime = omp_get_wtime();

    // Perform serial matrix-vector multiplication
    for (i = 0; i < n; i++) {
        // Initialize the result vector element to zero
        vectorC[i] = 0;

        // Perform the dot product of row i of matrixA and vectorB
        for (j = 0; j < n; j++) {
            vectorC[i] += matrixA[i][j] * vectorB[j];
        }
    }

    // Get the end time from the wall clock
    double endTime = omp_get_wtime();

    // Return the execution time of the matrix-vector multiplication
    return endTime - startTime;
}


/*
--------- This function performs parallel for loop matrix-vector multiplication using OpenMP. -----------
Parameters:
- n: Size of the square matrix and vectors.
Returns:
- The execution time of the parallel matrix-vector multiplication in seconds.
*/
double parallel_for_multiplication(int n) {
    // Start time measurement using the wall clock
    double startTime = omp_get_wtime();

        // Set the number of threads
    omp_set_num_threads(THREADS);

    // Use OpenMP to parallelize the for loop
    #pragma omp parallel for shared(matrixA, vectorB, vectorC) schedule(static) num_threads(THREADS)
    for (int i = 0; i < n; i++) {
        // Initialize the result vector element to zero
        vectorC[i] = 0;

        // Perform the dot product of row i of matrixA and vectorB
        for (int j = 0; j < n; j++) {
            vectorC[i] += matrixA[i][j] * vectorB[j];
        }
    }

    // Get the end time from the wall clock
    double endTime = omp_get_wtime();

    // Return the execution time of the parallel matrix-vector multiplication
    return endTime - startTime;
}


/*
This function runs the serial program for matrix-vector multiplication with user input.
It prompts the user for the matrix size, performs the serial multiplication for a specified number of samples,
calculates the average time, and prints the result.
*/
void serial_Selection() {
    double totalElapsedTime = 0;

    // Perform matrix-vector multiplication for a specified number of samples
    for (int i = 0; i < SAMPLES; i++) {
        // Generate a random matrix and vectors
        generate_matrix(matrixSize);
        
        // Measure the time taken for serial matrix-vector multiplication and accumulate
        totalElapsedTime += serial_multiplication(matrixSize);

        // Free allocated memory outside the loop to prevent unnecessary deallocations
        for (int i = 0; i < matrixSize; i++) {
            delete[] matrixA[i];
        }
        delete[] matrixA;
        delete[] vectorB;
        delete[] vectorC;
    }

    // Calculate the average time taken for matrix-vector multiplication
    double average = totalElapsedTime / SAMPLES;

    // Print the result
    printf("Serial: Time taken for a %d x %d Matrix-Vector multiplication is %f\n", matrixSize, matrixSize, average);
    printf("\n---------------------------------------------\n");
}


/*
-------- This function runs the parallel program for matrix-vector multiplication with user input. ----------
It prompts the user for the matrix size, performs parallel multiplication for a specified number of samples,
calculates the average time, and prints the result.
*/
void parallel_Selection() {
    double totalElapsedTime = 0;

    // Perform parallel matrix-vector multiplication for a specified number of samples
    for (int i = 0; i < SAMPLES; i++) {
        // Generate a random matrix and vectors
        generate_matrix(matrixSize);

        // Measure the time taken for parallel matrix-vector multiplication and accumulate
        totalElapsedTime += parallel_for_multiplication(matrixSize);

        // Free allocated memory outside the loop to prevent unnecessary deallocations
        for (int i = 0; i < matrixSize; i++) {
            delete[] matrixA[i];
        }
        delete[] matrixA;
        delete[] vectorB;
        delete[] vectorC;
    }

    // Calculate the average time taken for parallel matrix-vector multiplication
    double average = totalElapsedTime / SAMPLES;

    // Print the result
    printf("Parallel: Time taken for a %d x %d Matrix-Vector multiplication is %f\n", matrixSize, matrixSize, average);
    printf("\n---------------------------------------------\n");
}


/*-------- Function to control the main menu for program selection ---------*/ 
void control_menu() {
    int selection;

    // Infinite loop to continuously prompt the user for program selection
    while (true) {
        printf("Enter one of the options given below\n \t1 - For serial program \n\t2 - For parallel program\n \t3 - Exit\n");
        scanf("%d", &selection);

        // Exit the loop and program if the user selects option 3
        if (selection == 3) return;

        // Prompt the user to enter the sample size
        printf("Enter sample size: ");
        scanf("%d", &SAMPLES);

        // Matrix Size input
        printf("Enter matrixSize: ");
        if (scanf("%d", &matrixSize) != 1) {
            printf("Invalid input for matrixSize. Exiting...\n");
            return;
        }

         // Prompt the user to input the number of threads
        printf("Enter the number of threads (default is %d): ", THREADS);
        scanf("%d", &THREADS);

        // Perform actions based on user's selection
        switch (selection) {
            case 1:
                // Call the function to perform operations in serial mode
                serial_Selection();
                break;

            case 2:
                // Call the function to perform operations in parallel mode
                parallel_Selection();
                break;
        }
    }
} // End function

/*
----------------------------- MAIN FUNCTION -------------------------
*/
int main() {
    
    control_menu();
    return 0;
}

