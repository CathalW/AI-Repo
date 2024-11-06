//PART 2

//Author -> Cathal Wallace - 22239408

//Import Libraries
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <stdbool.h> 
#include <omp.h> 

//Define fixed array size (100x100)
#define ROWS 100
#define COLS 100
//Define fixed values for array
#define first_row 0
#define last_row 100
#define first_col 100
#define last_col 100



int main() {
    
    //Define variables
    int i;
    int j;
    int iterations = 25000;
    
    
    //Create array 'w' of size 100, 100 (ROWS, COLUMNS)
    float w[ROWS][COLS];
    float w_new[ROWS][COLS];
    
    //Variables for updating 'w_new' array based on comparison with 'w' array
    float maximum_difference = 0;
    float threshold = 0.001;
    bool stop = false; 
  
    
    //Set thread number
    omp_set_num_threads(4);
    
    //Record start time
    double start = omp_get_wtime();
    
    
    //Implement paralell region, make loop variables private 
    #pragma omp parallel private(i, j)
    {
    
    //Set the first row to 0
    for (i = 0; i < ROWS; i++) {
        w[0][i] = first_row;
    }
    
    //Set the last row to 100    
    for (i = 0; i < ROWS; i++) {
        w[ROWS - 1][i] = last_row;
    }
    
    //Set the first and last columns to 100
    for (i = 1; i < COLS - 1; i++) {
        w[i][0] = first_col;
        w[i][COLS - 1] = last_col;
    }
    
    //All other elements set to 75 
    for (i = 1; i < ROWS - 1; i++) {
        for (j = 1; j < COLS - 1; j++) {
            w[i][j] = 75;
        }
    }
    
    
    //Set 'w_new' array to the same values as 'w' array
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            w_new[i][j] = w[i][j];
        }
    }
    
    
    
    
    //Perform provided calculation on the 'w_new' array
    while (iterations-- > 0 && !stop) {
        maximum_difference = 0;
        
        for (i = 1; i < ROWS - 1; i++) {
            for (j = 1; j < COLS - 1; j++) {
                w_new[i][j] = (w_new[i+1][j] + w_new[i-1][j] + w_new[i][j+1] + w_new[i][j-1]) / 4;
                
                //Compare the difference between arrays w and w_new
                float difference = fabs(w_new[i][j] - w[i][j]);
                
                //Update the max difference 
                if (difference > maximum_difference) {
                    maximum_difference = difference;
                }
            }
        }
        
        
        //Check if the stopping condition is met, stop if true
        if (maximum_difference < threshold) {
            stop = true;
        }
        
    }
    
    }
    
    
    //Record end time and calculate total time
    double end = omp_get_wtime();
    double total_time = end - start;
    
    
    //Print the total time it takes to run the program (in seconds)
    printf("%lf\n", total_time);
    
    
  
  return 0;
}



//How to run and compile
//===========================
//COMPILE:
// gcc -o part2_compiled part2_steady_state.c

//RUN:
// ./part2_compiled
//===========================



//References
//===========================
//Lecture notes and tutorial videos
//===========================



//Q2 P4
//Performance Times (over 3 runs each)
//===========================
// 4 Threads - 0.332206, 0.138538, 0.12511
// Average - 0.198618
// Relative Speedup - 25.6926 (Runs 25.6926 times faster than 1 thread)
// Absolute Speedup - 22.1618 (Runs 22.1618 times faster than unparalell version)
// Efficiency - 6.4231
//
//
// 2 Threads - 0.336893, 0.40101, 1.100666
// Average - 1.838569
// Relative Speedup - 2.7759 (Runs 2.7759 times faster than 1 thread)
// Absolute Speedup -  2.3941 (Runs 2.3941 times faster than unparalell version)
// Efficiency - 1.3879
//
// 1 Thread - 5.223815, 5.03996, 5.045322
// Average - 5.103032
// Absolute Speedup -  0.8625 (Runs 0.8625 times slower than unparalell version)
//
//
// Not Parelellised - 4.385547, 4.38662, 4.433051
// Average - 4.401739
//===========================



