#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define m 4
#define k 4

int main(int argc, char* argv[])
{
	int rank, size, A[m][k], b[k], * localA, c[m], * localc, l, column_sum_A[k], local_column_sum_A[k];
	MPI_Datatype row_type, row_type_resized, result_row, result_row_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = m / size;

	localA = (int*)malloc(l * k * sizeof(int));
	localc = (int*)malloc(l * sizeof(int));
	//local_column_sum_A = (int*)malloc(k * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < k; j++)
				A[i][j] = i + j;
		for (int i = 0; i < k; i++)
			b[i] = i;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Vektor b: ");
		for (int i = 0; i < k; i++)
			printf("%d ", b[i]);
		printf("\n");
	}

	MPI_Type_vector(l, k, size * k, MPI_INT, &row_type);
	MPI_Type_commit(&row_type);
	MPI_Type_create_resized(row_type, 0, k * sizeof(int), &row_type_resized);
	MPI_Type_commit(&row_type_resized);

	MPI_Type_vector(l, 1, size, MPI_INT, &result_row);
	MPI_Type_commit(&result_row);
	MPI_Type_create_resized(result_row, 0, sizeof(int), &result_row_resized);
	MPI_Type_commit(&result_row_resized);

	MPI_Scatter(&A[0][0], 1, row_type_resized, &localA[0], l * k, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b[0], k, MPI_INT, 0, MPI_COMM_WORLD);

	printf("Proces %d je primio vrste iz A:\n", rank);
	for (int i = 0; i < l; i++) 
	{
		for (int j = 0; j < k; j++)
			printf("%d ", localA[i * k + j]);
		printf("\n");
	}

	printf("Proces %d je primio vektor b: ", rank);
	for (int i = 0; i < k; i++)
		printf("%d ", b[i]);
	printf("\n");

	for (int i = 0; i < l; i++) 
	{
		localc[i] = 0;
		for (int j = 0; j < k; j++)
			localc[i] += localA[i * k + j] * b[j];
	}

	printf("Proces %d ima lokalni c: ", rank);
	for (int i = 0; i < l; i++)
		printf("%d ", localc[i]);
	printf("\n");

	MPI_Gather(&localc[0], l, MPI_INT, &c[0], 1, result_row_resized, 0, MPI_COMM_WORLD);

	for (int i = 0; i < k; i++) 
	{
		local_column_sum_A[i] = 1;
		for (int j = 0; j < l; j++)
			local_column_sum_A[i] *= localA[i + j * k];
	}

	MPI_Reduce(&local_column_sum_A[0], &column_sum_A[0], k, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Vektor c: ");
		for (int i = 0; i < m; i++)
			printf("%d ", c[i]);
		printf("\n");

		printf("Proizvodi kolona matrice A: ");
		for (int i = 0; i < k; i++)
			printf("%d ", column_sum_A[i]);
		printf("\n");
	}

	free(localA);
	free(localc);
	//free(local_column_sum_A);
	MPI_Type_free(&row_type);
	MPI_Type_free(&row_type_resized);
	MPI_Type_free(&result_row);
	MPI_Type_free(&result_row_resized);
	MPI_Finalize();
	return 0;
}