#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define N 20

typedef struct 
{
	int brojIndeksa;
	char ime[20];
	char prezime[20];
	float prosek;
}Student;

int main(int argc, char* argv[])
{
	int rank, size, row, column, n, *ranks;
	MPI_Comm DIAG;
	MPI_Group world, diagonal;
	MPI_Datatype student_type;
	MPI_Aint offseti[4];
	Student* studenti = (Student*)malloc(N * sizeof(Student));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	n = sqrt(size); //size je broj procesa za koji startujemo, ako uzmemo u obzir da su procesi rasporedjeni kao kvadratna mreza (matrica), to znaci da 
	//je matrica dimenzija nxn, odnosno da n racunam kao koren iz broja procesa
	row = rank / n;
	column = rank % n;
	ranks = (int*)malloc(n * sizeof(int)); //rezervisem niz za rankove procesa na glavnoj diagonali

	for (int i = 0; i < n; i++)
		ranks[i] = i * n + i;

	MPI_Comm_group(MPI_COMM_WORLD, &world);
	MPI_Group_incl(world, n, ranks, &diagonal);
	MPI_Comm_create(MPI_COMM_WORLD, diagonal, &DIAG);

	if (rank == 0)
	{
		for (int i = 0; i < N; i++)
		{
			studenti[i].brojIndeksa = 1000 + i;
			sprintf(studenti[i].ime, "Ime%d", i);
			sprintf(studenti[i].prezime, "Prezime%d", i);
			studenti[i].prosek = 6.0 + (float)(i % 5);
		}
	}

	MPI_Datatype tipovi[4] = { MPI_INT, MPI_CHAR, MPI_CHAR, MPI_FLOAT };
	int duzine_blokova[4] = { 1, 20, 20, 1 };
	MPI_Get_address(&(studenti[0].brojIndeksa), &offseti[0]);
	MPI_Get_address(&(studenti[0].ime), &offseti[1]);
	MPI_Get_address(&(studenti[0].prezime), &offseti[2]);
	MPI_Get_address(&(studenti[0].prosek), &offseti[3]);
	offseti[3] = offseti[3] - offseti[0];
	offseti[2] = offseti[2] - offseti[0];
	offseti[1] = offseti[1] - offseti[0];
	offseti[0] = 0;
	MPI_Type_create_struct(4, duzine_blokova, offseti, tipovi, &student_type);
	MPI_Type_commit(&student_type);

	if (row == column) 
	{
		MPI_Comm_rank(DIAG, &rank);
		Student* grupa_studenata = (Student*)malloc(N / n * sizeof(Student));

		MPI_Scatter(studenti, N / n, student_type, grupa_studenata, N / n, student_type, 0, DIAG);
		for (int i = 0; i < N / n; i++)
		{
			printf("Proces %d: ", rank);
			printf("%d %s %s %.2f\n", grupa_studenata[i].brojIndeksa, grupa_studenata[i].ime,
				grupa_studenata[i].prezime, grupa_studenata[i].prosek);
		}

		MPI_Comm_free(&DIAG);
		MPI_Group_free(&diagonal);
		MPI_Group_free(&world);
		free(grupa_studenata);
	}

	free(ranks);
	free(studenti);
	MPI_Type_free(&student_type);
	MPI_Finalize();
	return 0;
}