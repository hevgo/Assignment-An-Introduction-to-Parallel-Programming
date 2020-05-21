
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void   Read_vector(char* prompt, float local_v[], int local_n, int n, int my_rank, MPI_Comm comm);
void   Print_results(float dot, int p, int my_rank, MPI_Comm comm);
float  Serial_dot(float x[], float y[], int m, float scalar);
float  Parallel_dot(float local_x[], float local_y[], int local_n,
          MPI_Comm comm, float local_scalar);
    
int main(void) {
    float*   local_x;
    float*   local_y;
    int      n;
    int      local_n;
    float    scalar, local_scalar;
    float    dot;
    int      p;
    int      my_rank;

    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    MPI_Comm_rank(comm, &my_rank);

    if (my_rank == 0) {
        printf("Enter the order of the vectors\n");
        scanf("%d", &n);
        printf("Enter the scalar to multiply\n");
        scanf("%f", &scalar);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_n = n/p;
    local_scalar = scalar;

    local_x = malloc(local_n*sizeof(float));
    local_y = malloc(local_n*sizeof(float));
    Read_vector("the first vector", local_x, local_n, n, my_rank, comm);
    Read_vector("the second vector", local_y, local_n, n, my_rank, comm);

    dot = Parallel_dot(local_x, local_y, local_n, comm, local_scalar);

    Print_results(dot, p, my_rank, comm);

    free(local_x);
    free(local_y);
    MPI_Finalize();

    return 0;
}
  

void Read_vector(char* prompt, float local_v[], int local_n, int n, int my_rank, MPI_Comm comm){
    int i;
    float* temp;

    if (my_rank == 0) {
        temp = malloc(n*sizeof(float));
        printf("Enter %s\n", prompt);
        for (i = 0; i < n; i++)
            scanf("%f", &temp[i]);
        MPI_Scatter(temp, local_n, MPI_FLOAT, local_v, local_n, MPI_FLOAT,
           0, comm);
        free(temp);
    } else {
        MPI_Scatter(temp, local_n, MPI_FLOAT, local_v, local_n, MPI_FLOAT,
           0, comm);
    }
}  /* Read_vector */

void Print_results(float dot, int p, int my_rank, MPI_Comm comm){
   int q;
   float* temp;

   if (my_rank == 0) {
      temp = malloc(p*sizeof(float));
      MPI_Gather(&dot, 1, MPI_FLOAT, temp, 1, MPI_FLOAT, 0, comm);

      for (q = 0; q < p; q++)
         printf("Proc %d > dot = %f\n", q, temp[q]);
      free(temp);
   } else {
      MPI_Gather(&dot, 1, MPI_FLOAT, temp, 1, MPI_FLOAT, 0, comm);
   }
}  /* Print_results */


float Serial_dot(float x[], float y[], int n, float scalar){

    int    i; 
    float  sum = 0.0;

    for (i = 0; i < n; i++)
        sum += scalar*x[i]*y[i];
    return sum;
} /* Serial_dot */

float Parallel_dot( float local_x[], float local_y[], intlocal_n, MPI_Comm comm, float local_scalar){

    float  local_dot;
    float  dot = 0.0;

    local_dot = Serial_dot(local_x, local_y, local_n, local_scalar);
    MPI_Allreduce(&local_dot, &dot, 1, MPI_FLOAT, MPI_SUM, comm);
    return dot;
} /* Parallel_dot */