//Copyright 2021 Dumitrescu Rares Matei 311CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ALLOC 'L'
#define QUIT 'Q'
#define DIMENSION 'D'
#define PRINT 'P'
#define REDIMENSION 'C'
#define MULTIPLICATION 'M'
#define SORT 'O'
#define TRANSPOSE 'T'
#define FREE 'F'
#define STRASSEN 'S'
#define MOD 10007
typedef struct {
	int **matrix, m, n;
	int s;
} my_struct;

void start_program(int tester, my_struct *list, int index, int capacity);
int **alloc_matrix(int m, int n);
void realloc_mem(my_struct **list, int capacity);
void print_matrix(my_struct *list, int index);
void free_mem(my_struct *list, int index);
void redim_matrix(my_struct *list, int index, int index_list);
void free_position(my_struct *list, int index, int l, int c);
void multip_matrices(my_struct *list, int pos1, int pos2, int index);
void sum_elem_mat(my_struct *list, int index);
void sort_matrices(my_struct *list, int index);
void swap_matrices(my_struct *list, int pos1, int pos2);
void transpose(my_struct *list, int index);
void start_free(my_struct *list, int index, int final);
int **subtract(int **matrix1, int **matrix2, int n);
int **add(int **matrix1, int **matrix2, int n);
int **alloc_tmp_mat(int n);
void free_tmp_mat(int **tmp, int n);
int **start_strassen(int **mat1, int **mat2, int k);
void strassen(my_struct **list, int pos1, int pos2, int *index, int *capacity);
void free_matrices(int **A11, int **A12, int **A21, int **A22,
				   int **B11, int **B12, int **B21, int **B22, int **P1,
				   int **P2, int **P3, int **P4, int **P5,
				   int **P6, int **P7, int **C11, int **C12,
				   int **C21, int **C22, int k);

int main(void)
{
	my_struct *list;
	int tester = 1;
	int capacity, index;
	index = 0;
	capacity = 51;
	list = (my_struct *)malloc(capacity * sizeof(my_struct));
	if (!list) {
		fprintf(stderr, "Malloc failed\n");
		return -1;
	}
	start_program(tester, list, index, capacity);
	return 0;
}

//functie ce contine toate operatiile posibile
void start_program(int tester, my_struct *list, int index, int capacity)
{
	int pos1, pos2, i; char operator;
	while (tester) {
		scanf("%c", &operator);
		switch (operator) {
		case QUIT:
			tester = 0;
			free_mem(list, index);//dezalocare memorie
			break;
		case ALLOC:
			if (index == capacity)
				capacity = capacity * 2, realloc_mem(&list, capacity);
			scanf("%d%d", &list[index].m, &list[index].n);
			list[index].matrix = alloc_matrix(list[index].m, list[index].n);
			index++;//actualizarea ultimei pozitii din lista
			break;
		case DIMENSION:
			scanf("%d", &i);
			if (i >= index || i < 0)//matrice inexistenta
				printf("No matrix with the given index\n");
			else
				printf("%d %d\n", list[i].m, list[i].n);
			break;
		case PRINT:
			scanf("%d", &i);
			if (i >= index || i < 0)
				printf("No matrix with the given index\n");
			else
				print_matrix(list, i);
			break;
		case REDIMENSION:
			scanf("%d", &i), redim_matrix(list, i, index);
			break;
		case MULTIPLICATION:
			scanf("%d%d", &pos1, &pos2);
			if (pos1 >= index || pos1 < 0 || pos2 >= index || pos2 < 0) {
				printf("No matrix with the given index\n");
			} else {
				if (list[pos1].n != list[pos2].m) {//compatibilitatea matricelor
					printf("Cannot perform matrix multiplication\n");
				} else {
					index++;
					if (index == capacity)
						capacity = capacity * 2, realloc_mem(&list, capacity);
					multip_matrices(list, pos1, pos2, index - 1); //multiplicare
				}
			}
			break;
		case SORT:
			sum_elem_mat(list, index), sort_matrices(list, index);
			//sume elemente matrici si sortare dupa sume
			break;
		case TRANSPOSE:
			scanf("%d", &i);
			if (i >= index || i < 0)
				printf("No matrix with the given index\n");
			else
				transpose(list, i);
			break;
		case FREE:
			scanf("%d", &i);
			if (i >= index || i < 0) {
				printf("No matrix with the given index\n");
			} else {
				start_free(list, i, index), index--;//nr de matrici actualizat
				if (index < capacity / 2)
					capacity /= 2, realloc_mem(&list, capacity);
					//pt eficienta se realoca memorie
			}
			break;
		case STRASSEN:
			scanf("%d%d", &pos1, &pos2);
			strassen(&list, pos1, pos2, &index, &capacity);
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
		getchar();
	}
}

//functie ce returneaza o matrice citita de la tastatura
int **alloc_matrix(int m, int n)
{
	int **matrix;
	matrix = (int **)malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++) {
		matrix[i] = (int *)malloc(n * sizeof(int));
		for (int j = 0; j < n; j++)
			scanf("%d", &matrix[i][j]);
	}
	return matrix;
}

