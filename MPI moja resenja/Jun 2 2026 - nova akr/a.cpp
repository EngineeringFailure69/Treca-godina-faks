#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define m 4
#define k 8
#define s 2

int main(int argc, char* argv[])
{
	int rank, size, A[m][k], b[k], c[m], localA[m][s], localc[m], local_min[m], global_min[m];
	MPI_Datatype column_type, column_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (int i = 0; i < k; i++)
		b[i] = i;

	if (rank == 0)
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

		printf("\nVektor b: ");
		fflush(stdout);
		for (int i = 0; i < k; i++)
			printf("%d ", b[i]);
	}

	MPI_Type_vector(m, s, k, MPI_INT, &column_type);
	MPI_Type_commit(&column_type);
	MPI_Type_create_resized(column_type, 0, s * sizeof(int), &column_type_resized);
	MPI_Type_commit(&column_type_resized);

	MPI_Scatter(&A[0][0], 1, column_type_resized, &localA[0], m * s, MPI_INT, 0, MPI_COMM_WORLD);

	/*printf("Proces %d je primio kolone: \n", rank);
	fflush(stdout);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < s; j++)
			printf("%d ", localA[i][j]);
		printf("\n");
	}*/

	//c = A * b
	for (int i = 0; i < m; i++) 
	{
		localc[i] = 0;
		for (int j = 0; j < s; j++)
			localc[i] += localA[i][j] * b[rank * s + j];
	}

	//Min svake vrste A
	for (int i = 0; i < m; i++) 
	{
		local_min[i] = localA[i][0];
		for (int j = 1; j < s; j++)
			if (localA[i][j] < local_min[i])
				local_min[i] = localA[i][j];
	}

	MPI_Reduce(&localc[0], &c[0], m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&local_min[0], &global_min[0], m, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		printf("\nRezultujuci vektor c: ");
		//fflush(stdout);
		for (int i = 0; i < m; i++)
			printf("%d ", c[i]);

		printf("\nMinimumi svake vrste matrice A: ");
		//fflush(stdout);
		for (int i = 0; i < m; i++)
			printf("Vrsta %d: %d, ", i, global_min[i]);
		printf("\n");
	}

	MPI_Type_free(&column_type);
	MPI_Type_free(&column_type_resized);
	MPI_Finalize();
	return 0;
}