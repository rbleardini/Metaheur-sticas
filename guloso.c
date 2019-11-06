#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXCHAR 1000
//#define MAX 9999 //br17 p43
#define MAX 40 //rbg
//#define MAX 9999999 // ft, kro, ry
//#define MAX 100000000 // ftv

int** copia_matriz(int** mat_original, int num_cidades);
int** matriz_cidades(char* filename, int num_cidades);
int* copia_lista(int* lista_original, int size);

//Auxiliares
int pertence(int elem, int* solution, int size);
int pega_menor(int* lista, int size, int count, int* solution);
int pega_aleatorio(int* lista, int size, int count, int* solution, int i_grasp, double alpha);
int elem_repetido(int* lista, int size);
int e_valida(int** matriz, int* solution, int size, int count);
int menor_elem(int* lista, int size, int* solution);
int maior_elem(int* lista, int size, int* solution);
int compare( const void* a, const void* b);
int diferenca_simetrica(int* a, int* b, int size);
int menor_diferenca(int* solution, int size, int** elite_set, int n_atual);
int pega_pesada(int size, int**matriz, int** elite_set, int nE);
int pega_menor_pesada(int* solution, int size, int**matriz, int** elite_set, int nE);

int* guloso_grasp(int** matriz_entrada, int size, int start, double alpha, int i_grasp);
int peso_total(int* solution, int size, int**matriz);
int* opt2_swap(int* solution, int i, int k, int size);
int* busca_local(int** matriz, int* solution, int size);
int* grasp(int** matriz_entrada, int size, int num_restart, int target, double alpha);
int** update_elite_set(int* solution, int size, int** matriz, int** elite_set, int parameter_E, int nE, int n_atual);


int** copia_matriz(int** matriz, int num_cidades)
{
	int** saida = malloc(sizeof(int*)*num_cidades);
	int i = 0, count = 0, linha = 0, coluna = 0;
	for(i = 0; i < num_cidades; i++)
	{
		saida[i] = malloc(sizeof(int)*num_cidades);
	}
	while (count != (num_cidades * num_cidades))
    {  	
    	count++;
    	if(coluna > (num_cidades-1))
    	{
    		linha++;
    		coluna = 0;
    		if(linha > (num_cidades-1))
    		{
    			linha = 0;
    		}
    	}
    	saida[linha][coluna] = matriz[linha][coluna];
    	coluna++;
    }
    return saida;
}

int** matriz_cidades(char* filename, int num_cidades)
{
	FILE *fp;
	int** saida = malloc(sizeof(int*)*num_cidades);
	int i = 0, count = 0, linha = 0, coluna = 0;
	for(i = 0; i < num_cidades; i++)
	{
		saida[i] = malloc(sizeof(int)*num_cidades);
	}

    char str[MAXCHAR];
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Could not open file %s",filename);
        exit(1);
    }
    while (fgets(str, MAXCHAR, fp) != NULL)
    {
    	if(strstr(str, "EDGE_WEIGHT_SECTION"))
    	{
    		break;
    	}
    }
    i = 0;
    fscanf (fp, "%d", &i);    
	while (count != (num_cidades * num_cidades))
    {  	
    	count++;
    	if(coluna > (num_cidades-1))
    	{
    		linha++;
    		coluna = 0;
    		if(linha > (num_cidades-1))
    		{
    			linha = 0;
    		}
    	}
    	saida[linha][coluna] = i;
    	coluna++;
    	fscanf (fp, "%d", &i);
    }
    fclose (fp);

    return saida;
}

int* copia_lista(int* lista_original, int size)
{
	int* saida = malloc(sizeof(int)*size);
	for(int i = 0; i < size; i++)
	{
		saida[i] = lista_original[i];
	}

	return saida;
}

int pertence(int elem, int* solution, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(solution[i]==elem)
		{
			return 1;
		}
	}
	return 0;
}

int pega_menor(int* lista, int size, int count, int* solution)
{
	int i, menor = MAX, menor_i = -1;
	for(int i = 0; i < size; i++)
	{
		if((lista[i] < menor) && (lista[i] != 0) && (lista[i] != MAX) && (!pertence(i, solution, size-1)))
		{
			menor = lista[i];
			menor_i = i;
		}
	}
	return menor_i;
}

