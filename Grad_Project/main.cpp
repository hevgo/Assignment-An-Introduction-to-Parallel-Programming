//this code is write for the gradate stdudent project of the course High Performance Computing.
//this code intend to implement a simple simulator for the wave equation and apply MPI on them for multi-thread computation.
//If the above implementation can be finished, the author will try to extend the simulator with an automatic differentiation package
//  with the applying of MPI for multi-thread computation.
//Author: 		Yuxuan Jing
//Start date: 	04/20/2020
//Last edition:	04/20/2020


#include <iostream>
#include <fstream> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
//#include <mpi.h> 
#include <math.h>
#include <vector>
#include <omp.h>
#include <iomanip>

#include "CONSTANTS.cpp"

using std::ofstream;


void matrix_initial(double* matrix);
void output_file(double* matrix);
void matrix_update_direct(double* x_prev, double* x, double* x_next, double t);
void matrix_update_direct_omp(double* x_prev, double* x, double* x_next, double t);
void matrix_update_direct_omp_cache(double* x_prev, double* x, double* x_next, double t);
void matrix_update_direct_omp_manual(double* x_prev, double* x, double* x_next, double t);
void Step_one(double* x_prev, double* x, double* x_next, double* temp);
void Usage(char* prog_name);
void initial_time();
void print_time(char* words);


ofstream outdata; // outdata is like cin
double time_start, time_end;
int thread_count;

int main(int argc, char* argv[]){ 
		
	double t;
	
	initial_time();
	
	outdata.open("./output/output.dat"); // opens the file

	std::cout << "c = " << c << std::endl;
	if (c > 0.25){
		std::cout << "c is " << c << ", too large, the scheme may not converge!" << std::endl;
	//	return 1;
	}
	double* x_prev = new double[tot];
	double* x = new double[tot];
	double* x_next = new double[tot];
	double* temp;

	if (argc != 1 && argc != 2) Usage(argv[0]);
	if (argc == 1) thread_count = 4;
	if (argc == 2) thread_count = strtol(argv[1], NULL, 10);
	//print_time("end of the preparation");

	//first step
	Step_one(x_prev, x, x_next, temp);
	//print_time("end of initialization");

	//using explicit iterater update the values (with MPI)
	for (t = start_t; t < end_t; t += dt){
		//matrix_update_direct_omp_manual(x_prev, x, x_next, t);
		//matrix_update_direct_omp_cache(x_prev, x, x_next, t);
		matrix_update_direct_omp(x_prev, x, x_next, t);

		temp = x_prev;
		x_prev = x;
		x = x_next;
		x_next = temp;

		matrix_initial(x_next);

		//output_file(x);
	}
	//print_time("end of all the calculation");

	// delete [] x_prev;
	// delete [] x;
	// delete [] x_next;

	outdata.close();

	print_time("end of the program");
    
    return 0;
    
}


void matrix_initial(double* matrix){
	for (size_t i = 0; i < tot; i++){
		matrix[i] = 0.0;
	}
}

void output_file(double* matrix){
	for (size_t i = 0; i < tot; i++){
		outdata << matrix[i] << ' ';
	}
	outdata << std::endl;
}

