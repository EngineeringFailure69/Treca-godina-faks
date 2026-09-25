#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 3

int main(int argc, char* argv[])
{
	int rank, size, A[n][n], B[n][n], C[n][n], localA[n], localB[n], localC = 0, row, column;
	MPI_Datatype columType;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	row = rank / n;
	column = rank % n;

	MPI_Type_vector(n, 1, n, MPI_INT, &columType);
	MPI_Type_commit(&columType);

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A[i][j] = i + j;
				B[i][j] = i * j;
			}
		}

		printf("Matrica A:\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
		printf("Matrica B:\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("%d ", B[i][j]);
			}
			printf("\n");
		}

		//Master proces ne salje samom sebi, on vec ima elemente, samo uzima one koji mu trebaju
		for (int i = 0; i < n; i++) 
		{
			localA[i] = A[0][i];
			localB[i] = B[i][0];
		}

		for (int i = 1; i < size; i++) 
		{
			int destRow = i / n;
			int destColumn = i % n;
			MPI_Send(&A[destRow][0], n, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&B[0][destColumn], 1, columType, i, 1, MPI_COMM_WORLD);
		}
	}
	else 
	{
		MPI_Recv(&localA, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&localB, n, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}

	printf("Proces: %d je primio vrstu iz A: ", rank);
	for (int i = 0; i < n; i++)
		printf("%d ", localA[i]);

	printf("Proces: %d je primio kolonu iz B: ", rank);
	for (int i = 0; i < n; i++)
		printf("%d ", localB[i]);

	for (int i = 0; i < n; i++)
		localC += localA[i] * localB[i];

	printf("Proces: %d je izracunao element za C: ", rank);
	printf("%d\n", localC);

	MPI_Gather(&localC, 1, MPI_INT, &C[0][0], 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		printf("Matrica C: \n");
		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < n; j++) 
				printf("%d ", C[i][j]);
			printf("\n");
		}
	}

	MPI_Type_free(&columType);
	MPI_Finalize();
	return 0;
}