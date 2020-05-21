#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include "timer.h"

const int MAX_THREADS = 1024;

int thread_count;
double a;
double b;
int n;
int flag;
double sum;
double block;
pthread_mutex_t mutex;
sem_t sems;

void* Thread_sum_busy(void* rank);
void* Thread_sum_mut(void* rank);
void* Thread_sum_sem(void* rank);

double f(double x);
/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial(int n);

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;

   /* Get number of threads and number of terms from command line */
   Get_args(argc, argv);
   block = (b-a)/n;
   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 


//busy

   GET_TIME(start);
   sum = 0.0;
   flag = 0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Thread_sum_busy, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   sum = sum - f(a)/2 + f(b)/2;
   sum *= (b-a)/n;
   printf("With n = %d terms,\n\n", n);
   printf("Multi-threaded estimate of busy  = %.1f\n", sum);
   printf("   Elapsed time = %e seconds\n", elapsed);

//mut

   pthread_mutex_init(&mutex, NULL);

   GET_TIME(start);
   sum = 0.0;
   flag = 0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Thread_sum_mut, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   sum = sum - f(a)/2 + f(b)/2;
   sum *= (b-a)/n;
   printf("Multi-threaded estimate of mutex  = %.1f\n", sum);
   printf("   Elapsed time = %e seconds\n", elapsed);

   pthread_mutex_destroy(&mutex);

//sem
sem_init(&sems, 0, 0);
sem_post(&sems);
   GET_TIME(start);
   sum = 0.0;
   flag = 0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Thread_sum_sem, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   sum = sum - f(a)/2 + f(b)/2;
   sum *= (b-a)/n;
   printf("Multi-threaded estimate of semaphore  = %.1f\n", sum);
   printf("   Elapsed time = %e seconds\n", elapsed);

sem_destroy(&sems);

//serial

   GET_TIME(start);
   sum = Serial(n);
   GET_TIME(finish);
   elapsed = finish - start;
   printf("Single-threaded estimate = %.1f\n", sum);
   printf("   Elapsed time = %e seconds\n", elapsed);
   
   free(thread_handles);
   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:       Thread_sum
 * Purpose:        Add in the terms computed by the thread running this 
 * In arg:         rank
 * Ret val:        ignored
 * Globals in:     n, thread_count
 * Global in/out:  sum 
 */
void* Thread_sum_busy(void* rank) {
   long my_rank = (long) rank;
   int i;
   int my_n = n/thread_count;
   int my_first_i = my_n*my_rank;
   int my_last_i = my_first_i + my_n;

   for (i = my_first_i; i < my_last_i; i++) {
      while (flag != my_rank);
      sum += f(i * block + a);  
      flag = (flag+1) % thread_count;
   }

   return NULL;
}  /* Thread_sum */

/*------------------------------------------------------------------
 * Function:   Serial_pi
 * Purpose:    Estimate pi using 1 thread
 * In arg:     n
 * Return val: Estimate of pi using n terms of Maclaurin series
 */

void* Thread_sum_mut(void* rank){

   long my_rank = (long) rank;
   int i;
   double my_n = n/thread_count;
   int my_first_i = my_n*my_rank;
   int my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   for (i = my_first_i; i < my_last_i; i++) {
      my_sum += f(i * block + a);
   }
   pthread_mutex_lock(&mutex);
   sum += my_sum;
   pthread_mutex_unlock(&mutex);

   return NULL;

}


void* Thread_sum_sem(void* rank){

   long my_rank = (long) rank;
   int i;
   double my_n = n/thread_count;
   int my_first_i = my_n*my_rank;
   int my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   for (i = my_first_i; i < my_last_i; i++) {
      my_sum += f(i * block + a);
   }


   sem_wait(&sems);
   sum += my_sum;
   sem_post(&sems);

   return NULL;
}


double Serial(int n) {
   double sum = 0.0;
   int i;
   double block = (b-a)/n;

   //printf("a = %f, b = %f, n = %d, block = %f\n", a, b, n, block);

   for (i = 0; i < n; i++) {
      sum += f(i * block + a);
   }

   sum = sum - f(a)/2 + f(b)/2;
   sum *= (b-a)/n;
   
   return sum;

}  /* Serial_pi */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */

void Get_args(int argc, char* argv[]) {
   if (argc != 5) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   a = strtoll(argv[2], NULL, 10);
   b = strtoll(argv[3], NULL, 10);
   if (a >= b) Usage(argv[0]);
   n = strtoll(argv[4], NULL, 10);
   if (n <= 0) Usage(argv[0]);
}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <a> <b> <n>\n", prog_name);
   fprintf(stderr, "   a, b are the left and right bond of a range and should be a < b\n");
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  /* Usage */

double f(double x){
   return x*x;
}