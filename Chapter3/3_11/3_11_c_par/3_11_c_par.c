 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
#include <time.h> 
 
void Print_results(char* name, float dot, int p, int my_rank, MPI_Comm comm);
void loop(int i, int my_rank, int local_val, int addon);

int main(void){ 
    int size;
    int k;
    int my_rank; 
    int comm_sz; 
 
    int local_val; 
    int addon = 0;
 
    MPI_Init(NULL,NULL); 

    
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    
    if (my_rank == 0) {
        printf("Please tell me the number of processer, is 2 to the power of:\n"); 
        scanf("%d", &k);
        size = pow(2, k);
    }
    MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    srand(time(NULL) + my_rank); 
    local_val = rand() % 50;  
    
    Print_results("Start local_val", local_val, comm_sz, my_rank, MPI_COMM_WORLD);
    // Print_results("addon", addon, comm_sz, my_rank, MPI_COMM_WORLD);

    int nsend, block, half_block;
    for (int i = 0; i < k; i++){
        nsend = pow(2, i) - 1;
        block = pow(2, i+1);
        half_block = pow(2, i);

        if(my_rank % block == nsend){
            for(int j = 0; j < half_block; j++){
                MPI_Send(&local_val, 1, MPI_INT, my_rank + 1 + j, 0, MPI_COMM_WORLD);    
            }
        }
        else if(my_rank % block > nsend){
            MPI_Recv(&addon, 1, MPI_INT, my_rank - my_rank % block + nsend, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   
            local_val += addon;
        }    

        // Print_results("local_val", local_val, comm_sz, my_rank, MPI_COMM_WORLD);
        // Print_results("addon", addon, comm_sz, my_rank, MPI_COMM_WORLD);
    }

    Print_results("Final local_val", local_val, comm_sz, my_rank, MPI_COMM_WORLD);

    MPI_Finalize(); 
 
    return 0; 
}


void Print_results(char* name, float dot, int p, int my_rank, MPI_Comm comm){
    int q;
    float* temp;
    int status;

    if (my_rank == 0) {
        status = 0;
    }
    MPI_Bcast(&status, 1, MPI_INT, 0, comm);

    if (my_rank == 0) {
        temp = malloc(p*sizeof(float));
        MPI_Gather(&dot, 1, MPI_FLOAT, temp, 1, MPI_FLOAT, 0, comm);

        for (q = 0; q < p; q++)
            printf("Proc %d > %s = %f\n", q, name, temp[q]);
        printf("\n");
        free(temp);
    } else {
      MPI_Gather(&dot, 1, MPI_FLOAT, temp, 1, MPI_FLOAT, 0, comm);
}
}