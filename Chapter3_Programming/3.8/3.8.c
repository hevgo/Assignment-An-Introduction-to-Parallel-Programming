 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <mpi.h> 
#include <math.h>

void Initial(int local_rank, int n, double* local_arr);
void Print_arr(int round, int local_rank, int n, double* local_arr);
void Merge(int local_rank, int local_n, double* local_arr, double* added_arr);
void merge_sort(int i, int j, double* a, double* aux);
void merge_node(int n, double* a, double* aux);

int main(void){ 
    
    clock_t start = clock();
    clock_t last = clock();
    clock_t end = clock();

    int comm_sz; 
    int local_rank;

    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank); 

    int local_n;
    int global_n;
    double* local_arr;
    double* added_arr;
    global_n = 1.28E8;
    local_n = global_n / comm_sz;
//read n    
//     if (local_rank == 0) {
//         printf("Enter the order of the vectors\n");
//         scanf("%d", &local_n);
//     }
//     global_n = comm_sz * local_n;
// distribute n
    MPI_Bcast(&local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(local_rank ==0){
        printf("\n###############\n\nNumber of nodes = %d:\n", comm_sz);
    }

// creat array    
    local_arr = malloc(global_n*sizeof(double));
    added_arr = malloc(global_n*sizeof(double));
    Initial(local_rank, local_n, local_arr);
// print rank0
    if(local_rank ==0){
        printf("\nbefore merge:\n");
    }
    Print_arr(-1, local_rank, local_n, local_arr);
    if (local_rank == 0){
        printf ("end initial\n");
        end = clock();
        printf ("total time = %fs \t gap time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC, (double)(end - last) / CLOCKS_PER_SEC);   
        last = clock();
    }
// local sort
    merge_sort(0, local_n-1, local_arr, added_arr);

// print rank0
    if(local_rank ==0){
        printf("\nafter merge:\n");    
    }
    Print_arr(-1, local_rank, local_n, local_arr);
    if (local_rank == 0){
        printf ("end local sort\n");
        end = clock();
        printf ("total time = %fs \t gap time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC, (double)(end - last) / CLOCKS_PER_SEC);   
        last = clock();
    }

// tree sort logn
    int n_round = ceil(log10(comm_sz)/log10(2));
    int gap, block;

    for (int round = 0; round < n_round; round++){
        gap = pow(2, round);
        block = 2 * gap;
        // local_arr = realloc(local_arr, 2.0*local_n*sizeof(double));
        // added_arr = realloc(added_arr, 2.0*local_n*sizeof(double));

        if ( (local_rank % gap) == 0) {
            if ( (local_rank % block) == 0){
                if ( (local_rank + gap) < comm_sz){   
                    MPI_Recv(local_arr + local_n, local_n, MPI_DOUBLE, local_rank + gap, round, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
                    //printf("recv : %d from %d\n", local_rank, local_rank + gap);
                    merge_node(local_n, local_arr, added_arr);

                }
            }
            else{
                MPI_Send(local_arr, local_n, MPI_DOUBLE, local_rank - gap, round, MPI_COMM_WORLD);
                //printf("send : %d to %d\n", local_rank, local_rank - gap);
            }
        } 
        local_n *= 2;
        if (local_rank == 0){
            printf ("end round %d\n", round);
            last = end;
            end = clock();
            printf ("total time = %fs \t gap time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC, (double)(end - last) / CLOCKS_PER_SEC);   
            last = clock();
        }
        Print_arr(round, local_rank, local_n, local_arr); 
    }

// print rank0
    double* final_arr;
    final_arr = local_arr + (local_n - global_n);

    if (local_rank == 0) {
        printf("\nfinal sorted array:\n");
        // for (int i = local_n - global_n; i < local_n; i++){
        //     printf("%f||", local_arr[i]);
        // }
        // printf("\n");
    }

    Print_arr(-1, local_rank, global_n, final_arr);
    
    if (local_rank == 0){
        printf ("end calc\n");
        last = end;
        end = clock();
        printf ("total time = %fs \t gap time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC, (double)(end - last) / CLOCKS_PER_SEC);   
        last = clock();
    }

    free(local_arr);
    free(added_arr);

     
    

    MPI_Finalize(); 

    return 0; 
}

void Initial(int local_rank, int n, double* local_arr){
    srand(local_rank + 256);
    for(int i = 0; i < n; i++){
        local_arr[i] = (double)rand()/RAND_MAX*5.0;;
    }
}

void Print_arr(int round, int local_rank, int n, double* local_arr){
    // if (local_rank == 0) {
    //     printf("\nround %d:\n", round);
    //     if (n <= 40){
    //         for (int i = 0; i < n; i++){
    //             printf("%f||", local_arr[i]);
    //         }
    //         printf("\n");    
    //     }
    //     else{
    //         printf ("start 20 element:\n");
    //         for (int i = 0; i < 20; i++){
    //             printf("%f||", local_arr[i]);
    //         }
    //         printf("\n");    
    //         printf ("end 20 element:\n");
    //         for (int i = n-20; i < n; i++){
    //             printf("%f||", local_arr[i]);
    //         }
    //         printf("\n");    
    //     }

        
    // }
}

void Merge(int local_rank, int local_n, double* local_arr, double* added_arr){
    // if (local_rank == 0) {
    //     printf("before merge:\n");
    //     for (int i = 0; i < local_n; i++){
    //         printf("%f||", local_arr[i]);
    //     }
    //     printf("\n");
    //     for (int i = 0; i < local_n; i++){
    //         printf("%f||", added_arr[i]);
    //     }
    //     printf("\n");
    // }

    for (int i = 0; i < local_n; i++){
        local_arr[i + local_n] = added_arr[i];
    }

    // if (local_rank == 0) {
    //     printf("after merge:\n");
    //     for (int i = 0; i < 2*local_n; i++){
    //         printf("%f||", local_arr[i]);
    //     }
    //     printf("\n");
    // }
}

void merge_sort(int i, int j, double* a, double* aux) {
    if (j <= i) {
        return;     // the subsection is empty or a single element
    }
    int mid = (i + j) / 2;

    // left sub-array is a[i .. mid]
    // right sub-array is a[mid + 1 .. j]
    
    merge_sort(i, mid, a, aux);     // sort the left sub-array recursively
    merge_sort(mid + 1, j, a, aux);     // sort the right sub-array recursively

    int pointer_left = i;       // pointer_left points to the beginning of the left sub-array
    int pointer_right = mid + 1;        // pointer_right points to the beginning of the right sub-array
    int k;      // k is the loop counter

    // we loop from i to j to fill each element of the final merged array
    for (k = i; k <= j; k++) {
        if (pointer_left == mid + 1) {      // left pointer has reached the limit
            aux[k] = a[pointer_right];
            pointer_right++;
        } else if (pointer_right == j + 1) {        // right pointer has reached the limit
            aux[k] = a[pointer_left];
            pointer_left++;
        } else if (a[pointer_left] < a[pointer_right]) {        // pointer left points to smaller element
            aux[k] = a[pointer_left];
            pointer_left++;
        } else {        // pointer right points to smaller element
            aux[k] = a[pointer_right];
            pointer_right++;
        }
    }

    for (k = i; k <= j; k++) {      // copy the elements from aux[] to a[]
        a[k] = aux[k];
    }
}

void merge_node(int n, double* a, double* aux){
    int ll = 0;
    int lr = n-1;
    int rl = n;
    int rr = 2*n-1;
    int paux = 0;
    while(ll < lr && rl < rr){
        if (a[ll] < a[rl]){
            aux[paux] = a[ll];
            ll++;
            paux++;
        }
        else{
            aux[paux] = a[rl];
            rl++;
            paux++;
        }
    }

    if (ll == lr){
        for(int i = rl; i <= rr; i++){
            aux[paux] = a[i];
            paux++;
        }
    }
    else{
        for(int i = ll; i <= lr; i++){
            aux[paux] = a[i];
            paux++;
        }
    }
    double* temp;
    temp = aux;
    aux = a;
    a = temp;
}