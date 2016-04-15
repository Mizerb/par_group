#include "documentation.h"
#include <string.h>

#define CALL_ALL(foo, blocked, file_count) foo(matrix_data, mpi_rank, matrix_size, matrix_slice_height, blocked, file_count) 

#define blocked 1
#define compact 0

void File_Write(double * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height,
	int meh,
	int file_count)
{
	MPI_File fh;
	MPI_Status status;
	int fusize, nints;
	unsigned long long offset;
	int size = matrix_slice_height * matrix_size; //something like that, have to ask others
	// File name fun
	/*
	char file_name[80] = "output";
	char num_str[20];
	int file_num = 0;
	if(file_count != 1 ) file_num = mpi_rank/file_count; 
	sprintf(num_str, "%d" , file_num);
	strcat(file_name, num_str);
	strcat(file_name, ".log");
	*/

	char file_name[80];
	int file_number = 0;
	if(file_count != 1 ) file_number = mpi_rank/file_count; 

	//printf("RANK %d has filenum : %d for file count %d\n" , mpi_rank , file_number, file_count);
	sprintf(file_name , "out/%d-output-%d.log",meh, file_number);


	//End of Filename fun

	offset =  (mpi_rank%file_count) * size * sizeof(double);

		
	if( file_count == 1)
	{				//OFFSET count of object to offset, not byte count of offset
		// WORKING
		MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_WRONLY|MPI_MODE_CREATE , MPI_INFO_NULL, &fh);
	
		MPI_File_write_at_all(fh, offset, matrix_data, size , MPI_DOUBLE, &status );
	}
	else
	{
		//printf("dickbut\n");
		MPI_Comm file_comm;
		MPI_Comm_split(MPI_COMM_WORLD, file_number, mpi_rank, &file_comm);
	    MPI_File_open(file_comm, file_name, MPI_MODE_WRONLY|MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
	    
	    //MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_WRONLY|MPI_MODE_CREATE , MPI_INFO_NULL, &fh);
		
		MPI_Barrier(file_comm);

	    //printf("RANK %d writing to file %s\n", mpi_rank , file_name);
	    /*
	    char HA[200];
	    sprintf(HA , "meh.%d", mpi_rank );
	     int i;
	    if( mpi_rank == 0 )
	    	for( i = 0 ; i < size ; i++ )
	    		printf( "IS THERE DATA: %f\n" , matrix_data[i]  );
		*/
		//printf("RANK %d WRITING %d bytes at offset %d \n", mpi_rank, (int)size*sizeof(double) , (int)offset);	
		
		MPI_File_write_at(fh,offset, matrix_data, size, MPI_DOUBLE, &status);
		
	}
	
	MPI_Barrier( MPI_COMM_WORLD );
	MPI_File_close(&fh);
}



void Write_Out_Matrix(double * matrix_data, 
	int mpi_rank, 
	int matrix_size , 
	int matrix_slice_height,
	int mode,
	int meh )
{
	//shit shit shit
	//Tired, pain.. fkkk
	switch(mode) {
		case 0:
			CALL_ALL(File_Write, meh , 1);
			break;
		case 1:
			CALL_ALL(File_Write, meh , 4);
			break;
		case 2:
			CALL_ALL(File_Write, meh , 8);
			break;
		case 3:
			CALL_ALL(File_Write, meh , 32);
			break;
	}

}