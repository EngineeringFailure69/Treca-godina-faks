#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define m 4
#define k 4
#define x 2

//Sredi situaciju ako ima vise procesa od broja kolona matrice koje se dele

int main(int argc, char* argv[])
{
	int rank, size, A[m][k], *ranks, ranks_count = 0, index = 0;
	MPI_Group world_group, x_group;
	MPI_Comm x_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (int i = 0; i < size; i++)
		if (i % x == 0)
			ranks_count++;

	ranks = (int*)malloc(ranks_count * sizeof(int));

	for(int i=0;i<size;i++)
		if (i % x == 0) 
		{
			ranks[index] = i;
			index++;
		}

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, ranks_count, ranks, &x_group);
	MPI_Comm_create(MPI_COMM_WORLD, x_group, &x_comm);

	if (x_comm != MPI_COMM_NULL)
	{
		int newRank, * localA, local_sum = 0, min_sum;
		MPI_Datatype column_type, column_type_resized;
		MPI_Comm_rank(x_comm, &newRank);
		printf("Proces koji pripada prvoj koloni: %d->%d\n", rank, newRank);

		localA = (int*)malloc(m * sizeof(int));

		if (newRank == 0)
		{
			for (int i = 0; i < m; i++)
				for (int j = 0; j < k; j++)
					A[i][j] = i + j;

			printf("Matrica A:\n");
			fflush(stdout);
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < k; j++)
					printf("%d ", A[i][j]);
				printf("\n");
			}
		}

		MPI_Type_vector(m, 1, k, MPI_INT, &column_type);
		MPI_Type_commit(&column_type);
		MPI_Type_create_resized(column_type, 0, sizeof(int), &column_type_resized);
		MPI_Type_commit(&column_type_resized);

		MPI_Scatter(&A[0][0], 1, column_type_resized, &localA[0], m, MPI_INT, 0, x_comm);

		printf("Proces %d (novi rank %d) primio kolonu: \n", rank, newRank);

		for (int i = 0; i < m; i++)
			printf("%d ", localA[i]);
		printf("\n");

		for (int i = 0; i < m; i++)
			local_sum += localA[i];

		printf("Proces %d (novi rank %d) ima lokalnu sumu kolone: %d \n", rank, newRank, local_sum);

		MPI_Reduce(&local_sum, &min_sum, 1, MPI_INT, MPI_MIN, 0, x_comm);

		if (newRank == 0)
			printf("Minimalna suma svih primljenih kolona: %d\n", min_sum);

		free(localA);
		MPI_Type_free(&column_type);
		MPI_Type_free(&column_type_resized);
		MPI_Comm_free(&x_comm);
	}

	free(ranks);
	MPI_Group_free(&world_group);
	MPI_Group_free(&x_group);
	MPI_Finalize();
	return 0;
}