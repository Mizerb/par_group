#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#ifndef DOCUMENT_H
#define DOCUMENT_H
void Write_Out_Matrix(double * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height,
	int mod,
	int meh);

FILE* Create_Buffer_File();

#endif
