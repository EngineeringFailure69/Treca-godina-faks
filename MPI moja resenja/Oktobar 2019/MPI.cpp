#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define n 4
#define k 4
#define s 2

int main(int argc, char* argv[])
{
	int rank, size, A[n][k], b[k], c[n], i, j, l, maksimalni_element_iz_A, root = 0, lokalna_matrica[n][s];
	int	suma_elemenata_svake_vrste_A[n], lokalni_vektor_b[s], lokalna_suma_elemenata_vrste[n], lokalni_vektor_c[n];
	struct
	{
		int rank;
		int value;
	}in, out;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == root) 
	{
		//Inicijalizacija matrice
		for (i = 0; i < n; i++)
			for (j = 0; j < k; j++)
				A[i][j] = i + j;

		//Inicijalizacija vektora
		for (i = 0; i < k; i++)
			b[i] = 2 * i;

		//Provera
		for (i = 0; i < n; i++) 
		{
			for (j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}

		//Provera
		printf("\n");
		for (i = 0; i < k; i++)
			printf("%d ", b[i]);

		//Od prvog procesa krece slanje
		for (i = 1; i < size; i++) 
		{
			for (j = 0; j < s; j++)
				for (l = 0; l < n; l++)
					lokalna_matrica[l][j] = A[l][j+s*i];
			MPI_Send(lokalna_matrica, n * s, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		//Proces 0 dobija svoje dve kolone
		for (i = 0; i < s; i++)
			for (j = 0; j < n; j++)
				lokalna_matrica[j][i] = A[j][i];
	}
	else 
	{
		MPI_Recv(lokalna_matrica, n* s, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
	}

	//Provera
	//printf("\nMoj rank: %d, matrica koju sam dobio: \n", rank);
	//for (i = 0; i < n; i++)
	//{
	//	for (j = 0; j < s; j++)
	//		printf("%d ", lokalna_matrica[i][j]);
	//	printf("\n");
	//}

	MPI_Scatter(b, s, MPI_INT, lokalni_vektor_b, s, MPI_INT, root, MPI_COMM_WORLD); //slanje s elemenata iz vektora b

	//Provera
	//printf("\nMoj rank: %d, elementi b[k] koju sam dobio: ", rank);
	//for (i = 0; i < s; i++)
	//{
	//	printf("%d ", lokalni_vektor_b[i]);
	//}

	//Maksimalni element iz A
	maksimalni_element_iz_A = lokalna_matrica[0][0];
	for (i = 0; i < n; i++)
	{
		lokalna_suma_elemenata_vrste[i] = 0;
		for (j = 0; j < s; j++)
		{
			if (lokalna_matrica[i][j] > maksimalni_element_iz_A)
				maksimalni_element_iz_A = lokalna_matrica[i][j];
			lokalna_suma_elemenata_vrste[i] += lokalna_matrica[i][j];
		}
	}

	in.value = maksimalni_element_iz_A;
	in.rank = rank;

	MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
	MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

	//Suma elemenata svake vrste matrice A 
	MPI_Reduce(lokalna_suma_elemenata_vrste, suma_elemenata_svake_vrste_A, n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

	//Racunanje vektora c
	for (i = 0; i < n; i++) 
	{
		lokalni_vektor_c[i] = 0;
		for (j = 0; j < s; j++)
			lokalni_vektor_c[i] += lokalna_matrica[i][j] * lokalni_vektor_b[j];
	}

	MPI_Reduce(lokalni_vektor_c, c, n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

	if (rank == out.rank) 
	{
		printf("\nRank procesa sa najvecom vrednoscu iz A: %d, najveca vrednost iz A: %d", out.rank, out.value);
		printf("\nSuma elemenata svake vrste matrice A: ");
		for (i = 0; i < n; i++)
			printf("%d ", suma_elemenata_svake_vrste_A[i]);
		printf("\nVektor c: ");
		for (i = 0; i < n; i++)
			printf("%d ", c[i]);
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}