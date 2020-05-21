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
#include <mpi.h> 
#include <math.h>
#include <vector>

#include <iomanip>

#include "CONSTANTS.cpp"

using std::ofstream;

//const int MAX_LINE = 80;
//size_t N, M;
size_t tot;
ofstream outdata; // outdata is like cin
double c, damp, amp;

void matrix_initial(double* matrix);
void matrix_copy(double* to, double* from);
void matrix_print(double* matrix);
void output_file(double* matrix);
void output_test(double* matrix);
void matrix_update_direct(double* x_prev, double* x, double* x_next, double t);
void matrix_update_direct_sealed(double* x_prev, double* x, double* x_next, double t);
void matrix_update_extend(double* x_prev, double* x, double* x_next, double t);
void Step_one(double* x_prev, double* x, double* x_next, double* temp);

int main(int argc, char* argv[]){ 


	printf("Do you want to input the parameters? (y/n) (n for default mode)\n");
	char* bool_default = (char*)malloc(MAX_LINE*sizeof(char));
	fgets(bool_default, MAX_LINE, stdin);

	while(bool_default[0] != 'y' and bool_default[0] != 'n'){
		printf("It seems like you do not want to test this program!\n");
		return 1;
	}
	
	outdata.open("./output/output.dat"); // opens the file

	double t, dt, end_t, start_t;

	if(bool_default[0] == 'y'){
		printf("input y\n");
	}
	else{
		printf("input n\n");
		start_t = 0.0;
		end_t = 8.0;
		dt = 0.02;
		c = dt * dt * V * V / ( L * W /(N-1) /(M-1)); // c = dt*v*v/dxdy
		std::cout << "c = " << c << std::endl;
		if (c > 0.25){
			std::cout << "c is too large, the scheme may not converge!" << std::endl;
			return 1;
		}
		damp = 0.3 * dt;
		amp = 10.0 / (L/(N-1));
	}

	//mesh initial N*N and how many variable per grid

	tot = N * M;
	double* x_prev = new double[tot];
	double* x = new double[tot];
	double* x_next = new double[tot];
	double* temp;

	//initial the value of each node

	//x[(N/2)*M + M/2] = 100;

	//first step

	Step_one(x_prev, x, x_next, temp);

	//using explicit iterater update the values (with MPI)

	for (t = start_t; t < end_t; t += dt){
		//update x;

		//matrix_update_direct(x_prev, x, x_next);
		matrix_update_direct(x_prev, x, x_next, t);

		output_file(x);

		temp = x_prev;
		x_prev = x;
		x = x_next;
		x_next = temp;
		matrix_initial(x_next);
	}

	std::cout << std::endl;

	//matrix_print(x);

	delete [] x_prev;
	delete [] x;
	delete [] x_next;

	
	outdata.close();
    
    return 0;
    
}


void matrix_initial(double* matrix){
	for (size_t i = 0; i < tot; i++){
		matrix[i] = 0.0;
	}
}

void matrix_copy(double* to, double* from){
	for (size_t i = 0; i < tot; i++){
		to[i] = from[i];
	}	
}

void matrix_print(double* matrix){
	for (size_t i = 0; i < N; i++){
		for (size_t j = 0; j < M; j++){
			size_t temp = i*M + j;
			//std::cout << matrix[temp] << "|";

			if (matrix[temp] == 0){
				std::cout << '0';	
			}
			else{
				std::cout << '1';
			}
			
		}
		std::cout << std::endl;
	}		
}

void output_file(double* matrix){
	for (size_t i = 0; i < tot; i++){
		outdata << matrix[i] << ' ';
	}
	outdata << std::endl;
}

void output_test(double* matrix){
	for (size_t i = 0; i < tot; i++){
		if(matrix[i] == 0){
			outdata << "0";
		}
		else{
			outdata << "1";
		}
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

void matrix_update_direct_sealed(double* x_prev, double* x, double* x_next, double t){
	for (size_t i = 0; i < N; i++){
		for (size_t j = 0; j < M; j++){	
			size_t temp = i*M + j;
			if(i == N/2 && j == M/2){
			//if(false){
				x_next[temp] = 100*sin(10*t);
			}
			else{
				if(i == 0 || i == N-1){
					if (i == 0){
						x_next[temp] += - 1.0*x[temp] + x[temp+M];
					}
					else{
						x_next[temp] += x[temp-M] - 1.0*x[temp];
					}
				}
				else{
					x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
				}

				if(j == 0 || j == M-1){
					if (j == 0){
						x_next[temp] += - 1.0*x[temp] + x[temp+1];
					}
					else{
						x_next[temp] += x[temp-1] - 1.0*x[temp];
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


void matrix_update_extend(double* x_prev, double* x, double* x_next, double t){
	for (size_t i = 1; i < N-1; i++){
		for (size_t j = 1; j < M-1; j++){
			size_t temp = i*M + j;



			x_next[temp] += x[temp-M] - 2.0*x[temp] + x[temp+M];
			x_next[temp] += x[temp-1] - 2.0*x[temp] + x[temp+1];
			x_next[temp] *= c;

			x_next[temp] += 2.0 * x[temp] - x_prev[temp];
			
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