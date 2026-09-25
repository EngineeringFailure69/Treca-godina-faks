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
	int rank, size, row, column, * rankovi, index = 0, noviRank, novaVelicinaKomunikatora, n;
	MPI_Datatype zaposleniStructType;
	MPI_Group matricaGrupa, worldGrupa;
	MPI_Comm matricaKomunikator;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	n = sqrt(size);
	row = rank / n;
	column = rank % n;
	novaVelicinaKomunikatora = n * (n - 1) / 2;
	rankovi = (int*)malloc(novaVelicinaKomunikatora * sizeof(int));
	Zaposleni zaposleni[M];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < i; j++)
			rankovi[index++] = i * n + j;

	MPI_Comm_group(MPI_COMM_WORLD, &worldGrupa);
	MPI_Group_incl(worldGrupa, novaVelicinaKomunikatora, rankovi, &matricaGrupa);
	MPI_Comm_create(MPI_COMM_WORLD, matricaGrupa, &matricaKomunikator);

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
	MPI_Type_create_struct(4, duzine_blokova, offseti, tipovi, &zaposleniStructType);
	MPI_Type_commit(&zaposleniStructType);

	if (row > column) 
	{
		MPI_Comm_rank(matricaKomunikator, &noviRank);
		if (noviRank == 0) 
		{
			for (int i = 0; i < M; i++) 
			{
				sprintf(zaposleni[i].jmbg, "%013d", 1000 + i);
				sprintf(zaposleni[i].ime, "ime%d", i);
				sprintf(zaposleni[i].prezime, "prezime%d", i);
				zaposleni[i].plata = 1000.0 + i * 100.0;
			}
		}

		Zaposleni* grupaZaposlenih = (Zaposleni*)malloc(M / novaVelicinaKomunikatora * sizeof(Zaposleni));

		MPI_Scatter(zaposleni, M / novaVelicinaKomunikatora, zaposleniStructType, grupaZaposlenih, M / novaVelicinaKomunikatora, zaposleniStructType, 0, matricaKomunikator);

		printf("Proces sa rankom: %d je dobio zaposlenog: \n", noviRank);
		for (int i = 0; i < M / novaVelicinaKomunikatora; i++)
			printf("%s %s %s %.2f\n", grupaZaposlenih[i].jmbg, grupaZaposlenih[i].ime, grupaZaposlenih[i].prezime, grupaZaposlenih[i].plata);

		free(grupaZaposlenih);
		MPI_Comm_free(&matricaKomunikator);
		MPI_Group_free(&worldGrupa);
		MPI_Group_free(&matricaGrupa);
	}

	free(rankovi);
	MPI_Type_free(&zaposleniStructType);
	MPI_Finalize();
	return 0;
}