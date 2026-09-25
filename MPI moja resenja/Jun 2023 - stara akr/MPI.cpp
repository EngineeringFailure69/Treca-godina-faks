#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define N 6

typedef struct 
{
	int id;
	char ime[20];
	char prezime[20];
	float plata;
}Zaposleni;

int main(int argc, char* argv[])
{
	int rank, size, idMin, l;
	float plataMin;
	MPI_Datatype zaposleniType;
	Zaposleni* sviZaposleni, * grupaZaposlenih;
	struct 
	{
		float plataMin;
		int idMin;
	}ulaz, izlaz;

	int duzine_blokova[4] = { 1, 20, 20, 1 };
	MPI_Datatype tipovi[4] = { MPI_INT, MPI_CHAR, MPI_CHAR, MPI_FLOAT };
	MPI_Aint offseti[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = N / size;

	sviZaposleni = (Zaposleni*)malloc(N * sizeof(Zaposleni));
	grupaZaposlenih = (Zaposleni*)malloc(l * sizeof(Zaposleni));

	if (rank == 0)
	{
		sviZaposleni[0] = { 1, "Pera", "Peric", 50000 };
		sviZaposleni[1] = { 2, "Mika", "Mikic", 60000 };
		sviZaposleni[2] = { 3, "Zika", "Zikic", 55000 };
		sviZaposleni[3] = { 4, "Laza", "Lazic", 70000 };
		sviZaposleni[4] = { 5, "Sima", "Simic", 65000 };
		sviZaposleni[5] = { 6, "Dule", "Dulic", 72000 };
	}

	MPI_Get_address(&(sviZaposleni[0].id), &offseti[0]);
	MPI_Get_address(&(sviZaposleni[0].ime), &offseti[1]);
	MPI_Get_address(&(sviZaposleni[0].prezime), &offseti[2]);
	MPI_Get_address(&(sviZaposleni[0].plata), &offseti[3]);
	offseti[3] = offseti[3] - offseti[0];
	offseti[2] = offseti[2] - offseti[0];
	offseti[1] = offseti[1] - offseti[0];
	offseti[0] = 0;
	MPI_Type_create_struct(4, duzine_blokova, offseti, tipovi, &zaposleniType);
	MPI_Type_commit(&zaposleniType);

	MPI_Scatter(sviZaposleni, l, zaposleniType, grupaZaposlenih, l, zaposleniType, 0, MPI_COMM_WORLD);

	idMin = grupaZaposlenih[0].id;
	plataMin = grupaZaposlenih[0].plata;

	for (int i = 1; i < l; i++) 
	{
		if (grupaZaposlenih[i].plata < plataMin)
		{
			idMin = grupaZaposlenih[i].id;
			plataMin = grupaZaposlenih[i].plata;
		}
	}

	ulaz.plataMin = plataMin;
	ulaz.idMin = idMin;

	MPI_Reduce(&ulaz, &izlaz, 1, MPI_FLOAT_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Zaposleni sa najmanjom platom ima ID: %d i platu: %.2f\n", izlaz.idMin, izlaz.plataMin);
	}

	free(sviZaposleni);
	free(grupaZaposlenih);
	MPI_Type_free(&zaposleniType);
	MPI_Finalize();
	return 0;
}