//functie ce aloca dinamic mai multa memorie listei
void realloc_mem(my_struct **list, int capacity)
{
	my_struct *tmp;
	tmp = (my_struct *)realloc(*list, capacity * sizeof(my_struct));
	if (!tmp) {
		fprintf(stderr, "Realloc failed\n");
		free(*list);
		*list = NULL;
		return;
	}
	*list = tmp;
}

//functie ce printeaza o matrice de la indexul dat
void print_matrix(my_struct *list, int index)
{
	for (int i = 0; i < list[index].m; i++) {
		for (int j = 0; j < list[index].n; j++)
			printf("%d ", list[index].matrix[i][j]);
		printf("\n");
	}
}

//functie ce dezaloca memoria la final
void free_mem(my_struct *list, int index)
{
	for (int k = 0; k < index; k++) {
		for (int i = 0; i < list[k].m; i++)
			free(list[k].matrix[i]);
		free(list[k].matrix);
	}
	free(list);
}

//functie ce redimensioneaza o matrice de la un index dat
void redim_matrix(my_struct *list, int index, int index_list)
{
	int l, c, **matrix, *line, *column;
	scanf("%d", &l);
	line = (int *)malloc(l * sizeof(int));
	if (!line) {
		fprintf(stderr, "Malloc failed\n");
		return;
	}

	int i = 0;
	while (i < l) {
		scanf("%d", &line[i]);
		i++;
	}
	scanf("%d", &c);
	column = (int *)malloc(c * sizeof(int));
	if (!column) {
		fprintf(stderr, "Malloc failed\n");
		return;
	}
	i = 0;
	while (i < c) {
		scanf("%d", &column[i]);
		i++;
	}
	if (index >= index_list || index < 0) {
		printf("No matrix with the given index\n");
		free(line);
		free(column);
		return;
	}
	matrix = (int **)malloc(l * sizeof(int *));
	if (!matrix) {
		fprintf(stderr, "Malloc failed\n");
		return;
	}
	for (int k = 0; k < l; k++)
		matrix[k] = (int *)malloc(c * sizeof(int));
	if (!matrix) {
		fprintf(stderr, "Malloc failed\n");
		return;
	}
	//am creat 2 vectori ce vor retine pozitiile liniilor si coloanelor
	for (i = 0; i < l; i++)
		for (int j = 0; j < c; j++)
			matrix[i][j] = list[index].matrix[line[i]][column[j]];
	//am creat matricea ce este de fapt matricea redimensionata

	//este nevoie sa eliberam pozitia index din lista si sa punem noua matrice
	free_position(list, index, l, c);
	for (i = 0; i < l; i++)
		for (int j = 0; j < c; j++)
			list[index].matrix[i][j] = matrix[i][j];

	for (i = 0; i < l; i++)
		free(matrix[i]);
	free(matrix);
	free(line);
	free(column);
}

//functie ce elibereaza o pozitie din lista si aloca alta memorie
void free_position(my_struct *list, int index, int l, int c)
{
	for (int i = 0; i < list[index].m; i++)
		free(list[index].matrix[i]);
	free(list[index].matrix);
	list[index].m = l;
	list[index].n = c; //actualizarea dimensiunilor matricei
	list[index].matrix = (int **)malloc(l * sizeof(int *));
	for (int i = 0; i < l; i++)
		list[index].matrix[i] = (int *)malloc(c * sizeof(int));
}

