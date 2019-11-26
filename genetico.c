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
int** copia_matriz2(int** matriz, int linhas, int colunas);
int** cria_copia_matriz(int** matriz, int size, int tam);
int** matriz_cidades(char* filename, int num_cidades);
int* copia_lista(int* lista_original, int size);
int** ordena_lista(int** matriz_entrada, int** lista_original, int size, int size_s, int size_lista);
void limpa_matriz(int** matriz, int size);
long long current_timestamp(); 

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
int diferente(int* a, int* b, int size_s);
int menor_diferenca(int* solution, int size, int** elite_set, int n_atual);
int pega_pesada(int size, int**matriz, int** elite_set, int nE);
int* pega_leve(int size, int**matriz, int** elite_set, int nE);
int pega_menor_pesada(int* solution, int size, int**matriz, int** elite_set, int nE);
int pesa_arestas(int **matriz, int size);

int* guloso(int** matriz_entrada, int size, int start, int iter);
int peso_total(int* solution, int size_s, int**matriz);
int** gerar_populacao(int** matriz_entrada, int size, int tam_pop);
int fitness(int peso_s, int peso_arestas);
int** seleciona_pais(int** matriz_entrada, int size, int** populacao, int tam_pop, int size_s, int iter);
int inserir_filho(int** matriz_entrada, int size, int** populacao, int tam_pop, int* filho);
int* cruzamento(int* pai_elite, int* pai_normal, int size_s, int iter);
int** cruzamento_s(int* pai_elite, int* pai_normal, int size_s, int iter);
int* mutacao(int *filho, int size_s, int num_cidades, int iter);

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

