#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 3
#define k 4
#define m 8

int main(int argc, char* argv[])
{
	int rank, size, A[n][k], B[k][m], C[n][m], l, *localB, *localC, *localA, *localMin, *globalMin;
	MPI_Datatype col_type, col_type_resized;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = k / size;
	localB = (int*)malloc(l * m * sizeof(int));
	localC = (int*)malloc(n * m * sizeof(int));
	localA = (int*)malloc(l * n * sizeof(int));
	localMin = (int*)malloc(n * sizeof(int));
	globalMin = (int*)malloc(n * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < k; j++)
				A[i][j] = i + j;
		for (int i = 0; i < k; i++)
			for (int j = 0; j < m; j++)
				B[i][j] = i * j;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Matrica B:\n");
		fflush(stdout);
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}

	}

	MPI_Type_vector(n, l, k, MPI_INT, &col_type);
	MPI_Type_commit(&col_type);
	MPI_Type_create_resized(col_type, 0, l * sizeof(int), &col_type_resized);
	MPI_Type_commit(&col_type_resized); //izvedeni tip za slanje l kolona iz matrice A

	MPI_Scatter(&A[0][0], 1, col_type_resized, &localA[0], n * l, MPI_INT, 0, MPI_COMM_WORLD); //slanje kolona iz matrice A
	MPI_Scatter(&B[0][0], l * m, MPI_INT, &localB[0], l * m, MPI_INT, 0, MPI_COMM_WORLD); //posto su svi elementi vrste uzastopni u memoriji, mogu ovako

	//Racunam lokalni proizvod matrice za svaki proces
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++) 
		{
			localC[i * m + j] = 0;
			for (int z = 0; z < l; z++)
				localC[i * m + j] += localA[i * l + z] * localB[z * m + j];
		}
	}

	//Racunam lokalni minimum svake vrste matrice A
	for (int i = 0; i < n; i++)
	{
		localMin[i] = INT_MAX;

		for (int j = 0; j < l; j++)
		{
			if (localA[i * l + j] < localMin[i])
			{
				localMin[i] = localA[i * l + j];
			}
		}
	}

	for (int p = 0; p < size; p++)
	{
		if (rank == p)
		{
			printf("Primljene kolone iz A za proces %d:\n", rank);
			fflush(stdout);
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < l; j++)
					printf("%d ", localA[i * l + j]);
				printf("\n");
			}

			printf("Primljene vrste iz B za proces %d:\n", rank);
			fflush(stdout);
			for (int i = 0; i < l; i++)
			{
				for (int j = 0; j < m; j++)
					printf("%d ", localB[i * m + j]);
				printf("\n");
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	//Sumiram sve lokalne proizvode i stavljam u matricu C
	MPI_Reduce(&localC[0], C, n * m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	//Spajam sve lokalne minimume 
	MPI_Reduce(&localMin[0], &globalMin[0], n, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Matrica C:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d ", C[i][j]);
			printf("\n");
		}

		printf("Minimum svake vrste matrice A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			printf("Minimum vrste %d = %d\n", i, globalMin[i]);
		}
	}

	free(localB);
	free(localC);
	free(localA);
	free(localMin);
	free(globalMin);
	MPI_Type_free(&col_type);
	MPI_Type_free(&col_type_resized);
	MPI_Finalize();
	return 0;
}