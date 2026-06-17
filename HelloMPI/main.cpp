#include <iostream>
#include <mpi.h>
using std::cin;
using std::cout;
using std::endl;

#pragma comment(lib, "msmpi.lib")

void main(int argc, char** argv) 
{
	setlocale(LC_ALL, "");
	cout << "Hello MPI" << endl;
	int my_rank;
	int world_size;
	int count = 4;

	/*char* program = new char[32];
	strcpy(program, "HelloMPI.exe");
	char** p_program = &program;*/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	cout << "Hello from process " << my_rank << " out of " << world_size << " proccess!" << endl;
	//delete[] program;
	MPI_Finalize();
}