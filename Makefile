all:
	mpixlc -I. -qflag=w:w -lpthread calculation.c documentation.c generation.c main.c -o assignment4.xl


local:
	mpicc -lpthread calculation.c documentation.c generation.c  main.c -o test.exe 