#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define n 9
#define k 8

int main(int argc, char* argv[])
{
	int rank, size, A[n][k], m, row, column, index = 0, newCommSize, * ranks, newRank, count = 0, *diagonal;
	MPI_Datatype row_type;
	MPI_Group matrixGroup, worldGroup;
	MPI_Comm matrixComm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	m = (int)sqrt(size); //dimenzija matrice procesa
	row = rank / m;
	column = rank % m;
	newCommSize = m * (m - 1) / 2;
	ranks = (int*)malloc(newCommSize * sizeof(int));

	for (int i = 0; i < m; i++)
		for (int j = 0; j < i; j++)
			ranks[index++] = i * m + j;

	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
	MPI_Group_incl(worldGroup, newCommSize, ranks, &matrixGroup);
	MPI_Comm_create(MPI_COMM_WORLD, matrixGroup, &matrixComm);

	count = (n < k) ? n : k; //ako je n < k onda glavna dijagonala ima n elemenata, u suprotnom ima k elemenata

	MPI_Type_vector(count, 1, k + 1, MPI_INT, &row_type);
	MPI_Type_commit(&row_type);

	if (row > column) 
	{
		MPI_Comm_rank(matrixComm, &newRank);
		if (newRank == 0) 
		{
			for (int i = 0; i < n; i++)
				for (int j = 0; j < k; j++)
					A[i][j] = i + j;

			printf("Matrica A:\n");
			fflush(stdout);
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < k; j++)
					printf("%d ", A[i][j]);
				printf("\n");
			}
		}
		MPI_Bcast(&A[0][0], 1, row_type, 0, matrixComm); //salje 1 row_type svima, i upisuje ga u matricu A svakog procesa na poziciju glavne dijagonale, 
		//dok ostali elementi matrice nisu inicijalizovani 

		printf("Proces sa rankom: %d je primio dijagonalu: ", newRank);
		for (int i = 0; i < count; i++) 
			printf("%d ", A[i][i]);
		printf("\n");

		//Moze i bez izvedenog tipa:
		//diagonal = (int*)malloc(count * sizeof(int));
		//if (newRank == 0)
		//{
		//	for (int i = 0; i < n; i++)
		//		for (int j = 0; j < k; j++)
		//			A[i][j] = i + j;

		//	//Uzimam sa glavne dijagonale
		//	for (int i = 0; i < count; i++)
		//		diagonal[i] = A[i][i];

		//	printf("Matrica A:\n");
		//	fflush(stdout);
		//	for (int i = 0; i < n; i++) {
		//		for (int j = 0; j < k; j++)
		//			printf("%d ", A[i][j]);
		//		printf("\n");
		//	}
		//}
		//MPI_Bcast(diagonal, count, MPI_INT, 0, matrixComm); //saljem count elemenata iz niza diagonal svima

		//printf("Proces %d: ", newRank);
		//for (int i = 0; i < count; i++)
		//	printf("%d ", diagonal[i]);
		//printf("\n");

		//free(diagonal);
		MPI_Comm_free(&matrixComm);
		MPI_Group_free(&matrixGroup);
		MPI_Group_free(&worldGroup);
	}

	free(ranks);
	MPI_Type_free(&row_type);
	MPI_Finalize();
	return 0;
}