//functie ce face inmultirea a doua matrici
void multip_matrices(my_struct *list, int pos1, int pos2, int index)
{
	list[index].m = list[pos1].m;
	list[index].n = list[pos2].n; //setarea dimensiunilor matricei noi create
	list[index].matrix = (int **)malloc(list[index].m * sizeof(int *));
	for (int i = 0; i < list[index].m; i++)
		list[index].matrix[i] = (int *)calloc(list[index].n, sizeof(int));
	//se foloseste calloc pentru a initializa toate elementele cu 0
	for (int i = 0; i < list[index].m; i++) {
		for (int j = 0; j < list[index].n; j++) {
			int k = 0;
			while (k < list[pos1].n) {
				//algoritmul de parcurgere pe linie si coloana
				int p;
				p = (list[pos1].matrix[i][k] * list[pos2].matrix[k][j]) % MOD;
				list[index].matrix[i][j] = (list[index].matrix[i][j] + p) % MOD;
				k++;
			}
			if (list[index].matrix[i][j] < 0)
				list[index].matrix[i][j] += MOD;
		}
	}
}

//functie ce calculeaza suma elementelor fiecarei matrici
void sum_elem_mat(my_struct *list, int index)
{
	for (int k = 0; k < index; k++) {
		list[k].s = 0; //se initializeaza suma fiecarei matrici cu 0
		for (int i = 0; i < list[k].m; i++)
			for (int j = 0; j < list[k].n; j++) {
				list[k].s += list[k].matrix[i][j];
				list[k].s = list[k].s % MOD;
			}
		if (list[k].s < 0)
			list[k].s += MOD;
	}
}

//functie ce sorteaza matricile in ordine cresc. dupa suma elementelor
void sort_matrices(my_struct *list, int index)
{
	for (int k = 0; k < index; k++)
		for (int l = k + 1; l < index; l++)
			if (list[k].s > list[l].s)
				swap_matrices(list, k, l);
}

//functie ce interschimba 2 matrici din lista
void swap_matrices(my_struct *list, int pos1, int pos2)
{
	int **tmp;
	tmp = (int **)malloc(list[pos1].m * sizeof(int *));
	for (int i = 0; i < list[pos1].m; i++)
		tmp[i] = (int *)malloc(list[pos1].n * sizeof(int));
	for (int i = 0; i < list[pos1].m; i++)
		for (int j = 0; j < list[pos1].n; j++)
			tmp[i][j] = list[pos1].matrix[i][j];//copiere matrice pos1 in tmp
	int m, n, s;
	s = list[pos1].s;
	m = list[pos1].m;
	n = list[pos1].n; //retinere dimensiuni matrice pos1
	for (int i = 0; i < list[pos1].m; i++)
		free(list[pos1].matrix[i]);
	free(list[pos1].matrix);
	//am eliberat memoria de la matricea de pe pos1

	list[pos1].m = list[pos2].m;
	list[pos1].n = list[pos2].n; //actualizare dimensiuni
	list[pos1].s = list[pos2].s;

	//se aloca alta memorie matricei cat are cea de pe pos2
	list[pos1].matrix = (int **)malloc(list[pos1].m * sizeof(int *));
	for (int i = 0; i < list[pos1].m; i++)
		list[pos1].matrix[i] = (int *)malloc(list[pos1].n * sizeof(int));
	for (int i = 0; i < list[pos1].m; i++)
		for (int j = 0; j < list[pos1].n; j++)
			list[pos1].matrix[i][j] = list[pos2].matrix[i][j];
			//copiere matrice pos2 in pos1

	for (int i = 0; i < list[pos2].m; i++)
		free(list[pos2].matrix[i]);
	free(list[pos2].matrix);//eliberare memorie pos2
	list[pos2].m = m;
	list[pos2].n = n; //actualizare dimensiuni
	list[pos2].s = s;
	list[pos2].matrix = (int **)malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++)
		list[pos2].matrix[i] = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			list[pos2].matrix[i][j] = tmp[i][j];//copiere tmp in pos2
	for (int i = 0; i < m; i++)
		free(tmp[i]);
	free(tmp);
}

