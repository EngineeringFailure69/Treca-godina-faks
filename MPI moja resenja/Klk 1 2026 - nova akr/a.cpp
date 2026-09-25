#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define n 5
#define k 6

int main(int argc, char* argv[])
{
	int rank, size, l, A[n][k], b[k], c[n], * localA, * localb, * localc;
	MPI_Datatype column_type, column_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = k / size;
	localA = (int*)malloc(n * l * sizeof(int));
	localb = (int*)malloc(l * sizeof(int));
	localc = (int*)malloc(n * sizeof(int));

	if (rank == 0) 
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < k; j++)
				A[i][j] = i + j;
		for (int i = 0; i < k; i++)
			b[i] = i;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Vektor b: ");
		//fflush(stdout);
		for (int i = 0; i < k; i++)
			printf("%d ", b[i]);
		printf("\n");

		for (int i = 0; i < l; i++) //proces 0 dobija svoje elemente direktno
		{
			localb[i] = b[i * size];   //primer: b = 0 1 2 3 4 5, size = 3 
		}

		for (int i = 1; i < size; i++) //salje se od prvog procesa
		{ 
			int* temp = (int*)malloc(l * sizeof(int));
			for (int j = 0; j < l; j++)
				temp[j] = b[i + j * size];
			MPI_Send(temp, l, MPI_INT, i, 0, MPI_COMM_WORLD);
			free(temp);
		}
	}
	else
	{
		MPI_Recv(&localb[0], l, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	MPI_Type_vector(n * l, 1, size, MPI_INT, &column_type);
	MPI_Type_commit(&column_type);
	MPI_Type_create_resized(column_type, 0, sizeof(int), &column_type_resized);
	MPI_Type_commit(&column_type_resized);

	MPI_Scatter(&A[0][0], 1, column_type_resized, &localA[0], n * l, MPI_INT, 0, MPI_COMM_WORLD);

	printf("Proces %d je primio elemente vektora b: ", rank);
	for (int i = 0; i < l; i++)
		printf("%d ", localb[i]);
	printf("\n");

	printf("Proces %d je primio kolone matrice A: \n", rank);
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < l; j++)
			printf("%d ", localA[i * l + j]);
		printf("\n");
	}

	for (int i = 0; i < n; i++) 
	{
		localc[i] = 0;
		for (int j = 0; j < l; j++)
			localc[i] += localA[i * l + j] * localb[j];
	}

	MPI_Reduce(&localc[0], &c[0], n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Vektor c:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
			printf("%d ", c[i]);
		printf("\n");
	}

	free(localA);
	free(localb);
	free(localc);
	MPI_Type_free(&column_type);
	MPI_Type_free(&column_type_resized);
	MPI_Finalize();
	return 0;
}