void matrix_update_direct(double* x_prev, double* x, double* x_next, double t){

	for (size_t i = 0; i < N; i++){
		for (size_t j = 0; j < M; j++){	
			size_t temp = i*M + j;
			if(i == N/2 && j == M/2){
			//if(false){
				x_next[temp] = amp*sin(10*t);
			}
			else{
				if(i == 0 || i == N-1){
					if (i == 0){
						x_next[temp] += - 2.0*x[temp] + x[temp+M];
					}
					else{
						x_next[temp] += x[temp-M] - 2.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
				}

				if(j == 0 || j == M-1){
					if (j == 0){
						x_next[temp] += - 2.0*x[temp] + x[temp+1];
					}
					else{
						x_next[temp] += x[temp-1] - 2.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-1] - 2.0*x[temp] + x[temp+1];
				}

				x_next[temp] *= c;

				x_next[temp] += 2.0 * x[temp] - x_prev[temp];
	//damping
				x_next[temp] -= damp * x[temp];
				x_next[temp] *= 1.0/(1.0 + damp); 	
			}
			
		}
	}
}

void matrix_update_direct_omp(double* x_prev, double* x, double* x_next, double t){
#	pragma omp parallel for num_threads(thread_count) shared(x_prev, x, t, x_next)
	for (size_t i = 0; i < N; i++){
		for (size_t j = 0; j < M; j++){	
			size_t temp = i*M + j;
			if(i == N/2 && j == M/2){
			//if(false){
				x_next[temp] = amp*sin(10*t);
			}

			else{
				if(i == 0 || i == N-1){
					if (i == 0){
						x_next[temp] += - 2.0*x[temp] + x[temp+M];
					}
					else{
						x_next[temp] += x[temp-M] - 2.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
				}

				if(j == 0 || j == M-1){
					if (j == 0){
						x_next[temp] += - 2.0*x[temp] + x[temp+1];
					}
					else{
						x_next[temp] += x[temp-1] - 2.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-1] - 2.0*x[temp] + x[temp+1];
				}

				x_next[temp] *= c;

				x_next[temp] += 2.0 * x[temp] - x_prev[temp];
	//damping
				x_next[temp] -= damp * x[temp];
				x_next[temp] *= 1.0/(1.0 + damp); 	
			}
			
		}
	}
}

void matrix_update_direct_omp_cache(double* x_prev, double* x, double* x_next, double t){
#	pragma omp parallel for num_threads(thread_count) shared(x_prev, x, t, x_next)
	for (size_t temp = 0; temp < tot; temp++){
		//for (size_t j = 0; j < M; j++){	
			size_t i = temp/M;
			size_t j = temp%M;
			if(i == N/2 && j == M/2){
			//if(false){
				x_next[temp] = amp*sin(10*t);
			}

			else{
				if(i == 0 || i == N-1){
					if (i == 0){
						x_next[temp] += - 2.0*x[temp] + x[temp+M];
					}
					else{
						x_next[temp] += x[temp-M] - 2.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
				}

				if(j == 0 || j == M-1){
					if (j == 0){
						x_next[temp] += - 2.0*x[temp] + x[temp+1];
					}
					else{
						x_next[temp] += x[temp-1] - 2.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-1] - 2.0*x[temp] + x[temp+1];
				}

				x_next[temp] *= c;

				x_next[temp] += 2.0 * x[temp] - x_prev[temp];
	//damping
				x_next[temp] -= damp * x[temp];
				x_next[temp] *= 1.0/(1.0 + damp); 	
			}
			
		//}
	}
}

void matrix_update_direct_omp_manual(double* x_prev, double* x, double* x_next, double t){
	size_t my_rank; 
	size_t local_start, local_end;
#  	pragma omp parallel num_threads(thread_count) default(none) private(my_rank, local_start, local_end) shared(x_prev, x, t, x_next, thread_count)
	{
		my_rank = omp_get_thread_num();
		local_start = my_rank * (N/thread_count);
		local_end = (my_rank+1) * (N/thread_count) -1;


		for (size_t i = local_start; i < local_end; i++){
			for (size_t j = 0; j < M; j++){	
				size_t temp = i*M + j;
				if(i == N/2 && j == M/2){
				//if(false){
					x_next[temp] = amp*sin(10*t);
				}

				else{
					if(i == 0 || i == N-1){
						if (i == 0){
							x_next[temp] += - 2.0*x[temp] + x[temp+M];
						}
						else{
							x_next[temp] += x[temp-M] - 2.0*x[temp];
						}
					}
					else{
						x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
					}

					if(j == 0 || j == M-1){
						if (j == 0){
							x_next[temp] += - 2.0*x[temp] + x[temp+1];
						}
						else{
							x_next[temp] += x[temp-1] - 2.0*x[temp];
						}
					}
					else{
						x_next[temp] += x[temp-1] - 2.0*x[temp] + x[temp+1];
					}

					x_next[temp] *= c;

					x_next[temp] += 2.0 * x[temp] - x_prev[temp];
		//damping
					x_next[temp] -= damp * x[temp];
					x_next[temp] *= 1.0/(1.0 + damp); 	
				}
				
			}
		}

	}

	
}


void Step_one(double* x_prev, double* x, double* x_next, double* temp){

	for (size_t i = 0; i < N; i++){
		for (size_t j = 0; j < M; j++){	
			size_t temp = i*M + j;

			if(i == 0 || i == N-1){
				if (i == 0){
					x_next[temp] += - 2.0*x[temp] + x[temp+M];
				}
				else{
					x_next[temp] += x[temp-M] - 2.0*x[temp];
				}
			}
			else{
				x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
			}

			if(j == 0 || j == M-1){
				if (j == 0){
					x_next[temp] += - 2.0*x[temp] + x[temp+1];
				}
				else{
					x_next[temp] += x[temp-1] - 2.0*x[temp];
				}
			}
			else{
				x_next[temp] += x[temp-1] - 2.0*x[temp] + x[temp+1];
			}

			x_next[temp] *= c;

			x_next[temp] += 1.0 * x[temp];			
		}
	}

	temp = x_prev;
	x_prev = x;
	x = x_next;
	x_next = temp;
	matrix_initial(x_next);

}

void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
}  /* Usage */

void initial_time(){
	time_start = omp_get_wtime();
	time_end = omp_get_wtime();
	//std::cout << "time variables initialized" << std::endl;
}

void print_time(char* words){
	time_end = omp_get_wtime();
	printf("%s, total time = %f\n", words, time_end - time_start);
}