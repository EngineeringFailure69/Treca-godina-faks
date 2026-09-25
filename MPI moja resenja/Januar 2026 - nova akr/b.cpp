#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define n 4
#define s 3

int main(int argc, char* argv[])
{
	//size mora biti s*s velicine, odnosno startujem broj procesa u odnosu na to koliko kolona ima matrica A (s), pa na kvadrat, jer u zadatku mi
	//nije bas jasno da li se misli na istu matricu A(nxs) koja je bila pod a, ili da ja treba da kreiram novu matricu A koja ce imati dimenzije 
	//jednake korenu broja procesa, i koja ce biti kvadratna
	int rank, size, A[n][s], *ranks, row, column, *local_column;
	MPI_Group world, diagonal;
	MPI_Comm main_diagonal;
	MPI_Datatype column_type, column_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	ranks = (int*)malloc(s * sizeof(int));
	local_column = (int*)malloc(n * sizeof(int));

	for (int i = 0; i < s; i++)
		ranks[i] = i * s + i; //ovo su rankovi procesa na glavnoj dijagonali, to su rankovi 0, 4, 8 za matricu 4x3

	MPI_Comm_group(MPI_COMM_WORLD, &world); //grupa svih komunikatora
	MPI_Group_incl(world, s, ranks, &diagonal); //kreiram diagonal grupu sa procesima ciji se rankovi nalaze u nizu ranks
	MPI_Comm_create(MPI_COMM_WORLD, diagonal, &main_diagonal); //kreiram main_diagonal komunkator sastavljen od procesa iz grupe diagonal

	row = rank / s;
	column = rank % s;

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < s; j++)
				A[i][j] = i + j;

		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < s; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
	}
	if (row == column) 
	{
		printf("Proces koji pripada glavnoj dijagonali: %d\n", rank);

		MPI_Type_vector(n, 1, s, MPI_INT, &column_type); //uzimam jednu kolonu, n blokova (broj vrsti, odnosno br elemenata svake kolone) velicine 1 sa pomerajem s (broj kolona)
		MPI_Type_commit(&column_type);
		MPI_Type_create_resized(column_type, 0, sizeof(int), &column_type_resized);
		MPI_Type_commit(&column_type_resized);

		MPI_Comm_rank(main_diagonal, &rank);

		MPI_Scatter(&A[0][0], 1, column_type_resized, &local_column[0], n, MPI_INT, 0, main_diagonal);
		printf("Proces %d je primio kolonu: \n", rank);
		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < 1; j++)
				printf("%d ", local_column[i + j]);
			printf("\n");
		}
		MPI_Comm_free(&main_diagonal);
		MPI_Group_free(&diagonal);
		MPI_Group_free(&world);
	}
	free(ranks);
	free(local_column);
	MPI_Type_free(&column_type);
	MPI_Type_free(&column_type_resized);
	MPI_Finalize();
	return 0;
}