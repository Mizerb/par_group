all:
	mpixlc -I. -qflag=w -lpthread calculation.c documentation.c generation.c main.c -o assignment4.xl
