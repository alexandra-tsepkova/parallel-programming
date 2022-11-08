read -p "Input number of threads: " num_threads
echo "Running task 4. Parallel matrix multiplication. Matrices are saved to files matrix_1.txt and matrix_2.txt.
Result of multiplication is saved to file output.txt"
OMP_NUM_THREADS=$num_threads ./task4