#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define n 3
#define k 4
#define m 8

int main(int argc, char* argv[])
{
	int rank, size, l, A[n][k], B[k][m], *C, *localB, *localC;
	MPI_Datatype col_type, col_type_resized, result_column_type, result_column_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = m / size;

	C = (int*)malloc(n * m * sizeof(int));
	localC = (int*)malloc(n * l * sizeof(int));
	localB = (int*)malloc(l * k * sizeof(int));

	if (rank == 0) 
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < k; j++)
				A[i][j] = i + j;
		for (int i = 0; i < k; i++)
			for (int j = 0; j < m; j++)
				B[i][j] = i * j;

		printf("Matrix A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Matrix B:\n");
		fflush(stdout);
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}
	}

	MPI_Type_vector(k, l, m, MPI_INT, &col_type);
	MPI_Type_commit(&col_type);
	MPI_Type_create_resized(col_type, 0, l * sizeof(int), &col_type_resized);
	MPI_Type_commit(&col_type_resized);

	MPI_Type_vector(n, l, m, MPI_INT, &result_column_type);
	MPI_Type_commit(&result_column_type);
	MPI_Type_create_resized(result_column_type, 0, l * sizeof(int), &result_column_type_resized);
	MPI_Type_commit(&result_column_type_resized);

	MPI_Bcast(&A[0][0], n * k, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&B[0][0], 1, col_type_resized, &localB[0], k * l, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < l; j++)
		{
			localC[i * l + j] = 0;
			for (int z = 0; z < k; z++)
				localC[i * l + j] += A[i][z] * localB[z * l + j];
		}
    }

	printf("Primljene kolone iz B za proces %d:\n", rank);
	fflush(stdout);
	for (int i = 0; i < k; i++) 
	{
		for (int j = 0; j < l; j++) 
			printf("%d ", localB[i * l + j]);
		printf("\n");
	}

	MPI_Gather(&localC[0], n * l, MPI_INT, &C[0], 1, result_column_type_resized, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		printf("Result Matrix C:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d ", C[i * m + j]);
			printf("\n");
		}
	}

	free(localB);
	free(localC);
	free(C);
	MPI_Type_free(&col_type);
	MPI_Type_free(&col_type_resized);
	MPI_Type_free(&result_column_type);
	MPI_Type_free(&result_column_type_resized);
	MPI_Finalize();
	return 0;
}