int pega_aleatorio(int* lista, int size, int count, int* solution, int i_grasp, double alpha)
{
	// copia é a lista de pesos de entrada ordenada
	// n é lista de indices

	int i, j, elem = -1, tam_array = 0;
	int aux[size];

	for(i = 0; i < size; i++)
	{
		if((lista[i] != 0) && (lista[i] != MAX) && (!pertence(i, solution, size-1)))
		{
			aux[tam_array] = i;
			tam_array += 1;
		}
	}

	if(tam_array > 0)
	{

		int cmin, cmax, menor, j;
		double parameter_with_alpha;

		cmin = menor_elem(lista, size, solution);
		cmax = maior_elem(lista, size, solution);

		parameter_with_alpha = (double)cmin + alpha * ((double)cmax - (double)cmin);


		int pesos[tam_array];

		for(i = 0; i < tam_array; i++)
		{
			pesos[i] = lista[aux[i]];
		}

		int temp_peso, temp_i;
		for (int step = 0; step < tam_array - 1; step++)
		{
		    int min_idx = step;
		    for (i = step + 1; i < tam_array; i++)
		    {
		      	if (pesos[i] < pesos[min_idx])
		      	{
		      		min_idx = i;	
		      	} 
		    }
		    if(min_idx != step)
		    {
		    	temp_i = aux[min_idx];
		    	temp_peso = pesos[min_idx];
		    	aux[min_idx] = aux[step];
		    	pesos[min_idx] = pesos[step];
		    	aux[step] = temp_i;
		    	pesos[step] = temp_peso;
		    }
		}

		srand(i_grasp);
		int random_number;

		while(elem == -1)
		{
			random_number = rand() % tam_array;
			if((double)lista[aux[random_number]] <= parameter_with_alpha){
				elem = aux[random_number];
			}
		}
		
		return elem;
	}

	return elem;
}

int elem_repetido(int* lista, int size)
{
	int i, j, count = 1;
	for(i = 0; i < size; i++)
	{
		for(j = i+1; j < size;j++)
		{
			if((lista[i] == lista[j]) && (lista[i] != -1) && (lista[i] != 0))
			{
				count++;
				if(count == 2)
				{
					return 1;
				}
			}
			count = 1;
		}
	}
	return 0;
}

int e_valida(int** matriz, int* solution, int size, int count)
{
	if((matriz[solution[size-1]][solution[0]] == 0) || (matriz[solution[size-1]][solution[0]] == MAX))
	{
		return 0;
	} else
	{
		return 1;
	}
}

int menor_elem(int* lista, int size, int* solution)
{
	int i, menor = MAX;
	for(i = 0; i < size; i++){
		if((menor > lista[i]) && (lista[i] != 0) && (lista[i] != MAX) && (!pertence(i, solution, size-1)))
		{
			menor = lista[i];
		}
	}

	return menor;
}

int maior_elem(int* lista, int size, int* solution)
{
	int i, maior = -1;
	for(i = 0; i < size; i++){
		if((maior < lista[i]) && (lista[i] != 0) && (lista[i] != MAX) && (!pertence(i, solution, size-1)))
		{
			maior = lista[i];
		}
	}

	return maior;
}

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

int diferenca_simetrica(int* a, int* b, int size)
{
	int dif = 0;
	for(int i = 0; i < size; i++)
	{
		if(a[i] != b[i])
		{
			dif += 1;
		}
	}

	return dif;
}

int menor_diferenca(int* solution, int size, int** elite_set, int n_atual)
{
	int menor_dif = diferenca_simetrica(solution, elite_set[0], size), temp;
	for(int i = 1; i < n_atual; i++)
	{
		temp = diferenca_simetrica(solution, elite_set[i], size);
		if(temp < menor_dif)
		{
			menor_dif = temp;
		}
	}

	return menor_dif;
}

int pega_pesada(int size, int**matriz, int** elite_set, int nE)
{
	int maior_peso, temp;
	maior_peso = peso_total(elite_set[0], size+1, matriz);

	for(int i = 1; i < nE; i++)
	{
		temp = peso_total(elite_set[i], size+1, matriz);
		if(temp > maior_peso)
		{
			maior_peso = temp;
		}
	}

	return maior_peso;
}

