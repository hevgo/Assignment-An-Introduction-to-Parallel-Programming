 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <mpi.h> 

#define NTOSS 1E11

//double pi_estimate(long long int seed, long long int n_toss);

int main(void){ 
    
    clock_t start = clock();
    clock_t end = clock();

    long long int ntoss = NTOSS;

    int comm_sz; 
    int local_rank; 
    double global_pi;

    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank); 

    long long int local_n_toss;
    long long int local_seed;
    double local_pi;

    // if (local_rank == 0){
    //     printf ("before compute\n");
    //     end = clock();
    //     printf ("total time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC);    
    // } 

    local_n_toss = ntoss / comm_sz;
    local_seed = local_rank + 256;


    long long int n_in;
    double x, y, r;

    n_in = 0;

    /* Intializes random number generator */
    srand(local_seed);

    for (long long int toss = 0; toss < local_n_toss; toss++){
        // if (toss%(local_n_toss/10) == 0 && local_rank == 0){
        //     printf("finish %lld\n", toss);
        // }
        x = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
        y = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
        r = x*x + y*y;
        if (r <+ 1){
            n_in++;
        }
    }

    local_pi = 4.0 * n_in / local_n_toss;

    //local_pi = pi_estimate(local_seed, local_n_toss);

    //printf("node %d: pi = %f, seed = %ld\n", local_rank, local_pi, local_seed);

    // if (local_rank == 0){
    //     printf ("before reduce\n");
    //     end = clock();
    //     printf ("total time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC);    
    // } 


    MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (local_rank == 0) {
        global_pi /= comm_sz; 
        printf("\nNumber of nodes = %d\n", comm_sz);
        printf("Final result estimated pi = %f\n", global_pi);
    } 

    if (local_rank == 0){
        printf ("end calc\n");
        end = clock();
        printf ("total time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC);    
    } 
    
    MPI_Finalize(); 
    
    return 0; 
}

