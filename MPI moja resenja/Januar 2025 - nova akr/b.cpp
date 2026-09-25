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
	int rank, size, *A, row, column, * ranks, *local_row, n;
	MPI_Group world_group, diagonal_group;
	MPI_Comm main_diagonal;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	n = sqrt(size);
	row = rank / n;
	column = rank % n;

	ranks = (int*)malloc(n * sizeof(int));
	A = (int*)malloc(n * n * sizeof(int));
	local_row = (int*)malloc(n * sizeof(int));

	for (int i = 0; i < n; i++)
		ranks[i] = i * n + i;

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, n, ranks, &diagonal_group); //kreiram diagonal_group grupu sa procesima ciji se rankovi nalaze u nizu ranks
	MPI_Comm_create(MPI_COMM_WORLD, diagonal_group, &main_diagonal); //kreiram main_diagonal komunkator sastavljen od procesa iz grupe diagonal_group

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

	if (row == column) 
	{
		int sum = 0;
		printf("Proces koji pripada glavnoj dijagonali: %d\n", rank);

		MPI_Comm_rank(main_diagonal, &rank);

		MPI_Scatter(&A[0], n, MPI_INT, &local_row[0], n, MPI_INT, 0, main_diagonal);

		printf("Proces %d je primio vrstu: \n", rank);
		for (int i = 0; i < n; i++)
		{
			sum += local_row[i];
			printf("%d ", local_row[i]);
		}
		printf("Proces %d ima sumu elemenata vrste koju je primio: %d\n", rank, sum);

		MPI_Comm_free(&main_diagonal);
	}
	free(local_row);
	free(A);
	free(ranks);
	MPI_Group_free(&world_group);
	MPI_Group_free(&diagonal_group);
	MPI_Finalize();
	return 0;
}