//functie ce transpune o matrice de la indexul dat
void transpose(my_struct *list, int index)
{
	int **tmp;
	tmp = (int **)malloc(list[index].n * sizeof(int *));
	for (int i = 0; i < list[index].n; i++)
		tmp[i] = (int *)malloc(list[index].m * sizeof(int));
	//a fost alocata memorie pentru matricea temporala
	for (int i = 0; i < list[index].m; i++)
		for (int j = 0; j < list[index].n; j++)
			tmp[j][i] = list[index].matrix[i][j];
	//tmp contine matricea transpusa
	int m = list[index].n;
	int n = list[index].m; //actualizare dimensiuni
	for (int i = 0; i < list[index].m; i++)
		free(list[index].matrix[i]);
	free(list[index].matrix);
	//eliberare memorie matrice din lista

	list[index].matrix = (int **)malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++)
		list[index].matrix[i] = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			list[index].matrix[i][j] = tmp[i][j];
	//matricea a fost transpusa
	list[index].m = m;
	list[index].n = n;//actualizare dimensiuni matrice
	for (int i = 0; i < m; i++)
		free(tmp[i]);
	free(tmp); //elibare memorie matrice temporala
}

void start_free(my_struct *list, int index, int final)
{
	int k;
	for (k = index; k < final - 1; k++) {
		int m, n;
		m = list[k + 1].m;
		n = list[k + 1].n;
		free_position(list, k, m, n);
		//memoria matricei de pe indexul k  a fost actualizata
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				list[k].matrix[i][j] = list[k + 1].matrix[i][j];
		//lista a fost actualizata dupa sterge cu exceptia ultimei matrici
	}
	for (int i = 0; i < list[k].m; i++)
		free(list[k].matrix[i]);
	free(list[k].matrix);
	//memoria de pe ultima pozitie a fost eliberata
}

//functie ce face strassen daca matricile sunt compatibile si exista
void strassen(my_struct **list, int pos1, int pos2, int *index, int *capacity)
{
	if (pos1 >= *index || pos1 < 0 || pos2 >= *index || pos2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*list)[pos1].n != (*list)[pos2].m) {//compatibilitatea matricelor
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	if (*index == *capacity)
		*capacity *= 2, realloc_mem(&(*list), *capacity);
	int n = (*list)[pos1].m;
	(*list)[*index].m = n;
	(*list)[*index].n = n;
	int k = n;
	(*list)[*index].matrix = start_strassen((*list)[pos1].matrix,
											(*list)[pos2].matrix, k);
	(*index) = (*index) + 1;
}

