 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <mpi.h> 
 
int main(void){ 
 
    int local_rank; 
    int comm_sz; 
    int local_rand; 
    int sum = 0; 
 
    MPI_Init(NULL,NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank); 
 
    srand(time(NULL) + local_rank); 
    local_rand = rand() % 50; 
    
    //using a one-by-one pass algorithm
    if (local_rank == 0) { 
        sum += local_rand; 
        printf("x[%d] = %d, sum = %d\n", local_rank, local_rand, sum); 
        MPI_Send(&sum, 1, MPI_INT, local_rank + 1, 0, MPI_COMM_WORLD); 
    } else { 
        MPI_Recv(&sum, 1, MPI_INT, local_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        sum += local_rand; 
        printf("x[%d] = %d, sum = %d\n", local_rank, local_rand, sum); 
        if (local_rank < (comm_sz-1)) 
            MPI_Send(&sum, 1, MPI_INT, local_rank + 1, 0, MPI_COMM_WORLD); 
    } 
 
    MPI_Finalize(); 
    return 0; 
}