#! /usr/bin/env bash

#SBATCH --job-name=3.8
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out
#SBATCH --ntasks=128
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=100

module load gcc
module load openmpi

cd ~/Ex3_PA_hammer/3.8
mpiexec -n 1 ./run
mpiexec -n 2 ./run
mpiexec -n 4 ./run
mpiexec -n 8 ./run
mpiexec -n 16 ./run
mpiexec -n 32 ./run
mpiexec -n 64 ./run
mpiexec -n 128 ./run