//funcite ce face algoritmul strassen propriu-zis
int **start_strassen(int **mat1, int **mat2, int k)
{
	if (k == 1) {
		int **mat_final = alloc_tmp_mat(k);
		mat_final[0][0] = mat1[0][0] * mat2[0][0];
		return mat_final;
	}
	int **mat_final = alloc_tmp_mat(k);
	k /= 2;
	int **A11 = alloc_tmp_mat(k);
	int **A12 = alloc_tmp_mat(k);
	int **A21 = alloc_tmp_mat(k);
	int **A22 = alloc_tmp_mat(k);
	int **B11 = alloc_tmp_mat(k);
	int **B12 = alloc_tmp_mat(k);
	int **B21 = alloc_tmp_mat(k);
	int **B22 = alloc_tmp_mat(k);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			A11[i][j] = mat1[i][j];
			A12[i][j] = mat1[i][k + j];
			A21[i][j] = mat1[k + i][j];
			A22[i][j] = mat1[k + i][k + j];
			B11[i][j] = mat2[i][j];
			B12[i][j] = mat2[i][k + j];
			B21[i][j] = mat2[k + i][j];
			B22[i][j] = mat2[k + i][k + j];
		}
	int **aux1, **aux2;
	aux1 = subtract(B12, B22, k);
	int **P1 = start_strassen(A11, aux1, k);
	free_tmp_mat(aux1, k);
	aux1 = add(A11, A12, k);
	int **P2 = start_strassen(aux1, B22, k);
	free_tmp_mat(aux1, k);
	aux1 = add(A21, A22, k);
	int **P3 = start_strassen(aux1, B11, k);
	free_tmp_mat(aux1, k);
	aux1 = subtract(B21, B11, k);
	int **P4 = start_strassen(A22, aux1, k);
	free_tmp_mat(aux1, k);
	aux1 = add(A11, A22, k);
	aux2 = add(B11, B22, k);
	int **P5 = start_strassen(aux1, aux2, k);
	free_tmp_mat(aux1, k);
	free_tmp_mat(aux2, k);
	aux1 = subtract(A12, A22, k);
	aux2 = add(B21, B22, k);
	int **P6 = start_strassen(aux1, aux2, k);
	free_tmp_mat(aux1, k);
	free_tmp_mat(aux2, k);
	aux1 = subtract(A11, A21, k);
	aux2 = add(B11, B12, k);
	int **P7 = start_strassen(aux1, aux2, k);
	free_tmp_mat(aux1, k);
	free_tmp_mat(aux2, k);
	aux1 = add(P5, P4, k);
	aux2 = add(aux1, P6, k);
	int **C11 = subtract(aux2, P2, k);
	int **C12 = add(P1, P2, k);
	int **C21 = add(P3, P4, k);
	free_tmp_mat(aux2, k);
	free_tmp_mat(aux1, k);
	aux1 = add(P5, P1, k);
	aux2 = subtract(aux1, P3, k);
	int **C22 = subtract(aux2, P7, k);
	free_tmp_mat(aux2, k);
	free_tmp_mat(aux1, k);

	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			mat_final[i][j] = C11[i][j];
			mat_final[i][j + k] = C12[i][j];
			mat_final[k + i][j] = C21[i][j];
			mat_final[k + i][k + j] = C22[i][j];
		}
	free_matrices(A11, A12, A21, A22, B11, B12, B21, B22, P1,
				  P2, P3, P4, P5, P6, P7, C11, C12, C21, C22, k);

	return mat_final;
}

//functie ce dezaloca memoriile matricelor temporale
void free_matrices(int **A11, int **A12, int **A21, int **A22,
				   int **B11, int **B12, int **B21, int **B22, int **P1,
				   int **P2, int **P3, int **P4, int **P5,
				   int **P6, int **P7, int **C11, int **C12,
				   int **C21, int **C22, int k)
{
	free_tmp_mat(A11, k);
	free_tmp_mat(A12, k);
	free_tmp_mat(A21, k);
	free_tmp_mat(A22, k);
	free_tmp_mat(B11, k);
	free_tmp_mat(B12, k);
	free_tmp_mat(B21, k);
	free_tmp_mat(B22, k);
	free_tmp_mat(P1, k);
	free_tmp_mat(P2, k);
	free_tmp_mat(P3, k);
	free_tmp_mat(P4, k);
	free_tmp_mat(P5, k);
	free_tmp_mat(P6, k);
	free_tmp_mat(P7, k);
	free_tmp_mat(C11, k);
	free_tmp_mat(C12, k);
	free_tmp_mat(C21, k);
	free_tmp_mat(C22, k);
}

//functie de adunare necesara pentru strassen
int **add(int **matrix1, int **matrix2, int n)
{
	int **tmp = alloc_tmp_mat(n);
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			tmp[i][j] = (matrix1[i][j] + matrix2[i][j]) % MOD;
		if (tmp[i][j] < 0)
			tmp[i][j] += MOD;
		}
	}
	return tmp;
}

//functie de scadere necesara pentru strassen
int **subtract(int **matrix1, int **matrix2, int n)
{
	int **tmp = alloc_tmp_mat(n);
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			tmp[i][j] = (matrix1[i][j] - matrix2[i][j]) % MOD;
			if (tmp[i][j] < 0)
				tmp[i][j] += MOD;
			}
	}

	return tmp;
}

//functie ce aloca o matrice temporala
int **alloc_tmp_mat(int n)
{
	int **tmp;
	tmp = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++)
		tmp[i] = (int *)malloc(n * sizeof(int));
	return tmp;
}

//functie ce elibereaza memoria unei matrici temporale
void free_tmp_mat(int **tmp, int n)
{
	for (int i = 0; i < n; i++)
		free(tmp[i]);
	free(tmp);
}
