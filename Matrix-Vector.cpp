#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <fstream>
#define THREADS 20

using namespace std;

//default sample size is 1, unless the user specifies a value
int SAMPLES = 1;

//MatrixA and VectorB will  be the input Matrices - maximum size 2000
double matrixA[10000][10000];
double vectorB[10000];
double transposeB[10000];

//this matrix will be the multiplication of Matrix A and B
double vectorC[10000];

//method header initialization
void transpose(int n);
void populate_matrix(int n);
double serial_multiplication(int n);
double parallel_for_multiplication(int n);
double parallel_for_multiplication_optimized(int n);

//these methods will run the tasks according to the given excersice "Lab 3 and 4.pdf"
void step4_1();
void step4_2();

/*
This is the main method of the program
*/
int main() {

	char method;

	//Enter method that needs to be run
	printf("Enter one of the options given below\n \ts - for serial program \n\tp - for parallel program \n\to - for optimized program\n");
	scanf("%c", &method);

	//Enter sample size for the above selected method
	printf("Enter sample size: ");
	scanf("%d", &SAMPLES);

	switch (method) {
		case 's':
			step4_1();
			break;

		case 'p':
			step4_2();
			break;
		
				
	}
	
	return 0;


}

/*
This method will get the transpose of vectorB
*/
void transpose(int n) {
    int i;
    for(i=0; i<n; i++) {
        transposeB[i] = vectorB[i];
    }
}


/*
This method will populate the matrix with random numbers
*/
void populate_matrix(int n){

	//different seed for each experiment
	srand(time(NULL));

	//populate matrices
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			matrixA[i][j] = rand() % 50;
		}
		vectorB[i] = rand() % 50;
	}

}

/*
This methid will do serial multiplication
*/
double serial_multiplication(int n){

	//required variable initilization
	int i, j;

	//start time from the wall clock
	double startTime = omp_get_wtime();
	
	for (i = 0; i < n; i++){
		vectorC[i] = 0;
		for (j = 0; j < n; j++){
			vectorC[i] +=  matrixA[i][j] * vectorB[j];
		}
	}

	//get the end time from the wall clock
	double endTime = omp_get_wtime();

	//return the execution time
	return endTime - startTime;
}

/*
This method will do parallel for loop multiplication
*/
double parallel_for_multiplication(int n){
		
	//start time from the wall clock
	double startTime = omp_get_wtime();

	//for loop spefication with shared, private variables and number of threads
	#pragma omp parallel for shared(matrixA,  vectorB, vectorC) schedule(static) num_threads(THREADS)
	for (int i = 0; i < n ; i++ ){
		vectorC[i] = 0;
		for (int j = 0; j < n ; j++ ){
			vectorC[i] = vectorC[i] + matrixA[i][j]*vectorB[j];
		}
	}

	//get the end time from wall clock
	double endTime = omp_get_wtime();

	//return the execution time
	return endTime - startTime;

}

void writeMatrixToFile(const char* filename, double matrix[][10000], int rows, int cols) {
    ofstream outFile(filename);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outFile << matrix[i][j] << " ";
        }
        outFile << endl;
    }

    outFile.close();
}

void writeVectorToFile(const char* filename, double vector[], int size) {
    ofstream outFile(filename);

    for (int i = 0; i < size; i++) {
        outFile << vector[i] << endl;
    }

    outFile.close();
}


void step4_1() {
    int n;
    printf("Enter matrix size: ");
    scanf("%d", &n);

    populate_matrix(n);
    // Calculate execution time for serial multiplication
	double time = serial_multiplication(n);
	printf("Serial: Time taken for a %d x %d matrix-vector multiplication is %f s\n", n, n, time);
    // Print matrix A to A.txt
    writeMatrixToFile("A.txt", matrixA, n, n);

    // Print vector B to B.txt
    writeVectorToFile("B.txt", vectorB, n);

    
    
    // Print result vector C to C.txt
    writeVectorToFile("C.txt", vectorC, n);

    
}

void step4_2() {
    int n;
    printf("Enter matrix size: ");
    scanf("%d", &n);

    populate_matrix(n);
	// Calculate execution time for parallel multiplication
    double time = parallel_for_multiplication(n);
     printf("Parallel: Time taken for a %d x %d matrix-vector multiplication is %f s\n", n, n, time);
    // Print matrix A to A.txt
    writeMatrixToFile("A.txt", matrixA, n, n);

    // Print vector B to B.txt
    writeVectorToFile("B.txt", vectorB, n);

    

    // Print result vector C to C.txt
    writeVectorToFile("C.txt", vectorC, n);

   
}


