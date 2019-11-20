#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXCHAR 1000
//#define MAX 9999 //br17 p43
//#define MAX 40 //rbg
#define MAX 9999999 // ft, kro, ry
//#define MAX 100000000 // ftv

int** copia_matriz(int** mat_original, int num_cidades);
int** cria_copia_matriz(int** matriz, int size, int tam);
int** matriz_cidades(char* filename, int num_cidades);
int* copia_lista(int* lista_original, int size);
void limpa_matriz(int** matriz, int size);

//Auxiliares
int pertence(int elem, int* solution, int size);
int pega_menor(int* lista, int size, int count, int* solution);
int pega_aleatorio(int* lista, int size, int count, int* solution, int i_grasp, double alpha);
int elem_repetido(int* lista, int size);
int e_valida(int** matriz, int* solution, int size, int count);
int caminho_existe(int* solution, int size, int**matriz);
int menor_elem(int* lista, int size, int* solution);
int maior_elem(int* lista, int size, int* solution);
int compare( const void* a, const void* b);
int diferenca_simetrica(int* a, int* b, int size);
int menor_diferenca(int* solution, int size, int** elite_set, int n_atual);
int pega_pesada(int size, int**matriz, int** elite_set, int nE);
int* pega_leve(int size, int**matriz, int** elite_set, int nE);
int pega_menor_pesada(int* solution, int size, int**matriz, int** elite_set, int nE);

int* guloso(int** matriz_entrada, int size, int start);
int peso_total(int* solution, int size, int**matriz);
int** gerar_populacao(int** matriz_entrada, int size, int tam_pop);
int* cruzamento(int* pai_elite, int* pai_normal, int size_s, int mutacao, int iter);

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

int** cria_copia_matriz(int** matriz, int size, int tam)
{
	int** saida = malloc(sizeof(int*)*tam);
	int count = 0, linha = 0, coluna = 0;
	for(int i = 0; i < tam-1; i++)
	{
		saida[i] = matriz[i];
	}

	free(matriz);
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

void limpa_matriz(int** matriz, int size)
{
	for(int i = 0; i < size; i++)
	{
		free(matriz[i]);
	}
	free(matriz);
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

int caminho_existe(int* solution, int size, int**matriz)
{
	int valor;
	if(!solution)
	{
		return 0;
	}
	for(int i = 1; i < size; i++)
	{
		valor = matriz[solution[i-1]][solution[i]]; 
		if((valor == MAX) || (valor == 0))
		{
			return 0;
		}
	}
	valor = matriz[solution[size-1]][solution[0]];
	if((valor == MAX) || (valor == 0))
	{
		return 0;
	}
	return 1;
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

int* pega_leve(int size, int**matriz, int** elite_set, int nE)
{
	int menor_peso, temp, *saida = copia_lista(elite_set[0], size);
	menor_peso = peso_total(elite_set[0], size, matriz);

	for(int i = 1; i < nE; i++)
	{
		temp = peso_total(elite_set[i], size, matriz);
		if(temp < menor_peso)
		{
			free(saida);
			menor_peso = temp;
			saida = copia_lista(elite_set[i], size);
		}
	}

	return saida;
}

int* guloso(int** matriz_entrada, int size, int start)
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
	/*printf("Solucao: \n");
	for(i = 0; i < size_s-1; i++)
	{
		printf("%d -> ", solution[i]);
	}
	printf("%d\n", solution[size_s-1]);
	*/while(pertence(-1, solution, size_s))
	{
		menor = pega_menor(matriz[solution[count-1]], size, count, solution);
		//printf("MENOR: %d\n", menor);
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
		/*printf("Solucao: \n");
		for(i = 0; i < size_s-1; i++)
		{
			printf("%d -> ", solution[i]);
		}
		printf("%d\n", solution[size_s-1]);
		*/if(solution[size_s-1] != -1)
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

int** gerar_populacao(int** matriz_entrada, int size, int tam_pop)
{
	int **populacao = malloc(sizeof(int*)*tam_pop), individuos = 0;
	while(individuos < tam_pop)
	{
		srand(individuos);
		populacao[individuos] = guloso(matriz_entrada, size, rand() % size);
		individuos++;
	}
	return populacao;
}

int* cruzamento(int* pai_elite, int* pai_normal, int size_s, int mutacao, int iter)
{
	int *filho = malloc(sizeof(int)*size_s), random, cidade;
	srand(iter);
	for(int k = 0; k < size_s; k++)
	{
		filho[k] = -1;
	}
	for(int i = 0; i < size_s; i++)
	{
		random = rand() % 100;
		if(filho[i] != -1)
		{
			continue;
		}
		if(random <= mutacao)
		{
			cidade = rand() % (size_s+1);
			filho[i] = cidade;
		}
		else if(random <= 70)
		{
			if(pertence(pai_elite[i], filho, size_s))
			{
				if(!pertence(pai_normal[i], filho, size_s))
				{
					filho[i] = pai_normal[i];
				}
				continue;
			}
			filho[i] = pai_elite[i];
			if(pai_elite[i] != pai_normal[i])
			{
				for(j = i; j < size_s; j++)
				{
					if(pai_elite[j] == pai_normal[i])
					{
						filho[j] = pai_normal[i];
					}
				}
			}
		}
		else
		{
			if(pertence(pai_normal[i], filho, size_s))
			{
				if(!pertence(pai_elite[i], filho, size_s))
				{
					filho[i] = pai_elite[i];
				}
				continue;
			}
			filho[i] = pai_normal[i];
			if(pai_elite[i] != pai_normal[i])
			{
				for(j = i; j < size_s; j++)
				{
					if(pai_normal[j] == pai_elite[i])
					{
						filho[j] = pai_elite[i];
					}
				}
			}
		}
	}

	return filho;
}

int* swap(int* solution, int i, int k, int size)
{
	int* swap = copia_lista(solution, size);
	
	for(int j = 0; j < size; j++)
	{
		if(swap[j] == i)
		{
			swap[j] = k;
		}
		else if(swap[j] == k)
		{
			swap[j] = i;
		}
	}

	return swap;
}

int main(int* argc, char* argv[])
{	
	int num_cidades = 70;
	//int **matriz = matriz_cidades("br17.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("rbg443.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv170.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ft53.atsp.txt", num_cidades);
	int **matriz = matriz_cidades("ft70.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv33.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv35.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv38.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv44.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv47.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv55.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv64.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ftv70.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("p43.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("rbg323.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("rbg358.atsp.txt", num_cidades); //NAO
	//int **matriz = matriz_cidades("rbg403.atsp.txt", num_cidades); //NAO
	//int **matriz = matriz_cidades("rbg443.atsp.txt", num_cidades);
	//int **matriz = matriz_cidades("ry48p.atsp.txt", num_cidades);

	int i, j;

	
	clock_t begin = clock();


	
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

