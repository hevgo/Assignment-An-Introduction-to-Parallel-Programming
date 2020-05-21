#include <stdio.h> 
#include <stdlib.h> 
 
int main(int argc, char** argv){ 
 
    int n, i, psum, sum; 
    int *arrayval; 
    int *arraysum;
    
    sum = 0;

    printf("Enter number of array elements: "); 
    scanf("%d", &n); 
 
    arrayval = (int *)malloc(n*sizeof(int)); 
    arraysum = (int *)malloc(n*sizeof(int)); 
 
    if (arrayval==NULL) 
        return 1; 
 
    for (i = 0; i < n; i++){
        psum = rand()%50;
        sum += psum;
        arrayval[i] = psum;
        arraysum[i] = sum;
        printf("x[%2d] = %3d, presum[%2d] = %3d\n", i, psum, i, sum);
    }
 
    if (arrayval) {
        free(arrayval); 
        free(arraysum);  
    }
    return 0; 
}