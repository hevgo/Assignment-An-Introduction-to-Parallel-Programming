// This code for Ex3_coding assignment
// Yuxuan Jing
// April 202

// number in circle = 0;
// for (toss = 0; toss < number of tosses; toss++) {
// x = random double between − 1 and 1;
// y = random double between − 1 and 1;
// distance squared = x∗x + y∗y;
// if (distance squared <= 1) number in circle++;
// }
// pi estimate = 4∗number in circle/((double) number of tosses);


#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
#include <time.h>

#define NTOSS 1E11

double pi_estimate(long long int seed, long long int n_toss);

int main(){
	clock_t start = clock();

	printf ("Ex3_PA_ser:\n");

	long long int ntoss = NTOSS;

	printf ("Number of toss = %lld:\n", ntoss);
	//for (long int i = 0; i < 10; i++){
		pi_estimate(0, ntoss);		
	//}
	printf ("end calc\n");
	clock_t end = clock();
	printf ("total time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC);  
	return 0;
}

double pi_estimate(long long int seed, long long int n_toss){
	long long int n_in, n_out, n_tot;
	double x, y, r;

	n_in = 0;
	n_out = 0;
	n_tot = 0;

	/* Intializes random number generator */
   	srand(seed + 256);

	for (long long int toss = 0; toss < n_toss; toss++){
	    x = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
	    y = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
	    r = x*x + y*y;
	    r<=1 ? n_in++ : n_out++;
	    n_tot++;
	    //printf ( "%f\t%f\t%f\t%d\t%d\t%d\n", x, y, r, n_in, n_out, n_tot);
	}

	double pi_est = 4.0 * n_in / n_tot;
	printf ( "With %lld seed, %lld tosses, The estimated PI = %f\n", seed + 256, n_toss, pi_est);
	return pi_est;
}