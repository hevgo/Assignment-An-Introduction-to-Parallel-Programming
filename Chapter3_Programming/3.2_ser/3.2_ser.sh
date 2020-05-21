#! /usr/bin/env bash

#SBATCH --job-name=3.2_ser
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out
#SBATCH --ntasks=1
#SBATCH --time=3:00:00
#SBATCH --mem-per-cpu=10

module load gcc
module load openmpi

cd ~/Ex3_PA_hammer/3.2_ser
./run