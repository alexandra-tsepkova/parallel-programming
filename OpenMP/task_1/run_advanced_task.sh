read -p "Input number of threads: " num_threads
echo "Running task1 (advanced). Print numbers of threads in reverse order."
OMP_NUM_THREADS=$num_threads ./task1_add