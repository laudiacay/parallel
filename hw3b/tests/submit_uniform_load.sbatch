#!/bin/bash
#SBATCH --mail-user=laudecay@uchicago.edu
#SBATCH --mail-type=ALL
#SBATCH --output=/home/laudecay/parallel/hw3b/slurm/slurm_out/speedup_uniform_load%j.%N.stdout
#SBATCH --error=/home/laudecay/parallel/hw3b/slurm/slurm_out/speedup_uniform_load%j.%N.stderr
#SBATCH --workdir=/home/laudecay/parallel/hw3b/tests/
#SBATCH --job-name=uniform
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=4:00:00
#SBATCH --exclusive
#SBATCH --qos=stafftest

module load python/3.6.1+intel-16.0
python3 speedup_uniform_load.py
