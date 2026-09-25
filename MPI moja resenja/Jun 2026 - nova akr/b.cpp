#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 4

int main(int argc, char* argv[])
{
	int rank, size, * ranks, X[n][n], m, row;
	MPI_Group world_group, second_row_group;
	MPI_Comm second_row_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	m = sqrt(size);
	row = rank / m;

	ranks = (int*)malloc(m * sizeof(int));

	for (int i = 0; i < m; i++)
		ranks[i] = m + i;

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, m, ranks, &second_row_group);
	MPI_Comm_create(MPI_COMM_WORLD, second_row_group, &second_row_comm);

	if (row == 1)
	{
		int newRank, * received_elements, local_max;
		MPI_Datatype secondary_diagonal_type, secondary_diagonal_type_resized;
		MPI_Comm_rank(second_row_comm, &newRank);
		printf("Proces koji pripada drugom redu: %d->%d\n", rank, newRank);

		received_elements = (int*)malloc((n / m) * sizeof(int));

		if(newRank == 0)
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

		MPI_Type_vector(n / m, 1, n - 1, MPI_INT, &secondary_diagonal_type);
		MPI_Type_commit(&secondary_diagonal_type);
		MPI_Type_create_resized(secondary_diagonal_type, 0, n / m * (n - 1) * sizeof(int), &secondary_diagonal_type_resized);
		MPI_Type_commit(&secondary_diagonal_type_resized);

		MPI_Scatter(&X[0][n - 1], 1, secondary_diagonal_type_resized, &received_elements[0], n / m, MPI_INT, 0, second_row_comm);

		printf("Proces %d je primio elemente: ", newRank);
		for (int i = 0; i < n / m; i++)
			printf("%d ", received_elements[i]);
		printf("\n");

		local_max = received_elements[0];
		for (int i = 0; i < n / m; i++)
			if (received_elements[i] > local_max)
				local_max = received_elements[i];

		printf("Proces %d ima maksimum: %d\n", newRank, local_max);

		MPI_Type_free(&secondary_diagonal_type);
		MPI_Type_free(&secondary_diagonal_type_resized);
		free(received_elements);
		MPI_Comm_free(&second_row_comm);
	}
	
	free(ranks);
	MPI_Group_free(&world_group);
	MPI_Group_free(&second_row_group);
	MPI_Finalize();
	return 0;
}