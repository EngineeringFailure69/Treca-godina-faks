#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

int main(int argc, char* argv[])
{
	MPI_Send(&b1, 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	MPI_Recv(&b2, 2, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);

	//Bez moguceg deadlock-a
	MPI_Sendrecv(&b1, 2, MPI_INT, (rank + 1) % size, 0, &b2, 2, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);
}