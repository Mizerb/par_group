all:
	mpicc calculation.c documentation.c generation.c ./Timer/timer.c main.c -o test.exe -lpthread