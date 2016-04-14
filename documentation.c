#include "documentation.h"
#include <string.h>

#define CALL_ALL(foo, blocked, file_count) foo(matrix_data, mpi_rank, matrix_size, matrix_slice_height, blocked, file_count) 

#define blocked 1
#define compact 0

void File_Write(double * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height,
	int is_blocked,
	int file_count)
{
	MPI_File fh;
	MPI_Status status;
	int fusize, nints;
	int offset;
	int size = matrix_slice_height * matrix_size; //something like that, have to ask others
	// File name fun
	char file_name[80] = "out/output";
	char num_str[20];
	sprintf(num_str, "%d" , mpi_rank/file_count);
	strcat(file_name, num_str);
	strcat(file_name, ".log");
	//End of Filename fun
	
	if(is_blocked == blocked)
	{
		int eight_meg = 8388608;
		
		int blocks = ( (sizeof(double)*size) + eight_meg - 1)/ eight_meg;
		offset  = eight_meg * blocks; 
	}
	else
	{
		offset = mpi_rank  * size;
	}
	
	
	MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_WRONLY|MPI_MODE_CREATE , MPI_INFO_NULL, &fh);
	
	
	if( file_count == 1)
	{				//OFFSET count of object to offset, not byte count of offset
		MPI_File_write_at_all(fh, offset, matrix_data, size , MPI_DOUBLE, &status );
	}
	else
	{
		MPI_File_write_at(fh,offset, matrix_data, size, MPI_DOUBLE, &status);
	}
	
	
	
	

	MPI_File_close(&fh);
}



void Write_Out_Matrix(double * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height,
	int mode )
{
	//shit shit shit
	//Tired, pain.. fkkk
	switch(mode) {

		case 0: 
			CALL_ALL(File_Write, blocked , 1);
			break;
		case 1:
			CALL_ALL(File_Write, compact , 1);
			break;
		case 2:
			CALL_ALL(File_Write, blocked , 4);
			break;
		case 3:
			CALL_ALL(File_Write, compact , 4);
			break;
		case 4:
			CALL_ALL(File_Write, blocked , 8);
			break;
		case 5:
			CALL_ALL(File_Write, compact , 8);
			break;
		case 6:
			CALL_ALL(File_Write, blocked , 32);
			break;
		case 7:
			CALL_ALL(File_Write, compact , 32);
			break;
	}

}