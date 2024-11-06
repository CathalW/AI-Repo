#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

//Define number of processes and matrix size
#define NUM_PROCESSES 5 
#define MATRIX_SIZE 5


double determinant_calc(double mat[MATRIX_SIZE][MATRIX_SIZE]) {

    double det;

    //Assign letter to matrix positions for the calculation 
    double a = mat[0][0], b = mat[0][1], c = mat[0][2], d = mat[0][3], e = mat[0][4];
    double f = mat[1][0], g = mat[1][1], h = mat[1][2], i = mat[1][3], j = mat[1][4];
    double k = mat[2][0], l = mat[2][1], m = mat[2][2], n = mat[2][3], o = mat[2][4];
    double p = mat[3][0], q = mat[3][1], r = mat[3][2], s = mat[3][3], t = mat[3][4];
    double u = mat[4][0], v = mat[4][1], w = mat[4][2], x = mat[4][3], y = mat[4][4];
    
        
    //Define the 5 submatrices (4x4) for the 5x5 matrix calculation 
    double a_det = (g * h * i * j * l * m * n * o * q * r * s * t * v * w * x * y);
    double b_det = (f * h * i * j * k * m * n * o * p * r * s * t * u * w * x * y);
    double c_det = (f * g * i * j * k * l * n * o * p * q * s * t * u * v * x * y);
    double d_det = (f * g * h * j * k * l * m * o * p * q * r * t * u * v * w * y);
    double e_det = (f * g * h * i * k * l * m * n * p * q * r * s * u * v * w * x);
    
    //Calculation the determinant
    det = a_det - b_det + c_det - d_det + e_det;

    return det;
}




int main(int argc, char **argv) {

  int myRank,uniSize, ierror;

  //Initialise MPI and other MPI functions 
  ierror=MPI_Init(&argc,&argv);
  ierror=MPI_Comm_size(MPI_COMM_WORLD,&uniSize);
  ierror=MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
  
  
  //Error handling to check if run with correct num of processes - 5
  if (uniSize != NUM_PROCESSES) {
    printf("Run with %d processes. \n", NUM_PROCESSES); 
  }
   
   
  
  //Define the matrix values
  double matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {1, -0.5, -0.3333, -0.25, -0.2},
      {-0.5, 0.3333, -0.25, -0.2, -0.1666},
      {-0.3333, -0.25, 0.2, -0.1666, -0.1428},
      {-0.25, -0.2, -0.1666, 0.1428, -0.125},
      {-0.2, -0.1666, 0.1428, -0.125, 0.1111}
  };
  
  
  //Calculate determiant of the 5x5 matrix
  double det = determinant_calc(matrix);
  
  printf("Determinant value: %f\n", det);
  
  //Finalise MPI
  ierror=MPI_Finalize();
  
  return 0;

}


//COMPILE + RUN COMMANDS 
//=======================================================

//COMPILE - mpicc assignment2_CramersRule.c -o assignment2_CramersRule_compiled
//RUN - mpirun -n 5 ./assignment2_CramersRule_compiled

//=======================================================