int** copia_matriz2(int** matriz, int linhas, int colunas)
{
	int** saida = malloc(sizeof(int*)*linhas);
	int i = 0, count = 0, linha = 0, coluna = 0;
	for(int linha = 0; linha < linhas; linha++)
	{
		saida[i] = copia_lista(matriz[i], colunas);
	}
	for(i = 0; i < linhas; i++)
	{
		for(int j = 0; j < colunas; j++)
		{
			printf("%d\t", saida[i][j]);
		}
		printf("\n");
		//free(matriz[i]);
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

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
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

int** ordena_lista(int** matriz_entrada, int** lista_original, int size, int size_s, int size_lista)
{
	int i, j, *aux;
	for (i = 0; i < size_lista; i++) { 
        for (j = i + 1; j < size_lista; j++) { 
            if (peso_total(lista_original[j], size_s, matriz_entrada) < peso_total(lista_original[i], size_s, matriz_entrada)) { 
                aux = lista_original[i]; 
                lista_original[i] = lista_original[j]; 
                lista_original[j] = aux; 
            } 
        } 
    }

    return lista_original; 
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

		srand(current_timestamp());
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

int diferente(int* a, int* b, int size_s)
{
	for(int i = 0; i < size_s; i++)
	{
		if(a[i] != b[i])
		{
			return 1;
		}
	}
	return 0;
}

int* pega_leve(int size, int**matriz, int** elite_set, int nE)
{
	int menor_peso, temp, *saida = copia_lista(elite_set[0], size), indice = 0;

	menor_peso = peso_total(elite_set[0], size, matriz);

	for(int i = 1; i < nE; i++)
	{
		temp = peso_total(elite_set[i], size, matriz);
		if(temp < menor_peso)
		{
			free(saida);
			menor_peso = temp;
			saida = copia_lista(elite_set[i], size);

			indice = i;
		}
	}
	free(saida);
	return elite_set[indice];
}

int pesa_arestas(int **matriz, int size)
{
	int total = 0;
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(matriz[i][j] != MAX)
			{
				total += matriz[i][j];
			}
		}
	}

	return total;
}

int* guloso(int** matriz_entrada, int size, int start, int iter)
{
	double alpha = 0.2;
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
		menor = pega_aleatorio(matriz[solution[count-1]], size, count, solution, iter, alpha);
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

int peso_total(int* solution, int size_s, int**matriz)
{
	int peso = 0, i;
	if(!solution)
	{
		return 999999999;
	}
	for(i = 1; i < size_s; i++)
	{
		peso += matriz[solution[i-1]][solution[i]];
	}
	peso += matriz[solution[size_s-1]][solution[0]];
	return peso;
}

int** gerar_populacao(int** matriz_entrada, int size, int tam_pop)
{
	int **populacao = malloc(sizeof(int*)*tam_pop), individuos = 0, *solution;
	for(int i = 0; i < tam_pop; i++)
	{
		solution = guloso(matriz_entrada, size, rand() % size, i);
		if(solution == NULL)
		{
			i -= 1;
			continue;
		}
		populacao[i] = solution;
	}
	return populacao;
}

int fitness(int peso_s, int peso_arestas)
{
	int fit = (peso_arestas)/(1 - peso_s);
	if(fit < 0)
	{
		fit = fit * (-1);
	}
	return fit;
}

int** seleciona_pais(int** matriz_entrada, int size, int** populacao, int tam_pop, int size_s, int iter)
{
	int **pais = malloc(sizeof(int*)*2);
	pais[1] = NULL;
	srand(current_timestamp());
	int tamanho_torneio = tam_pop / 3, **torneio, random;
	torneio = malloc(sizeof(int*)*tamanho_torneio);
	for(int i = 0; i < tamanho_torneio; i++)
	{
		random = rand() % tam_pop;
		torneio[i] = populacao[random];
	}

	pais[0] = torneio[0];
	for(int i = 1; i < tamanho_torneio; i++)
	{
		if(peso_total(torneio[i], size_s, matriz_entrada) < peso_total(pais[0], size_s, matriz_entrada))
		{
			pais[0] = torneio[i];
		}
	}
	free(torneio);

	torneio = malloc(sizeof(int*)*tamanho_torneio);
	for(int i = 0; i < tamanho_torneio; i++)
	{
		random = rand() % tam_pop;
		torneio[i] = populacao[random];
	}
	
	for(int i = 1; i < tamanho_torneio; i++)
	{
		if(&torneio[i] != &pais[0])
		{
			if(peso_total(torneio[i], size_s, matriz_entrada) < peso_total(pais[1], size_s, matriz_entrada))
			{
				pais[1] = torneio[i];
			}	
		}
		
	}
	free(torneio);
	
	return pais;
}

int inserir_filho(int** matriz_entrada, int size_s, int** populacao, int tam_pop, int* filho)
{
	int pior_indice = 0, pior_peso = peso_total(populacao[0], size_s, matriz_entrada), peso_aux;
	for(int k = 0; k < tam_pop; k++)
	{
		peso_aux = peso_total(populacao[k], size_s, matriz_entrada);
		if(peso_aux >= pior_peso)
		{
			pior_peso = peso_aux;
			pior_indice = k;
		}
	}
	if(peso_total(filho, size_s, matriz_entrada) < pior_peso)
	{
		free(populacao[pior_indice]);
		populacao[pior_indice] = filho;
		return 1;
	}
	free(filho);
	return 0;
}

int* cruzamento(int* pai_elite, int* pai_normal, int size_s, int iter)
{
	int *filho = malloc(sizeof(int)*size_s), random, cidade;
	srand(current_timestamp());
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
		if(random <= 70)
		{
			filho[i] = pai_elite[i];
			if(pai_elite[i] != pai_normal[i])
			{
				for(int j = i; j < size_s; j++)
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
			filho[i] = pai_normal[i];
			if(pai_elite[i] != pai_normal[i])
			{
				for(int j = i; j < size_s; j++)
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

int** cruzamento_s(int* pai_elite, int* pai_normal, int size_s, int iter)
{
	int *filho_1, *filho_2, random, cidade, sub_trecho_size, indice, i, k, elem1, elem2, trocados1 = 0, trocados2 = 0;
	srand(current_timestamp());
	filho_1 = copia_lista(pai_elite, size_s);
	filho_2 = copia_lista(pai_normal, size_s);
	sub_trecho_size = size_s / 4;
	indice = rand() % (size_s - sub_trecho_size);
	int aux;
	for(i = indice; i < sub_trecho_size; i++)
	{
		aux = filho_1[i];
		filho_1[i] = filho_2[i];
		filho_2[i] = aux;
	}
	for(k = 0; k < indice; k++)
	{
		elem1 = filho_1[k];
		elem2 = filho_2[k];
		for(i = indice; i < sub_trecho_size; i++)
		{
			if(elem1 == filho_1[i])
			{
				filho_1[k] = filho_2[trocados1+indice];
				trocados1 += 1;
			}
			if(elem2 == filho_2[i])
			{
				filho_2[k] = filho_1[trocados2+indice];
				trocados2 += 1;
			}
		}	
	}
	for(k = indice+sub_trecho_size; k < size_s; k++)
	{
		elem1 = filho_1[k];
		elem2 = filho_2[k];
		for(i = indice; i < sub_trecho_size; i++)
		{
			if(elem1 == filho_1[i])
			{
				filho_1[k] = filho_2[trocados1+indice];
				trocados1 += 1;
			}
			if(elem2 == filho_2[i])
			{
				filho_2[k] = filho_1[trocados2+indice];
				trocados2 += 1;
			}
		}	
	}
	int **filhos = malloc(sizeof(int*)*2);
	random = rand() % 100;
	if(random <= 9){
		filho_1 = mutacao(filho_1, size_s, size_s + 1, iter);
	}
	random = rand() % 100;
	if(random <= 9){
		filho_2 = mutacao(filho_2, size_s, size_s + 1, iter);
	}
	filhos[0] = filho_1;
	filhos[1] = filho_2;

	return filhos;
}

int* mutacao(int *filho, int size_s, int num_cidades, int iter)
{
	srand(current_timestamp());
	int indice, nova_cidade, aux;
	indice = rand() % size_s;
	nova_cidade = rand() % num_cidades;
	aux = filho[indice];
	filho[indice] = nova_cidade;
	for(int j = 0; j < size_s; j++)
	{
		if(j == indice)
		{
			continue;
		}
		if(filho[j] == nova_cidade)
		{
			filho[j] = aux;
			break;
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

int* genetico(int **matriz_entrada, int size, int tam_pop, int target, int limite, int cruzamentos)
{
	int size_s = size - 1, *saida;
	int **populacao = gerar_populacao(matriz_entrada, size, tam_pop), procura = 0, iter = 0,
	 **pais, **filhos, k, **filhos_totais, size_totais, inseriu;
	while(procura < limite)
	{
		srand(current_timestamp());
		filhos_totais = malloc(sizeof(int*)*cruzamentos*2);
		size_totais = 0;
		inseriu = 0;
		for(int i = 0; i < cruzamentos; i++)
		{
			pais = seleciona_pais(matriz_entrada, size, populacao, tam_pop, size_s, (iter*iter) + i);
			filhos = cruzamento_s(pais[0], pais[1], size_s, iter);
			filhos_totais[size_totais] = filhos[0];
			size_totais += 1;
			filhos_totais[size_totais] = filhos[1];
			size_totais += 1;
			free(filhos);
			free(pais);
		}

		for(int i = 0; i < size_totais; i++)
		{
			if(peso_total(filhos_totais[i], size_s, matriz_entrada) <= target)
			{
				saida = copia_lista(filhos_totais[i], size_s);
				for(k = 0; k < tam_pop; k++)
				{
					free(populacao[k]);
				}
				free(populacao);
				for(k = 0; k < size_totais; k++)
				{
					free(filhos_totais[i]);
				}
				free(filhos_totais);
				return saida;
			}
			else if(inserir_filho(matriz_entrada, size_s, populacao, tam_pop, filhos_totais[i]))
			{
				inseriu = 1;
			}
		}

		if(inseriu == 0)
		{
			procura += 1;
		}

		iter++;
		free(filhos_totais);
	}

	populacao = ordena_lista(matriz_entrada, populacao, size, size_s, tam_pop);

	saida = copia_lista(populacao[0], size_s);
	for(k = 0; k < tam_pop; k++)
	{
		free(populacao[k]);
	}
	free(populacao);
	return saida;
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

	//(int **matriz_entrada, int size, int tam_pop, int target, int limite, int cruzamentos)
	int *solution = genetico(matriz, num_cidades, 400, 39, 50, 150);

	time_t end = clock();

	int peso = peso_total(solution, num_cidades-1, matriz);


	for(i = 0; i < num_cidades; i++)
	{
		for(int j = 0; j < num_cidades; j++)
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

