#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 8
#define k 4
#define m 3

int main(int argc, char* argv[])
{
	int rank, size, A[m][k], B[k][n], C[m][n], l, *localB, *localC;
	MPI_Datatype result_col_type, result_col_type_resized;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = n / size;
	localB = (int*)malloc(l * k * sizeof(int));
	localC = (int*)malloc(m * l * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < k; j++)
				A[i][j] = i + j;
		for (int i = 0; i < k; i++)
			for (int j = 0; j < n; j++)
				B[i][j] = i * j;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Matrica B:\n");
		fflush(stdout);
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < n; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}

		//Slanje krece od prvog procesa
		for (int i = 1; i < size; i++) 
		{
			for (int j = 0; j < l; j++) 
				for (int z = 0; z < k; z++) 
					localB[z * l + j] = B[z][j + l * i];
			MPI_Send(localB, l * k, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		//Za proces 0
		for (int i = 0; i < k; i++) 
			for (int j = 0; j < l; j++)
				localB[i * l + j] = B[i][j];

	}
	else 
	{
		MPI_Recv(localB, l * k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}

	MPI_Type_vector(m, l, n, MPI_INT, &result_col_type);
	MPI_Type_commit(&result_col_type);
	MPI_Type_create_resized(result_col_type, 0, l * sizeof(int), &result_col_type_resized);
	MPI_Type_commit(&result_col_type_resized);

	MPI_Bcast(&A[0][0], m * k, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < m; i++) 
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

	MPI_Gather(&localC[0], m * l, MPI_INT, &C[0], 1, result_col_type_resized, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Matrica C:\n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
				printf("%d ", C[i][j]);
			printf("\n");
		}
	}

	free(localB);
	free(localC);
	MPI_Type_free(&result_col_type);
	MPI_Type_free(&result_col_type_resized);
	MPI_Finalize();
	return 0;
}