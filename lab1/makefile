all: serial_compute parallel_compute

serial_compute: serial_compute.c
	gcc serial_compute.c -o serial_compute -lm -std=c99

parallel_compute: parallel_compute.c
	mpicc parallel_compute.c -o parallel_compute -lm -std=c99