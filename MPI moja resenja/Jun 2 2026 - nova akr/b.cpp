#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

int main(int argc, char* argv[])
{
	int rank, size, * ranks, product = 1, index = 0, x = 10;
	MPI_Group world_group, no_master_group;
	MPI_Comm no_master_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	ranks = (int*)malloc((size - 1) * sizeof(int)); //rezervisem prostor za size-1 procesa jer se ne uzima proces sa rankom 0
	
	for (int i = 1; i < size; i++)
		ranks[index++] = i;

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, size - 1, ranks, &no_master_group);
	MPI_Comm_create(MPI_COMM_WORLD, no_master_group, &no_master_comm);

	if (rank != 0) 
	{
		int newRank;

		MPI_Comm_rank(no_master_comm, &newRank);
		printf("Proces koji pripada trazenoj grupi: %d\n", rank);

		//Ako se trazi proces P5 iz starog komunikatora 
		MPI_Reduce(&x, &product, 1, MPI_INT, MPI_PROD, 4, no_master_comm);
		if (rank == 5) //Ako je rank procesa 5 u starom komunikatoru, to bi u novom bilo 4 za taj isti proces
			printf("Proizvod iz procesa P5 starog komunikatora: %d\n", product);

		//Ako se trazi proces P5 iz novog komunikatora
		MPI_Reduce(&x, &product, 1, MPI_INT, MPI_PROD, 5, no_master_comm);
		if(newRank == 5) //Ako se trazi proces P5 iz novog komunikatora, onda ovako
			printf("Proizvod iz procesa P5 novog komunikatora: %d\n", product);

		MPI_Comm_free(&no_master_comm);
	}

	free(ranks);
	MPI_Group_free(&world_group);
	MPI_Group_free(&no_master_group);
	MPI_Finalize();
	return 0;
}