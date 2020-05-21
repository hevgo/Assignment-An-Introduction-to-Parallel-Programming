/* File:  
 *    omp_tokenize.c
 *
 * Purpose:
 *    Try to use threads to tokenize text input.  Illustrate problems 
 *    with function that isn't threadsafe.  This program has a serious
 *    bug.
 *
 * Compile:
 *    gcc -g -Wall -fopenmp -o omp_tokenize omp_tokenize.c
 * Usage:
 *    omp_tokenize <thread_count> < <input>
 *
 * Input:
 *    Lines of text
 * (Desired) Output:
 *    For each line of input:
 *       the line read by the program, and the tokens identified by 
 *       strtok
 *
 * Algorithm:
 *    For each line of input, next thread reads the line and
 *    "tokenizes" it.
 *
 * IPP:   Section 5.10 (pp. 256 and ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <5.6.queue.c>

const int MAX_LINES = 1000;
const int MAX_LINE = 80;
const int MAXCHAR = 1000;

void Usage(char* prog_name);
void Get_filenames(char* lines[], int* line_count_p);
int Read_file(char* filename, struct queue_s* q_p);
void Tokenize(char* lines[], int line_count, int thread_count); 

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	char* filenames[1000];
   	int file_count, i;
	struct queue_s* q_p = Allocate_queue();	
	
	int thread_count;
	int n_prod, n_cons;

	if (argc != 4) Usage(argv[0]);
	thread_count = strtol(argv[1], NULL, 10);
	n_prod = strtol(argv[2], NULL, 10);
	n_cons = strtol(argv[3], NULL, 10);

	if (thread_count != (n_prod+n_cons)) Usage(argv[0]);

	printf("Enter filenames\n");
   	Get_filenames(filenames, &file_count);
   	
   	for (i = 0; i < file_count; i++){
   		size_t len = strlen(filenames[i]);
  		if (len > 0 && filenames[i][len-1] == '\n') {
    		filenames[i][--len] = '\0';
  		}
   	}


   	//int read_count = -1;
   	int my_rank; 
   	int bool_done = 0;

#  	pragma omp parallel num_threads(thread_count) default(none) private(my_rank) shared(filenames, q_p, n_prod, file_count, bool_done)
   	{

   		my_rank = omp_get_thread_num();

   		if (my_rank < n_prod){
   			for (int k = 0; k < file_count; k++){
   				if (my_rank == (k % n_prod) ){
	   				FILE *fp;
				    char str[MAXCHAR]; 
				    char file_temp[100] = "./input/";
				    strcat(file_temp, filenames[k]);
				    strcat(file_temp, ".txt");

					fp = fopen(file_temp, "r");
					if (fp == NULL){
					    printf("Could not open file %s",file_temp);
					}

					//int count = 0;
					while (fgets(str, MAXCHAR, fp) != NULL){
						char* queue_msg = malloc(strlen(str)*sizeof(char));
						strcpy(queue_msg, str);
		
						omp_set_lock(&q_p->lock);
						printf("thread %d,\t Enqueue %s", my_rank, queue_msg);
						Enqueue(q_p, queue_msg);
						omp_unset_lock(&q_p->lock);
					    //printf("%d, %s", count, str);
					}
					fclose(fp);	
   				}
   				
   			}
			// while(read_count++ < file_count - 1){
		 //   		FILE *fp;
			//     char str[MAXCHAR]; 
			//     char file_temp[100] = "./input/";
			//     strcat(file_temp, filenames[read_count]);
			//     strcat(file_temp, ".txt");

			// 	fp = fopen(file_temp, "r");
			// 	if (fp == NULL){
			// 	    printf("Could not open file %s",file_temp);
			// 	}

			// 	//int count = 0;
			// 	while (fgets(str, MAXCHAR, fp) != NULL){
			// 		char* queue_msg = malloc(strlen(str)*sizeof(char));
			// 		strcpy(queue_msg, str);
			// 		omp_set_lock(&q_p->lock);
			// 		printf("thread %d,\t Enqueue %s", my_rank, queue_msg);
			// 		Enqueue(q_p, queue_msg);
			// 		omp_unset_lock(&q_p->lock);
			// 	    //printf("%d, %s", count, str);
			// 	}
			// 	fclose(fp);
		 //   	}
		   	bool_done = 1; 
   		}
   		else{
   			while(bool_done != 1){
	   			while (q_p->enqueued > q_p->dequeued){
					int j = 0;
					char *my_token, *saveptr;

					omp_set_lock(&q_p->lock);
					char* my_queue = Dequeue_from(q_p);
					omp_unset_lock(&q_p->lock);

					my_token = strtok_r(my_queue, " \t\n", &saveptr);
			     	while ( my_token != NULL ) {
			        	printf("thread %d,\t token %d = %s\n", my_rank, j, my_token);
			        	my_token = strtok_r(NULL, " \t\n", &saveptr);
			        	j++;
			     	}//while token
					
			   	}//while queue	
   			}//bool_done

   			while (q_p->enqueued > q_p->dequeued){
				int j = 0;
				char *my_token, *saveptr;
				printf("thread %d,\t Dequeue", my_rank);
				char* my_queue = Dequeue_from(q_p);

				my_token = strtok_r(my_queue, " \t\n", &saveptr);
		     	while ( my_token != NULL ) {
		        	printf("thread %d,\t token %d = %s\n", my_rank, j, my_token);
		        	my_token = strtok_r(NULL, " \t\n", &saveptr);
		        	j++;
		     	}//while token
				
		   	}//while queue	
   		}

   	}//omp 

   	Print_queue(q_p);
   	
   	return 0;

}//main


void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads> <number of producer> <number of consumer>\n", prog_name);
   fprintf(stderr, "usage: the sum of n_prod and n_cons should be equal to n_thread\n");

   exit(0);
}  /* Usage */


void Get_filenames(char* lines[], int* line_count_p) {
   char* line = malloc(MAX_LINE*sizeof(char));
   int i = 0;
   char* fg_rv;

   fg_rv = fgets(line, MAX_LINE, stdin);
   while (fg_rv != NULL && fg_rv[0] != '\n') {
      lines[i++] = line;
      line = malloc(MAX_LINE*sizeof(char));
      fg_rv = fgets(line, MAX_LINE, stdin);
   }
   *line_count_p = i;


}  /* Get_text */

/*
input1
input2
input3
input4

*/