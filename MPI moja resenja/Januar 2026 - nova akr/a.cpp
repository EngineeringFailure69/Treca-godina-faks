#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define n 4
#define s 3
#define k 8

//Kada proces Pi treba da dobije kolone/vrste (u ovom slucaju kolone) sa indeksima l, l mod p = i, tj. kolone/vrste sa indeksima i, i+p, i+2p,..., i+k-p, ovde samo umesto i stavim rank procesa, 
//p je ukupan broj procesa (size), i samo prolazim kroz ovaj izraz i dobijam indeks kolona/vrsta koje treba da dobije taj proces, i tako idem dok ne dobijem indeks koji 
//je van opsega matrice, odnosno recimo, ako je p=4, a k=8 (broj kolona matrice B koje se salju) kao u ovom slucaju, onda ce za P0 biti i=0 - krecem  od ranka procesa, (kao po formuli gore), 0+4=4, 0+2*4=8 - ovo je van opsega jer kolone idu 0-7, 
//znaci da ce proces P0 dobiti kolone 0 i 4, i tako se ide redom i za procese P1, P2 i P3. 

int main(int argc, char* argv[])
{
	int rank, size, A[n][s], B[s][k], * C, * local_C, * local_B_column, l, local_row_product[s], row_product = 1, B_row_product[s];

	MPI_Datatype column_type, column_type_resized, result_column_type, result_column_type_resized;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	l = k / size; //broj kolona iz B koji svaki proces dobija

	local_B_column = (int*)malloc(l * s * sizeof(int)); //l*s ide zato sto je l broj kolona iz B koji svaki proces dobija, a s je broj elemenata po koloni
	C = (int*)malloc(n * k * sizeof(int)); //rezultujuca matrica C je dimenzija nxk
	local_C = (int*)malloc(l * n * sizeof(int)); //lokalna matrica C za svaki proces, dimenzije su nxl

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < s; j++)
				A[i][j] = i + j;

		for (int i = 0; i < s; i++)
			for (int j = 0; j < k; j++)
				B[i][j] = i + j;

		//Ispis radi provere
		printf("Matrica A:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < s; j++)
				printf("%d ", A[i][j]);
			printf("\n");
		}
		printf("Matrica B:\n");
		fflush(stdout);
		for (int i = 0; i < s; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", B[i][j]);
			printf("\n");
		}
	}

	//Custom tip za slanje l kolona matrice B svakom procesu
	MPI_Type_vector(s*l, 1, size, MPI_INT, &column_type);
	MPI_Type_commit(&column_type);
	MPI_Type_create_resized(column_type, 0, sizeof(int), &column_type_resized);
	MPI_Type_commit(&column_type_resized);

	//Custom tip za prijem kolona
	MPI_Type_vector(n*l, 1, size, MPI_INT, &result_column_type);
	MPI_Type_commit(&result_column_type);
	MPI_Type_create_resized(result_column_type, 0, sizeof(int), &result_column_type_resized);
	MPI_Type_commit(&result_column_type_resized);

	//Svima saljem matricu A
	MPI_Bcast(&A[0][0], n * s, MPI_INT, 0, MPI_COMM_WORLD);
	//Procesima saljem cutom tip za kolonu
	MPI_Scatter(&B[0][0], 1, column_type_resized, local_B_column, l * s, MPI_INT, 0, MPI_COMM_WORLD);

	printf("Primljene kolone iz B za proces %d:\n", rank);
	fflush(stdout);
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < l; j++)
		{
			printf("%d ", local_B_column[i * l + j]);
			row_product *= local_B_column[i * l + j]; //racunam proizvod za svaku vrstu, ovo i * l + j je indeks za elemente vrste svakog od delova matrica koje je dobio svaki proces
		}
		local_row_product[i] = row_product; //trpam taj proizvod u niz, da bih posle mogao da radim reduce operaciju nad svakim elementom (vrstom) niza posebno
		row_product = 1; //restartujem proizvod
		printf("\n");
	}

	/*printf("Lokalni proizvodi vrsta za proces %d: ", rank);
	for (int i = 0; i < s; i++)
		printf("%d ", local_row_product[i]);
	printf("\n");*/

	for (int i = 0; i < n; i++)
		for (int j = 0; j < l; j++)
		{
			local_C[i * l + j] = 0;
			for (int z = 0; z < s; z++)
				local_C[i * l + j] += A[i][z] * local_B_column[z * l + j];
		}

	MPI_Gather(&local_C[0], l * n, MPI_INT, &C[0], 1, result_column_type_resized, 0, MPI_COMM_WORLD);

	MPI_Reduce(&local_row_product[0], B_row_product, s, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD); //radim operaciju redukcije nad svakim elementom (ima ih s)
	//niza local_row_product, i stavljam ih sve u B_row_product root procesa, tip operacije je MPI_PROD za racunanje proizvoda 

	if (rank == 0)
	{
		printf("A * B:\n");
		fflush(stdout);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < k; j++)
				printf("%d ", C[i * k + j]);
			printf("\n");
		}

		printf("Proizvod svake vrste matrice B je: ");
		fflush(stdout);
		for (int i = 0; i < s; i++)
			printf("%d ", B_row_product[i]);
		printf("\n");
	}

	free(local_C);
	free(C);
	free(local_B_column);
	MPI_Type_free(&column_type);
	MPI_Type_free(&column_type_resized);
	MPI_Type_free(&result_column_type);
	MPI_Type_free(&result_column_type_resized);
	MPI_Finalize();
	return 0;
}