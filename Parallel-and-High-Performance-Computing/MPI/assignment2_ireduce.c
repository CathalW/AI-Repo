#include <stdio.h>
#include <mpi.h>
#include <math.h>

//Define set number of processes
#define NUM_PROCESSES 5

int main(int argc, char **argv) {

    //Declare variables and MPI datatypes
    int myRank, uniSize, ierror;
    MPI_Status status;
    MPI_Request sendRequest, recvRequest;
    MPI_Request reduceRequest;


    //Initialise MPI and other MPI functions 
    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);


    //Error handling to check if run with correct num of processes - 5
    if (uniSize != NUM_PROCESSES) {
        printf("Run with %d processes. \n", NUM_PROCESSES);
    }


    //Declare message, sum and total sum variables
    int message = myRank;
    int sum = 0;
    int total_sum = 0;


    //Calculations for both the left and right neighbours
    int leftNeighbor = (myRank + NUM_PROCESSES - 1) % NUM_PROCESSES;
    int rightNeighbor = (myRank + 1) % NUM_PROCESSES;


    //Loop through processes
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        //Perform 'non-blocking' send to the processes right neighbour
        ierror = MPI_Isend(&message, 1, MPI_INT, rightNeighbor, 0, MPI_COMM_WORLD, &sendRequest);
        
        //Perform 'non-blocking' recieve from processes left neighbour
        ierror = MPI_Irecv(&message, 1, MPI_INT, leftNeighbor, 0, MPI_COMM_WORLD, &recvRequest);

        //Wait for recieve request to complete
        MPI_Wait(&recvRequest, &status);
        
        //Accumulate sum
        sum += message;

        //Wait for send request to complete
        MPI_Wait(&sendRequest, &status);
    }


    //Synchronize processes
    MPI_Barrier(MPI_COMM_WORLD);


    //Calculate final sum 
    sum += myRank;
    
    //Print individual process values - 10 for each
    printf("Rank %d, Sum = %d\n", myRank, sum);

    
    //Perform reduction of sum, store this value in total_sum
    MPI_Ireduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD, &reduceRequest);

    // Wait for the reduction operation to complete
    MPI_Wait(&reduceRequest, &status);

    if (myRank == 0) {
        //Print total sum for all processes
        printf("Rank %d, Total Sum = %d\n", myRank, total_sum);
    }


    //Finalise MPI
    ierror = MPI_Finalize();

    return 0;
}



//COMPILE + RUN COMMANDS 
//=======================================================

//COMPILE - mpicc assignment2_ireduce.c -o assignment2_ireduce_compiled
//RUN - mpirun -n 5 ./assignment2_ireduce_compiled

//=======================================================
