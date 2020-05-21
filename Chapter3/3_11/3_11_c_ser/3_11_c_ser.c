#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
 
int main(void) { 
 
    int k;
    printf("Enter the power of 2, number of array elements: "); 
    scanf("%d", &k);

    int size = pow(2, k);
    int a[size]; 
    int sum[size]; 
 
    for (int i = 0; i < size; i++){ 
        a[i] = rand() % 50; 
    } 

    int presum = 0;
    
    for (int i = 0; i < size; i++) { 
        presum += a[i];
        sum[i] = presum; 
    } 
 
    for (int i = 0; i < size; i++){ 
        printf("a[%d]=%d \t SUM[%d]=%3d\n", i, a[i], i, sum[i]); 
    } 
 
 
    return 0; 
}