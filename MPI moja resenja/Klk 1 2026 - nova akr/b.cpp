#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 8

int main(int argc, char* argv[])
{
	int rank, size, * ranks, m, column, X[n][n];
	MPI_Group world_group, first_column_group;
	MPI_Comm first_column_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	m = sqrt(size);
	column = rank % m;

	ranks = (int*)malloc(m * sizeof(int));

	for (int i = 0; i < m; i++)
		ranks[i] = i * m;

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, m, ranks, &first_column_group);
	MPI_Comm_create(MPI_COMM_WORLD, first_column_group, &first_column_comm);

	if (column == 0) 
	{
		int newRank, *received_elements, product = 1;
		MPI_Comm_rank(first_column_comm, &newRank);
		printf("Proces koji pripada prvoj koloni: %d->%d\n", rank, newRank);
		MPI_Datatype main_diagonal_type, main_diagonal_type_resized;

		received_elements = (int*)malloc((n / m) * sizeof(int));

		if (newRank == 0)
		{
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					X[i][j] = i * j;

			printf("Matrica X: \n");
			fflush(stdout);
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
					printf("%d ", X[i][j]);
				printf("\n");
			}
		}

		MPI_Type_vector(n / m, 1, n + 1, MPI_INT, &main_diagonal_type);
		MPI_Type_commit(&main_diagonal_type);
		MPI_Type_create_resized(main_diagonal_type, 0, n / m * (n + 1) * sizeof(int), &main_diagonal_type_resized);
		MPI_Type_commit(&main_diagonal_type_resized);

		MPI_Scatter(&X[0][0], 1, main_diagonal_type_resized, &received_elements[0], n / m, MPI_INT, 0, first_column_comm);

		printf("Proces %d je primio elemente: ", newRank);
		for (int i = 0; i < n / m; i++)
			printf("%d ", received_elements[i]);
		printf("\n");

		for (int i = 0; i < n / m; i++)
			product *= received_elements[i];
		printf("Proces %d ima proizvod: %d\n", newRank, product);

		MPI_Comm_free(&first_column_comm);
		MPI_Type_free(&main_diagonal_type);
		MPI_Type_free(&main_diagonal_type_resized);
		free(received_elements);
	}

	free(ranks);
	MPI_Group_free(&first_column_group);
	MPI_Group_free(&world_group);
	MPI_Finalize();
	return 0;
}