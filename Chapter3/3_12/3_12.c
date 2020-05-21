#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
 
int main(void){ 
 
    int comm_sz; 
    int my_rank; 
 
    int *local_array=NULL; 
    int local_value; 
    int sum; 
    int temp_val; 
    int source; 
    int dest; 
 
    int i; 
 
    MPI_Init(NULL,NULL); 
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); 
    
    //distribute the local_values
    if (my_rank==0){
        local_array = (int*)malloc(comm_sz*sizeof(int)); 
        for (i=0;i<comm_sz;i++){ 
            *(local_array+i)=(rand()%50); 
            printf("D[%d]=%d\n",i,*(local_array+i)); 
        } 
        MPI_Scatter(local_array,1,MPI_INT,&local_value,1,MPI_INT,0,MPI_COMM_WORLD); 
    } else { 
        MPI_Scatter(local_array,1,MPI_INT,&local_value,1,MPI_INT,0,MPI_COMM_WORLD); 
    } 
 
     
    //implement ring-pass algorithm
    if (my_rank == 0){ 
        source = comm_sz - 1; 
        dest = 1; 
    } else if (my_rank == (comm_sz-1)){ 
        source = (comm_sz-2); 
        dest = 0; 
    } else{ 
        source = (my_rank-1); 
        dest = (my_rank+1); 
    } 
    
    //using MPI_Sendrecv_replace
    sum = temp_val = local_value; 
    
    for (i = 0; i < comm_sz - 1; i++) { 
        MPI_Sendrecv_replace(&temp_val, 1, MPI_INT, dest, 0, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        sum += temp_val; 
    } 

    // print out sums 
    MPI_Gather(&sum,1,MPI_INT,local_array,1,MPI_INT,0,MPI_COMM_WORLD); 
 
    if (my_rank == 0){ 
        for (i = 0; i < comm_sz; i++){ 
            printf("ALLREDUCE_SUM[%d]=%d\n",i,*(local_array+i)); 
        } 
    } 
 
    if (local_array) free(local_array); 
 
    MPI_Finalize(); 
 
    return 0; 
}