#define _CRT_SECURE_NO_WARNINGS
#define MSMPI_NO_DEPRECATE_20
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define m 16

int main(int argc, char* argv[])
{
	int rank, size, i, j, a[m], b[m], deo, * lokalni_a, * lokalni_b, root = 0, skalarni_proizvod = 0, lokalni_skalarni_proizvod = 0, * svi_skalari;
	MPI_Status status;
	MPI_Datatype deo_za_svaki_proces;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	deo = m / size;
	lokalni_a = (int*)calloc(deo, sizeof(int));
	lokalni_b = (int*)calloc(deo, sizeof(int));

	if (rank == root) 
	{
		//Inicijalizacija vektora
		for (i = 0; i < m; i++) 
		{
			a[i] = i;
			b[i] = 2 * i;
		}

		printf("Inicijalizovani vektori a i b: \n");
		printf("a: ");
		for (i = 0; i < m; i++)
			printf("%d ", a[i]);
		printf("\nb: ");
		for (i = 0; i < m; i++)
			printf("%d ", b[i]);
		printf("\n");

		//b) Slanje koriscenjem PtP operacija

		//Saljem pocevsi od procesa 1
		//for (i = 1; i < size; i++) 
		//{
		//	for (j = 0; j < deo; j++) 
		//	{
		//		lokalni_a[j] = a[i * deo + j];
		//		lokalni_b[j] = b[i * deo + j];
		//	}
		//	MPI_Send(lokalni_a, deo, MPI_INT, i, 0, MPI_COMM_WORLD);
		//	MPI_Send(lokalni_b, deo, MPI_INT, i, 1, MPI_COMM_WORLD);
		//}

		////Ovo ostaje procesu 0
		//for (j = 0; j < deo; j++) 
		//{
		//	lokalni_a[j] = a[j];
		//	lokalni_b[j] = b[j];
		//}
	}
	//else 
	//{
	//	MPI_Recv(lokalni_a, deo, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
	//	MPI_Recv(lokalni_b, deo, MPI_INT, root, 1, MPI_COMM_WORLD, &status);
	//}

	//Ispis radi provere
	//printf("Moj rank: %d, elementi vektora a koje sam dobio: ", rank);
	//for (i = 0; i < deo; i++)
	//	printf("%d ", lokalni_a[i]);
	//printf("Elementi vektora b koje sam dobio: ");
	//for (i = 0; i < deo; i++)
	//	printf("%d ", lokalni_b[i]);
	//printf("\n");

	//Racunam lokalni skalarni proizvod za svaki proces
	//for (i = 0; i < deo; i++)
	//	lokalni_skalarni_proizvod += lokalni_a[i] * lokalni_b[i];

	//U root procesu , u okviru niza svi skalari skupljam sve lokalne skalarne proizvode drugih procesa, i posle samo sumiram sve elemente niza 
	// svi skalari, i smestam u promenljivu skalarni proizvod
	//if (rank == root) 
	//{
	//	svi_skalari = (int*)calloc(size, sizeof(int));
	//	svi_skalari[root] = lokalni_skalarni_proizvod;
	//	for (i = 1; i < size; i++)
	//		MPI_Recv(&svi_skalari[i], 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
	//	for (i = 0; i < size; i++)
	//		skalarni_proizvod += svi_skalari[i];
	//	free(svi_skalari);
	//}
	//else 
	//{
	//	MPI_Send(&lokalni_skalarni_proizvod, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	//}

	//a) koriscenjem grupnih operacija
	//Kreiam custom tip koji salje po p/m elemenata oba vektora svakom procesu
	MPI_Type_vector(deo, 1, 1, MPI_INT, &deo_za_svaki_proces);
	MPI_Type_commit(&deo_za_svaki_proces);
	//Saljem elemente
	MPI_Scatter(a, 1, deo_za_svaki_proces, lokalni_a, deo, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Scatter(b, 1, deo_za_svaki_proces, lokalni_b, deo, MPI_INT, root, MPI_COMM_WORLD);

	//Ispis radi provere
	printf("Moj rank: %d, elementi vektora a koje sam dobio: ", rank);
	for (i = 0; i < deo; i++)
		printf("%d ", lokalni_a[i]);
	printf("Elementi vektora b koje sam dobio: ");
	for (i = 0; i < deo; i++)
		printf("%d ", lokalni_b[i]);
	printf("\n");

	//Racuman lokalni skalarni proizvod za svaki proces
	for (i = 0; i < deo; i++)
		lokalni_skalarni_proizvod += lokalni_a[i] * lokalni_b[i];

	//Skupljam sve lokalne skalarne proizvode u root proces, sumiram  ih i trpam  u  promenljivu skalarni_proizvod
	MPI_Reduce(&lokalni_skalarni_proizvod, &skalarni_proizvod, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

	if (rank == root) 
	{
		printf("Skalarni proizvod ova dva vektora: %d\n", skalarni_proizvod);
	}

	free(lokalni_a);
	free(lokalni_b);
	MPI_Finalize();
	return 0;
}