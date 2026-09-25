#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include <iostream>

#define M 12

typedef struct
{
	char jmbg[14];
	char ime[15];
	char prezime[20];
	double plata;
}Zaposleni;

int main(int argc, char* argv[])
{
	int rank, size, row, column, n, * ranks, newCommSize, index = 0, newRank;
	MPI_Datatype triangleMatrix;
	MPI_Group matrixGroup, worldGroup;
	MPI_Comm matrixComm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	n = sqrt(size); //dimenzija matrice procesa nxn
	row = rank / n;
	column = rank % n;
	newCommSize = n * (n + 1) / 2;
	ranks = (int*)malloc(newCommSize * sizeof(int));
	Zaposleni zaposleni[M];

	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			ranks[index++] = i * n + j;

	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
	MPI_Group_incl(worldGroup, newCommSize, ranks, &matrixGroup);
	MPI_Comm_create(MPI_COMM_WORLD, matrixGroup, &matrixComm);

	MPI_Datatype tipovi[4] = { MPI_CHAR, MPI_CHAR, MPI_CHAR, MPI_DOUBLE };
	int duzine_blokova[4] = { 14, 15, 20, 1 };
	MPI_Aint offseti[4];
	MPI_Get_address(&(zaposleni[0].jmbg), &offseti[0]);
	MPI_Get_address(&(zaposleni[0].ime), &offseti[1]);
	MPI_Get_address(&(zaposleni[0].prezime), &offseti[2]);
	MPI_Get_address(&(zaposleni[0].plata), &offseti[3]);
	offseti[3] -= offseti[0];
	offseti[2] -= offseti[0];
	offseti[1] -= offseti[0];
	offseti[0] = 0;
	MPI_Type_create_struct(4, duzine_blokova, offseti, tipovi, &triangleMatrix);
	MPI_Type_commit(&triangleMatrix);

	if (column >= row)
	{
		MPI_Comm_rank(matrixComm, &newRank);

		if (newRank == 0)
		{
			for (int i = 0; i < M; i++)
			{
				sprintf(zaposleni[i].jmbg, "%013d", 1000 + i);
				sprintf(zaposleni[i].ime, "Ime%d", i);
				sprintf(zaposleni[i].prezime, "Prezime%d", i);
				zaposleni[i].plata = 1000.0 + i * 100.0;
			}
		}

		Zaposleni* grupaZaposlenih = (Zaposleni*)malloc(M / newCommSize * sizeof(Zaposleni));

		MPI_Scatter(zaposleni, M / newCommSize, triangleMatrix, grupaZaposlenih, M / newCommSize, triangleMatrix, 0, matrixComm);

		printf("Proces sa rankom: %d je dobio zaposlenog: \n", newRank);
		for (int i = 0; i < M / newCommSize; i++)
			printf("%s %s %s %.2f\n", grupaZaposlenih[i].jmbg, grupaZaposlenih[i].ime, grupaZaposlenih[i].prezime, grupaZaposlenih[i].plata);

		free(grupaZaposlenih);
		MPI_Comm_free(&matrixComm);
		MPI_Group_free(&matrixGroup);
		MPI_Group_free(&worldGroup);
	}

	free(ranks);
	MPI_Type_free(&triangleMatrix);
	MPI_Finalize();
	return 0;
}