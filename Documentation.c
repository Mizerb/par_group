#include "Documentation.h"

#define CALL_ALL(foo) foo(matrix_data, mpi_rank, matrix_size, matrix_slice_height) 


void 1_File_Block(int * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height)
{
	MPI_File fh;
	MPI_Status status;
	int fusize, nints;

	int offset = mpi_rank * matrix_slice_height; //something like that

	char* file_name;
	MPI_FILE_open(MPI_COMM_WORLD, file_name, MPI_MODE_WRONLY|MPI_MODE_CREATE , MPI_INFO_NULL, &fh);

	MPI_File_write_at(fh,offset, matrix_data, matrix_size, MPI_INT, &status);

	MPI_File_close(&fh);
}



void Write_OUt_Matrix(int * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height,
	int mode )
{
	//shit shit shit

	switch(mode) {

		case 0: 
			CALL_ALL(1_File_Block);
			break;
		case 1:
			CALL_ALL(1_File_Boundary);
			break;
		case 2:
			CALL_ALL(4_File_Block);
			break;
		case 3:
			CALL_ALL(4_File_Boundary);
			break;
		case 4:
			CALL_ALL(8_File_Block);
			break;
		case 5:
			CALL_ALL(8_File_Boundary);
		case 6:
			CALL_ALL(32_File_Block);
		case 7:
			CALL_ALL(32_File_Boundary);

	}

}