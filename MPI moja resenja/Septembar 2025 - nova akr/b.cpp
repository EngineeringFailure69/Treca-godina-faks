#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define x 10

typedef struct 
{
	int rank;
	int prod;
}max_prod;

int main(int argc, char* argv[])
{
	int rank, size, * A, row, column, * ranks, * local_row, n, counter = 0;
	MPI_Group world_group, diagonal_group;
	MPI_Comm secondary_diagonal;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	n = sqrt(size);
	row = rank / n;
	column = rank % n;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			int process_rank = i * n + j;

			if (i + j == n - 1 && process_rank < x)
				counter++;
		}

	ranks = (int*)malloc(counter * sizeof(int));

	int index = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			int process_rank = i * n + j;

			if (i + j == n - 1 && process_rank < x)
				ranks[index++] = process_rank;
		}

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, counter, ranks, &diagonal_group); //kreiram diagonal_group grupu sa procesima ciji se rankovi nalaze u nizu ranks
	MPI_Comm_create(MPI_COMM_WORLD, diagonal_group, &secondary_diagonal); //kreiram secondary_diagonal komunkator sastavljen od procesa iz grupe diagonal_group

	if (secondary_diagonal != MPI_COMM_NULL)
	{
		int prod = 1;
		A = (int*)malloc(n * n * sizeof(int));
		local_row = (int*)malloc(n * sizeof(int));
		max_prod local_max, global_max;

		printf("Proces koji pripada sporednoj dijagonali i ima rank manji od x: %d\n", rank);

		MPI_Comm_rank(secondary_diagonal, &rank);

		if (rank == 0)
		{
			for (int i = 0; i < n * n; i++)
				A[i] = i;
			printf("Matrica A:\n");
			fflush(stdout);
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
					printf("%d ", A[i * n + j]);
				printf("\n");
				fflush(stdout);
			}
		}

		MPI_Scatter(&A[0], n, MPI_INT, &local_row[0], n, MPI_INT, 0, secondary_diagonal);

		printf("Proces %d je primio vrstu: \n", rank);
		for (int i = 0; i < n; i++)
		{
			prod *= local_row[i];
			printf("%d ", local_row[i]);
		}
		printf("Proces %d ima proizvod elemenata vrste koju je primio: %d\n", rank, prod);

		local_max.prod = prod;
		local_max.rank = rank;
		MPI_Reduce(&local_max, &global_max, 1, MPI_2INT, MPI_MAXLOC, 0, secondary_diagonal);
		if(rank == 0)
			printf("Proces sa rankom %d sadrzi najveci proizvod elemenata vrste koju je primio, i taj proizvod iznosi: %d\n", global_max.rank, global_max.prod);

		MPI_Comm_free(&secondary_diagonal);
		free(local_row);
		free(A);
	}

	free(ranks);
	MPI_Group_free(&world_group);
	MPI_Group_free(&diagonal_group);
	MPI_Finalize();
	return 0;
}