int pega_menor_pesada(int* solution, int size, int**matriz, int** elite_set, int nE)
{
	int indice = size+1, dif, peso, dif_aux, peso_aux, peso_temp;
	dif = diferenca_simetrica(solution, elite_set[0], size);
	peso = peso_total(solution, size+1, matriz);
	peso_temp = peso_total(elite_set[0], size+1, matriz);

	if(peso <= peso_temp)
	{
		indice = 0;
	}

	for(int i = 1; i < nE; i++)
	{
		dif_aux = diferenca_simetrica(solution, elite_set[i], size);
		peso_aux = peso_total(elite_set[i], size+1, matriz);

		if((dif_aux < dif) && (peso <= peso_aux))
		{
			indice = i;
			dif = dif_aux;
		}
		else if((indice == size+1) && (peso <= peso_aux)){
			indice = i;
			dif = dif_aux;
		}

	}

	return indice;
}

int* guloso_grasp(int** matriz_entrada, int size, int start, double alpha, int i_grasp)
{
	int** matriz = copia_matriz(matriz_entrada, size);
	int i, count = 1, count_2 = 0, menor, size_s = size-1;
	int random_number;
	int* solution = malloc(sizeof(int)*(size_s)), *lista;
	for(i = 1; i < size_s; i++)
	{
		solution[i] = -1;
	}
	solution[0] = start;
	printf("Solucao: \n");
	for(i = 0; i < size_s-1; i++)
	{
		printf("%d -> ", solution[i]);
	}
	printf("%d\n", solution[size_s-1]);
	while(pertence(-1, solution, size_s))
	{
		menor = pega_aleatorio(matriz[solution[count-1]], size, count, solution, i_grasp, alpha);
		printf("MENOR: %d\n", menor);
		if(menor == -1)
		{
			matriz[solution[count-1]][solution[count]] = MAX;
			count--;
		} else {
			count_2++;
			if(count_2 > size+2)
			{
				free(solution);
				solution = NULL;
				break;
			}
			solution[count] = menor;
			count++;
		}
		printf("Solucao: \n");
		for(i = 0; i < size_s-1; i++)
		{
			printf("%d -> ", solution[i]);
		}
		printf("%d\n", solution[size_s-1]);
		if(solution[size_s-1] != -1)
		{
			if(!e_valida(matriz, solution, size_s, count))
			{
				matriz[solution[count-1]][solution[count]] = 0;
				solution[size_s-1] = -1;
				count--;
			} else {
				break;
			}
		}
	}
	free(matriz);
	return solution;
}

int peso_total(int* solution, int size, int**matriz)
{
	int peso = 0, i;
	if(!solution)
	{
		return 999999999;
	}
	for(i = 1; i < size; i++)
	{
		peso += matriz[solution[i-1]][solution[i]];
	}
	peso += matriz[solution[size-1]][solution[0]];
	return peso;
}

int* opt2_swap(int* solution, int i, int k, int size)
{
	int* swap = malloc(sizeof(int)*size);
	int j, count = 0;
	for(j = i; j < k+1; j++)
	{
		swap[j] = solution[k - count];
		count++;
	}
	for(j = 0; j < i; j++)
	{
		swap[j] = solution[j];
	}
	for(j = k+1; j < size-1; j++)
	{
		swap[j] = solution[j];
	}
	return swap;
}

int* busca_local(int** matriz, int* solution, int size)
{
	int i, k, peso_atual, novo_peso;
	int* nova_solution;
	start_again:
	peso_atual = peso_total(solution, size-1, matriz);
   	for (i = 1; i < size - 3; i++) {
       for (k = i + 1; k < size - 2; k++) {
           	nova_solution = opt2_swap(solution, i, k, size);
           	novo_peso = peso_total(nova_solution, size-1, matriz);
           	if (novo_peso < peso_atual) {
           		free(solution);
           		solution = nova_solution;
               	peso_atual = novo_peso;
               	goto start_again;
           }else{
           		free(nova_solution);
           }
       }
   	}
	return solution;
}

