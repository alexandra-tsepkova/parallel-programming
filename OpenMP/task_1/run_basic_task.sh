read -p "Input number of threads: " num_threads
echo "Running task 1. Print numbers of threads."
OMP_NUM_THREADS=$num_threads ./task1