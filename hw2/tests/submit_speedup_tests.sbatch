#!/bin/bash
#SBATCH --mail-user=laudecay@uchicago.edu
#SBATCH --mail-type=ALL
#SBATCH --output=/home/laudecay/parallel/hw2/slurm/slurm_out/constant%j.%N.stdout
#SBATCH --error=/home/laudecay/parallel/hw2/slurm/slurm_out/constant%j.%N.stderr
#SBATCH --workdir=/home/laudecay/parallel/hw2/tests/
#SBATCH --job-name=laudecay-hw-2-constant
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=2:00:00
#SBATCH --partition=broadwl
#SBATCH --qos=stafftest

module load python/3.7.0
python3 constant_speedup.py
