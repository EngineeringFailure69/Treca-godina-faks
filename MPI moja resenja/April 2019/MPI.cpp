#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define n 4
#define k 4
#define m 4

int main(int argc, char* argv[])
{
	int rank, size, A[n][k], B[k][m], C[n][m], i, j, root = 0, lokalna_kolona[n], lokalna_vrsta[m], najmanji_element_iz_B;
	int lokalna_matrica_C[n][m], suma_elemenata_svake_kolone_B[m];
	struct
	{
		int value;
		int rank;
	}in, out;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == root)
	{
		//Inicijalizacija matrica
		for (i = 0; i < n; i++)
			for (j = 0; j < k; j++)
				A[i][j] = i + j;

		for (i = 0; i < k; i++)
			for (j = 0; j < m; j++)
				B[i][j] = 2 * i + j;

		//Ispisivanje radi provere
		printf("Matrica A: \n");
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < k; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}

		printf("\nMatrica B: \n");
		for (i = 0; i < k; i++)
		{
			for (j = 0; j < m; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}

		//Svakom procesu pocev od procesa 1 i kolone 1 matrice A, saljem po jednu kolonu
		for (i = 1; i < k; i++) 
		{
			for (j = 0; j < n; j++)
				lokalna_kolona[j] = A[j][i];
			MPI_Send(lokalna_kolona, n, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		//Prvu kolonu matrice A uzimam za proces 0
		for (i = 0; i < 1; i++)
			for (j = 0; j < n; j++)
				lokalna_kolona[j] = A[j][i];
	}
	else 
	{
		MPI_Recv(lokalna_kolona, n, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
	}
	
	//Ispis radi provere
	//printf("Moj rank: %d, kolona matrice A koju sam dobio: ", rank);
	//for (i = 0; i < n; i++)
	//	printf("%d ", lokalna_kolona[i]);

	//Slanje po jedne vrste matrice B svim procesima
	MPI_Scatter(B, m, MPI_INT, lokalna_vrsta, m, MPI_INT, root, MPI_COMM_WORLD);

	//Ispis radi provere
	//printf("Moj rank: %d, vrsta matrice B koju sam dobio: ", rank);
	//for (i = 0; i < m; i++)
	//	printf("%d ", lokalna_vrsta[i]);

	//Nalazenje procesa sa minimalnom vrednoscu matrice B nakon raspodele elemenata po vrstama
	najmanji_element_iz_B = lokalna_vrsta[0]; //Uzimam prvi element iz svakog procesa kao najmanji, ovo je proizvoljno
	for (i = 1; i < n; i++)
		if (lokalna_vrsta[i] < najmanji_element_iz_B)
			najmanji_element_iz_B = lokalna_vrsta[i];

	in.value = najmanji_element_iz_B;
	in.rank = rank;

	MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);

	if (rank == root)
		printf("\nRank procesa sa najmanjom vrednoscu iz B: %d, najmanja vrednost iz B: %d", out.rank, out.value);

	//Suma elemenata svake kolone matrice B
	MPI_Reduce(lokalna_vrsta, suma_elemenata_svake_kolone_B, m, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

	if (rank == root) 
	{
		printf("\nSuma elemenata svake kolone matrice B: ");
		for (i = 0; i < m; i++)
			printf("%d ", suma_elemenata_svake_kolone_B[i]);
		printf("\n");
	}

	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			lokalna_matrica_C[i][j] = lokalna_vrsta[j] * lokalna_kolona[i];

	MPI_Reduce(lokalna_matrica_C, C, n* m, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

	if (rank == root) 
	{
		printf("\nRezultujuca matrica C:\n");
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
				printf("%d ", C[i][j]);
			printf("\n");
		}
	}

	MPI_Finalize();
	return 0;
}