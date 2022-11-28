read -p "Input number of threads: " num_threads
echo "Running task 8. Quicksort"
read -p "Print 1, if you want to read array from file named input.txt. Print 2, if you want to read array from stdin. " condition
echo "In case you choose file: file structure must be as follows: first line is amount of elements, then elements on the next line."
OMP_NUM_THREADS=$num_threads ./task8 $condition