read -p "Input number of threads: " num_threads
echo "Running task 3. Make array from other array. Result is saved in the file output.txt."
OMP_NUM_THREADS=$num_threads ./task3