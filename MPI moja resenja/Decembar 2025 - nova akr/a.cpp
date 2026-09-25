#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define r 8
#define m 4
#define k 6

//Kada proces Pi treba da dobije kolone/vrste (u ovom slucaju vrste) sa indeksima l, l mod p = i, tj. kolone/vrste sa indeksima i, i+p, i+2p,..., i+k-p, ovde samo umesto i stavim rank procesa, 
//p je ukupan broj procesa (size), i samo prolazim kroz ovaj izraz i dobijam indeks kolona/vrsta koje treba da dobije taj proces, i tako idem dok ne dobijem indeks koji 
//je van opsega matrice, odnosno recimo, ako je p=4, a r=8 (broj vrsta matrice A koje se salju) kao u ovom slucaju, onda ce za P0 biti i=0 - krecem  od ranka procesa, (kao po formuli gore), 0+4=4, 0+2*4=8 - ovo je van opsega jer vrste idu 0-7, 
//znaci da ce proces P0 dobiti vrste 0 i 4, i tako se ide redom i za procese P1, P2 i P3.

int main(int argc, char* argv[])
{
	int rank, size, l, A[r][m], B[m][k], C[r][k], * local_C, * local_A;

	MPI_Datatype row, row_resized, result_row, result_row_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = r / size;

	local_A = (int*)malloc(l * m * sizeof(int));
	local_C = (int*)malloc(l * k * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0; i < r; i++)
			for (int j = 0; j < m; j++)
				A[i][j] = i + j;

		for (int i = 0; i < m; i++)
			for (int j = 0; j < k; j++)
				B[i][j] = i + j;

		printf("Matrica A: \n");
		fflush(stdout);
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}

		printf("Matrica B: \n");
		fflush(stdout);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}
	}

	//Reseno upotrebom izvedenog tipa podatka, jer se tako trazi u zadatku
	MPI_Type_vector(l, m, size * m, MPI_INT, &row); //uzimam l blokova velicine m, sa pomerajem size*m 
	MPI_Type_commit(&row);
	MPI_Type_create_resized(row, 0, m * sizeof(int), &row_resized);
	MPI_Type_commit(&row_resized);

	MPI_Type_vector(l, k, size * k, MPI_INT, &result_row);
	MPI_Type_commit(&result_row);
	MPI_Type_create_resized(result_row, 0, k * sizeof(int), &result_row_resized);
	MPI_Type_commit(&result_row_resized);

	MPI_Bcast(&B[0][0], m * k, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&A[0][0], 1, row_resized, &local_A[0], l * m, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Scatter(&A[0][0], l * m, MPI_INT, &local_A[0], l * m, MPI_INT, 0, MPI_COMM_WORLD); //svaki proces dobija po l vrsta matrice A (kad saljem vrstu)
	//ne mora da se pravi custom tip jer su elementi vrste na sukcesivnim memorijskim lokacijama, odnosno kao da saljem obican niz, ovo zapravo ne radi za ovaj zadatak

	printf("Proces %d je dobio vrste matrice A: \n", rank);
	fflush(stdout);
	for (int i = 0; i < l; i++) 
	{
		for (int j = 0; j < m; j++)
			printf("%d ", local_A[i * m + j]);
		printf("\n");
	}
	for (int i = 0; i < l; i++)
	{
		for (int j = 0; j < k; j++)
		{
			local_C[i * k + j] = 0;
			for (int z = 0; z < m; z++)
				local_C[i * k + j] += local_A[i * m + z] * B[z][j];
		}
	}

	MPI_Gather(&local_C[0], l * k, MPI_INT, &C[0][0], 1, result_row_resized, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("C = A*B:\n");
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", C[i][j]);
			printf("\n");
		}
	}

	free(local_A);
	free(local_C);
	MPI_Type_free(&row);
	MPI_Type_free(&row_resized);
	MPI_Type_free(&result_row);
	MPI_Type_free(&result_row_resized);
	MPI_Finalize();
	return 0;
}