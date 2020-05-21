#! /usr/bin/env bash

#SBATCH --job-name=3.2_4_par
#SBATCH --error=Test_4.%J.err
#SBATCH --output=Test_4.%J.out
#SBATCH --ntasks=4
#SBATCH --time=3:00:00
#SBATCH --mem-per-cpu=10000

module load gcc
module load openmpi

cd ~/Ex3_PA_hammer/3.2_par

mpiexec -n 4 ./run