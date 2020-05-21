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
double c;

void matrix_initial(double* matrix);
void matrix_copy(double* to, double* from);
void matrix_print(double* matrix);
void output_file(double* matrix);

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
		// N = 11;
		// M = 11;
		start_t = 0.0;
		end_t = 10.0;
		dt = 0.01;
		c = 1;
	}

	//mesh initial N*N and how many variable per grid

	tot = N * M;
	double* x_prev = new double[tot];
	double* x_now = new double[tot];
	double* x_next = new double[tot];

	//initial the value of each node

	matrix_initial(x_prev);
	matrix_initial(x_now);
	matrix_initial(x_next);

	//using explicit iterater update the values (with MPI)

	for (t = start_t; t < end_t; t += dt){
		//update a;	
		for (size_t i = 0; i < N; i++){
			for (size_t j = 0; j < M; j++){
				size_t temp = i*M + j;


				//constant boundary x
				if (i != 0) a[temp-M] += 1.0 * c * x[temp];
				a[temp] -= 2.0 * c * x[temp];
				if (i != N-1) a[temp+M] += 1.0 * c * x[temp];

				// //constant boundary y
				if (j != 0) a[temp-1] += 1.0 * c * x[temp];
				a[temp] -= 2.0 * c * x[temp];
				if (j != M-1) a[temp+1] += 1.0 * c * x[temp];

				//free boundary y
				// if (j != 0) new_a[temp-1] += x[temp];
				// if (j == 0 || j == M-1){
				// 	new_a[temp] -= 1.0 * x[temp];
				// }
				// else{
				// 	new_a[temp] -= 2.0 * x[temp];	
				// }
				// if (j != M-1) new_a[temp+1] += x[temp];	
				
			}
		}

		//update v;
		for (size_t i = 0; i < N; i++){
			for (size_t j = 0; j < M; j++){
				size_t temp = i*M + j;	

				v[temp] += dt * a[temp];

				if (j == N/2){
					v[temp] += sin(t);
				}
			}
		}



		//update x;
		for (size_t i = 0; i < N; i++){
			for (size_t j = 0; j < M; j++){
				size_t temp = i*M + j;
				x[temp]  += dt * v[temp];
			}
		}
 



		std::cout << "#";
		output_file(x);
		
		// matrix_copy(x, new_x);
		// matrix_copy(v, new_v);
		// matrix_copy(a, new_a);
		

		// matrix_initial(new_x);
		// matrix_initial(new_v);
		matrix_initial(new_a);
		//output the states
	}

	std::cout << std::endl;

	//matrix_print(x);

	delete [] x;
	delete [] v;
	delete [] a;
	
	delete [] new_x;
	delete [] new_v;
	delete [] new_a;
	
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