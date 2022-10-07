read -p "Input number of threads: " num_threads
echo "Running task 2. Sum numbers from 1 to N."
OMP_NUM_THREADS=$num_threads ./task2