int* grasp(int** matriz_entrada, int size, int num_restart, int target, double alpha)
{
	int i;
	int* solution = NULL; 
	int* aux_solution = NULL;
	int peso_aux = 0;
	for(i = 1; i <= num_restart; i++)
	{
		srand(i);
		aux_solution = guloso_grasp(matriz_entrada, size, rand() % size, alpha, i);
		if(aux_solution == NULL){
			continue;
		}
		aux_solution = busca_local(matriz_entrada, aux_solution, size);
		peso_aux = peso_total(aux_solution, size-1, matriz_entrada);
		
		if(peso_aux <= target)
		{
			return aux_solution;
		}
		
		if(peso_aux < peso_total(solution, size-1, matriz_entrada))
		{
			free(solution);
			solution = aux_solution;
		}
		else
		{
			free(aux_solution);
		}
	}
	return solution;
}

/*
	passar size como size - 1
	atualizar o n_atual + 1, sempre que essa função for chamada, caso n_atual < nE
	parameter_E é o parametro que define se a diferença simetrica é boa ou não
	lembrar de limpar elite_set quando a função terminar
*/
int** update_elite_set(int* solution, int size, int** matriz, int** elite_set, int parameter_E, int nE, int n_atual)
{
	int menor_dif, maior_peso;
	if (n_atual < nE)
	{
		if(n_atual == 0)
		{
			elite_set[0] = copia_lista(solution, size);
		}
		else{
			
			menor_dif = menor_diferenca(solution, size, elite_set, n_atual);

			if(menor_dif > parameter_E)
			{
				elite_set[n_atual] = copia_lista(solution, size);
			}
		}
	}
	else{
		menor_dif = menor_diferenca(solution, size, elite_set, n_atual);
		maior_peso = pega_pesada(size, matriz, elite_set, nE);
		int peso_solution = peso_total(solution, size+1, matriz), indice;
		
		if((peso_solution < maior_peso) && (menor_dif > parameter_E))
		{
			indice = pega_menor_pesada(solution, size, matriz, elite_set, nE);
			if(indice == size + 1)
			{
				exit(1);
			}
			else{
				free(elite_set[indice]);
				elite_set[indice] = copia_lista(solution, size);
			}
		}
	}

	return elite_set;
}

int main(int* argc, char* argv[])
{	
	int num_cidades = 443;
	//int **matriz = matriz_cidades("br17.atsp.txt", num_cidades);
	int **matriz = matriz_cidades("rbg443.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv170.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ft53.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ft70.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv33.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv35.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv38.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv44.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv47.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv55.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv64.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv70.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("kro124p.atsp.txt", num_cidades); //NAO
	//int **matriz = matriz_cidades("p43.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("rbg323.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("rbg358.atsp.txt", num_cidades); //NAO
	//int **matriz = matriz_cidades("rbg403.atsp.txt", num_cidades); //NAO
	//int **matriz = matriz_cidades("ry48p.atsp.txt", num_cidades);

	int i, j;

	
	clock_t begin = clock();

	// MATRIZ DE ENTRADA || NUMERO DE CIDADES || QUANTIDADE DE VEZES QUE PROCURA SOLUCAO MELHOR || TARGET || ALPHA (0.0 Greedy | 1.0 Random)
	int* solution = grasp(matriz, num_cidades, 3, 6010, 0.0);

	time_t end = clock();

	int peso = peso_total(solution, num_cidades-1, matriz);


	for(i = 0; i < num_cidades; i++)
	{
		for(j = 0; j < num_cidades; j++)
		{
			//printf("%d\t", matriz[i][j]);
		}
		//printf("\n");
		free(matriz[i]);
	}

	printf("Solucao: \n");
	for(i = 0; i < num_cidades-2; i++)
	{
		printf("%d -> ", solution[i]);
	}
	printf("%d\n", solution[num_cidades-2]);

	printf("Peso total: %d\n", peso);

	free(matriz);
	free(solution);
	double time_spent = 0.0;
	time_spent += (double)(end - begin)/CLOCKS_PER_SEC;
	printf("TEMPO DE EXECUÇÃO: %f\n", time_spent);
    return 0;
}

