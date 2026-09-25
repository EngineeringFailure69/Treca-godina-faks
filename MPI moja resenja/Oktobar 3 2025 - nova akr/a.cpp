#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define m 3
#define n 4
#define k 8

int main(int argc, char* argv[])
{
	int rank, size, l, A[n][k], B[k][m], * localA, * localB, *localC, * C;
	MPI_Datatype row_type, row_type_resized, column_type, column_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = k / size;

	localA = (int*)malloc(l * n * sizeof(int));
	localB = (int*)malloc(l * m * sizeof(int));
	localC = (int*)malloc(n * m * sizeof(int));
	C = (int*)malloc(n * m * sizeof(int));

	if (rank == 0) 
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < k; j++)
				A[i][j] = i + j;

		for (int i = 0; i < k; i++)
			for (int j = 0; j < m; j++)
				B[i][j] = i * j;

		for (int i = 0; i < n * m; i++)
			C[i] = 0;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Matrica B:\n");
		fflush(stdout);
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < m; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}
	}

	MPI_Type_vector(l * n, 1, size, MPI_INT, &column_type); //kolona
	MPI_Type_commit(&column_type);
	MPI_Type_create_resized(column_type, 0, sizeof(int), &column_type_resized);
	MPI_Type_commit(&column_type_resized);

	MPI_Type_vector(l, m, m * size, MPI_INT, &row_type);
	MPI_Type_commit(&row_type);
	MPI_Type_create_resized(row_type, 0, m * sizeof(int), &row_type_resized);
	MPI_Type_commit(&row_type_resized);

	MPI_Scatter(&A[0][0], 1, column_type_resized, &localA[0], l * n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&B[0][0], 1, row_type_resized, &localB[0], l * m, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			localC[i * m + j] = 0;
			for (int z = 0; z < l; z++)
				localC[i * m + j] += localA[i * l + z] * localB[z * m + j];
		}
	}

	MPI_Reduce(&localC[0], &C[0], n * m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		printf("Matrica C:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < m; j++)
				printf("%d ", C[i * m + j]);
			printf("\n");
		}
	}

	free(localA);
	free(localB);
	free(localC);
	free(C);
	MPI_Type_free(&column_type);
	MPI_Type_free(&column_type_resized);
	MPI_Type_free(&row_type);
	MPI_Type_free(&row_type_resized);
	MPI_Finalize();
	return 0;
}