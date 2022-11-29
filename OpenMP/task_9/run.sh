read -p "Input number of threads: " num_threads
echo "Running task 9. Bugfixing"
read -p "Input number of task (from 1 to 6) you would like to run: " num
echo -e "Running fixed task $num\n"
OMP_NUM_THREADS=$num_threads ./fixed"$num"
