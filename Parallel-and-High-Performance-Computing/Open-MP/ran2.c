/*
* This code generates random numbers between 0 and 1.
* This code is not threadsafe. Below is an example of * how to call ran2.
*
*   double x;
*   long seed;
*   x = ran2(&seed);
*/

//Author -> Cathal Wallace - 22239408

//Import Libraries
#include <stdio.h>
#include <omp.h> 
#include <math.h>
#include <time.h>


#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran2(long *idum) {
  int j;
  long k;
  static long idum2=123456789; 
  static long iy=0;
  static long iv[NTAB];
  float temp;
  
  //Implement Thread Safety using OMP
  #pragma omp threadprivate(idum2, iy, iv)
  #pragma omp critical
  {
  
  if (*idum <= 0) {
     if (-(*idum) < 1) *idum=1; 
     else *idum = -(*idum);
     idum2=(*idum);

    
    
     for (j=NTAB+7;j>=0;j--) {
        k=(*idum)/IQ1; 
       *idum=IA1*(*idum-k*IQ1)-k*IR1; 
        if (*idum < 0) *idum += IM1; 
        if (j < NTAB) iv[j] = *idum;
     }
     iy=iv[0];
    }
    
    }

  

  k=(*idum)/IQ1; 
 *idum=IA1*(*idum-k*IQ1)-k*IR1;
  if (*idum < 0) *idum += IM1; 
  k=idum2/IQ2; 
  idum2=IA2*(idum2-k*IQ2)-k*IR2;

  if (idum2 < 0) idum2 += IM2; j=iy/NDIV;
  iy=iv[j]-idum2;
  iv[j] = *idum;
  if (iy < 1) iy += IMM1;
  if ((temp=AM*iy) > RNMX) 
    return RNMX; 
    else return temp;
  
}



int main() {
  //Initialise variables
  double x;
  double y;
  double distance;
  int i;
  int nd = 0;
  int nt = 0;
  double pie;
  int tid;
  int nthreads;
  long seed;
  
  
  
  //Specify the paralell region, set number of threads and sort variables as shared or private
  #pragma omp parallel private(tid, x, y, distance, i, pie, nd, nt, seed) shared(nthreads) num_threads (4)
  {
    
    
    //Declare the thread id and number of threads 
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    
   
    
    for (i = 0; i < 100000; ++i) {
      
      //Calculate 2 random numbers - x and y
      x = ran2(&seed);
      y = ran2(&seed);


      //Calculate the distance
      distance = sqrt((x * x) + (y * y));
  
  
      //Calculate pie
      //----------------
      //Number of points below 1
      if (distance <= 1.0) {
        //Use 'critical' directive to only allow access to 1 thread at a time
        #pragma omp critical
        {
          nd++;
        }
      }
  
        //Total number of points
        nt++;
      
  
      //Pie calculation
      pie = (4.0 * ((double)nd/nt));  
      
    }
  
  //Print thread id, x and y values and pie vaue  
  printf("Thread ID: %d", tid);  
  printf("X: %.16lf    Y: %.16lf  ", x, y);
  printf("Pie %f\n", pie);
    
  
  }
  
  
  return 0;
}


//Compile and Run files
//==========================
//COMPILE:
//gcc -o ran2_compiled ran2.c -lm -fopenmp

//RUN:
//./ran2_compiled
//==========================



//Q1 -> PART 6 ANSWER
//==========================
//I find that changing the amount of threads (and iterations) changes the result slightly.
//When I have more threads (more than 1, and more importantly, more iterations), the time it
//takes to run the programme take longer, however when I have less threads (and iterations 
//it is shorter. I have found that the ideal number of threads for around 100000 iterations is 4.
//When the programme is not paralellised, results vary more. Naturally the more iterations done the 
//closer the values of pie are to 3.14.
//==========================



//References:
//==========================
// - Lecture notes
// - Lecture Tutorial Videos/Notes
// - https://learn.microsoft.com/en-us/cpp/parallel/openmp/reference/openmp-directives?view=msvc-170#atomic
//==========================







