#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 6
#define m 4

int main(int argc, char* argv[])
{
	int rank, size, A[n][m], b[m], c[n], l, * localB, * localC, * localA;
	MPI_Datatype col_type, col_type_resized, row_type, row_type_resized;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = m / size;
	localB = (int*)malloc(l * sizeof(int));
	localC = (int*)malloc(n * sizeof(int));
	localA = (int*)malloc(l * n * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				A[i][j] = i + j;
		for (int i = 0; i < m; i++)
			b[i] = i;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Vektor b:\n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
			printf("%d ", b[i]);
		printf("\n");
	}

	MPI_Type_vector(n * l, 1, size, MPI_INT, &col_type);
	MPI_Type_commit(&col_type);
	MPI_Type_create_resized(col_type, 0, sizeof(int), &col_type_resized);
	MPI_Type_commit(&col_type_resized); 

	MPI_Type_vector(l, 1, size, MPI_INT, &row_type);
	MPI_Type_commit(&row_type);
	MPI_Type_create_resized(row_type, 0, sizeof(int), &row_type_resized);
	MPI_Type_commit(&row_type_resized);

	MPI_Scatter(&A[0][0], 1, col_type_resized, &localA[0], n * l, MPI_INT, 0, MPI_COMM_WORLD); //slanje kolona iz matrice A
	MPI_Scatter(&b[0], 1, row_type_resized, &localB[0], l, MPI_INT, 0, MPI_COMM_WORLD); //Slanje elemenata vektora b

	//Racunam lokalni proizvod A*b za svaki proces
	for (int i = 0; i < n; i++) 
	{
		localC[i] = 0;
		for (int j = 0; j < l; j++) 
			localC[i] += localA[i * l + j] * localB[j];
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

			printf("Primljeni elementi iz b za proces %d:\n", rank);
			fflush(stdout);
			for (int i = 0; i < l; i++)
				printf("%d ", localB[i]);
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	//Sumiram sve lokalne proizvode i stavljam u vektor c
	MPI_Reduce(&localC[0], &c[0], n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Vektor c:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
			printf("%d ", c[i]);
		printf("\n");
	}

	free(localB);
	free(localC);
	free(localA);
	MPI_Type_free(&col_type);
	MPI_Type_free(&col_type_resized);
	MPI_Type_free(&row_type);
	MPI_Type_free(&row_type_resized);
	MPI_Finalize();
	return 0;
}