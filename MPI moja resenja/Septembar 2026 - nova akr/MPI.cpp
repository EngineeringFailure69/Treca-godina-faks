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
	int rank, size, A[n][n], B[n][n], C[n][n], * localA, * localB, * localC, alfa, q, block, block_size, process_row, process_column, start_row; 
	int start_column, * allC;
	MPI_Datatype type, type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	q = sqrt(size);
	block_size = n / q;
	block = block_size * block_size; //(n / q) * (n / q); 
	localA = (int*)malloc(block * sizeof(int));
	localB = (int*)malloc(block * sizeof(int));
	localC = (int*)malloc(block * sizeof(int));
	allC = (int*)malloc(size * block * sizeof(int));

	MPI_Type_vector(block_size, block_size, n, MPI_INT, &type);
	MPI_Type_commit(&type);
	MPI_Type_create_resized(type, 0, (block / 2) * sizeof(int), &type_resized);
	MPI_Type_commit(&type_resized);

	if (rank == 0) 
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				A[i][j] = i * j;

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				B[i][j] = i + j;

		alfa = A[block][block] * B[block][block]; //faktor alfa moze da se inicijalizuje kako god, nije vazna vrednost

		printf("Matrica A:\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}

		printf("Matrica B:\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}

		printf("Faktor alfa: %d\n", alfa);

		//Proces 0 uzima svoje
		for (int i = 0; i < block_size; i++)
		{
			for (int j = 0; j < block_size; j++)
			{
				localA[i * block_size + j] = A[i][j];
				localB[i * block_size + j] = B[i][j];
			}
		}

		for (int i = 1; i < size; i++) 
		{
			process_row = i / q;
			process_column = i % q;
			start_row = block_size * process_row;
			start_column = block_size * process_column;

			MPI_Send(&A[start_row][start_column], 1, type, i, 0, MPI_COMM_WORLD);
			MPI_Send(&B[start_row][start_column], 1, type, i, 1, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(&localA[0], block, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&localB[0], block, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	MPI_Bcast(&alfa, 1, MPI_INT, 0, MPI_COMM_WORLD);

	printf("Proces %d je primio elemente iz A:\n", rank);
	for (int i = 0; i < block_size; i++) 
	{
		for (int j = 0; j < block_size; j++)
			printf("%d ", localA[i * block_size + j]);
		printf("\n");
	}

	printf("Proces %d je primio elemente iz B:\n", rank);
	for (int i = 0; i < block_size; i++)
	{
		for (int j = 0; j < block_size; j++)
			printf("%d ", localB[i * block_size + j]);
		printf("\n");
	}

	for (int i = 0; i < block; i++)
		localC[i] = alfa * localA[i] + localB[i];

	MPI_Gather(localC, block, MPI_INT, allC, block, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int p = 0; p < size; p++)
		{
			int process_row_c = p / q;
			int process_col = p % q;

			int start_row_c = process_row_c * block_size;
			int start_col = process_col * block_size;

			for (int i = 0; i < block_size; i++)
				for (int j = 0; j < block_size; j++)
					C[start_row_c + i][start_col + j] = allC[p * block + i * block_size + j];
		}
		printf("Matrica C:\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				printf("%d ", C[i][j]);
			printf("\n");
		}
	}

	MPI_Type_free(&type);
	MPI_Type_free(&type_resized);
	free(localA);
	free(localB);
	free(localC);
	free(allC);
	MPI_Finalize();
	return 0;
}