all:
	mpicc -lpthread calculation.c documentation.c generation.c ./Timer/timer.c main.c -o test.exe 