#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define m 4
#define n 6

int main(int argc, char* argv[])
{
	int A[m][n], b[n], c[m], rank, size, l, *local_A, *local_b, *local_c;
	MPI_Datatype kolona_type, vektor_type, kolona_type_resized, vektor_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = n / size;
	local_A = (int*)malloc(m * l * sizeof(int));
	local_b = (int*)malloc(l * sizeof(int));
	local_c = (int*)malloc(m * sizeof(int));

	if (rank == 0) 
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				A[i][j] = i * j;

		for (int i = 0; i < n; i++)
			b[i] = i;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Vektor b:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			printf("%d ", b[i]);
		}
	}

	MPI_Type_vector(m * l, 1, size, MPI_INT, &kolona_type);
	MPI_Type_commit(&kolona_type);
	MPI_Type_create_resized(kolona_type, 0, sizeof(int), &kolona_type_resized);
	MPI_Type_commit(&kolona_type_resized);

	MPI_Type_vector(l, 1, size, MPI_INT, &vektor_type);
	MPI_Type_commit(&vektor_type);
	MPI_Type_create_resized(vektor_type, 0, sizeof(int), &vektor_type_resized);
	MPI_Type_commit(&vektor_type_resized);

	MPI_Scatter(&A[0][0], 1, kolona_type_resized, &local_A[0], m * l, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&b[0], 1, vektor_type_resized, &local_b[0], l, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < m; i++) 
	{
		local_c[i] = 0;
		for (int j = 0; j < l; j++)
			local_c[i] += local_A[i * l + j] + local_b[j];
	}

	MPI_Reduce(local_c, c, m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Vektor c:\n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
			printf("%d ", c[i]);
		printf("\n");
	}

	free(local_A);
	free(local_b);
	free(local_c);
	MPI_Type_free(&kolona_type);
	MPI_Type_free(&kolona_type_resized);
	MPI_Type_free(&vektor_type);
	MPI_Type_free(&vektor_type_resized);
	MPI_Finalize();
	return 0;
}