read -p "Input number of threads: " num_threads
echo "Running task 2 (advanced). Sum numbers from 1 to N."
read -p "Input N: " N
OMP_NUM_THREADS=$num_threads ./task2_add $N