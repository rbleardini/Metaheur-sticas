#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXCHAR 1000

int** copia_matriz(int** mat_original, int num_cidades);
int** cria_copia_matriz(int** matriz, int size, int tam);
int** matriz_cidades(char* filename, int num_cidades);
int* copia_lista(int* lista_original, int size);
int** ordena_lista(int** matriz_entrada, int** lista_original, int size, int size_s, int size_lista, int val_a);
void limpa_matriz(int** matriz, int size);
long long current_timestamp(); 

//Auxiliares
int pertence(int elem, int* solution, int size);
int pega_aleatorio(int* lista, int size, int count, int* solution, int i_grasp, double alpha, int MAX);
int e_valida(int** matriz, int* solution, int size, int count, int MAX);
int menor_elem(int* lista, int size, int* solution, int MAX);
int maior_elem(int* lista, int size, int* solution, int MAX);
int pesa_arestas(int **matriz, int size, int MAX);

int* guloso(int** matriz_entrada, int size, int start, int iter, int MAX);
int peso_total(int* solution, int size_s, int**matriz);
int** gerar_populacao(int** matriz_entrada, int size, int tam_pop, int MAX);
int fitness(int peso_s, int peso_arestas);
int** seleciona_pais(int** matriz_entrada, int size, int** populacao, int tam_pop, int size_s, int iter, int val_a);
int inserir_filho(int** matriz_entrada, int size, int** populacao, int tam_pop, int* filho, int val_a);
int** cruzamento(int* pai_elite, int* pai_normal, int size_s, int iter);
int* mutacao(int *filho, int size_s, int num_cidades, int iter);
int* genetico(int **matriz_entrada, int size, int tam_pop, int target, int limite, int cruzamentos, int MAX);

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
    //gettimeofday(&te, NULL); // get current time
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

int** ordena_lista(int** matriz_entrada, int** lista_original, int size, int size_s, int size_lista, int val_a)
{
	int i, j, *aux;
	for (i = 0; i < size_lista; i++) { 
        for (j = i + 1; j < size_lista; j++) { 
            if (fitness(peso_total(lista_original[j], size_s, matriz_entrada), val_a) > fitness(peso_total(lista_original[i], size_s, matriz_entrada), val_a)) { 
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

int pega_aleatorio(int* lista, int size, int count, int* solution, int i_grasp, double alpha, int MAX)
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

		cmin = menor_elem(lista, size, solution, MAX);
		cmax = maior_elem(lista, size, solution, MAX);

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

int e_valida(int** matriz, int* solution, int size, int count, int MAX)
{
	if((matriz[solution[size-1]][solution[0]] == 0) || (matriz[solution[size-1]][solution[0]] == MAX))
	{
		return 0;
	} else
	{
		return 1;
	}
}

int menor_elem(int* lista, int size, int* solution, int MAX)
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

int maior_elem(int* lista, int size, int* solution, int MAX)
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

int pesa_arestas(int **matriz, int size, int MAX)
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

int* guloso(int** matriz_entrada, int size, int start, int iter, int MAX)
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
		menor = pega_aleatorio(matriz[solution[count-1]], size, count, solution, iter, alpha, MAX);
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
			if(!e_valida(matriz, solution, size_s, count, MAX))
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

int** gerar_populacao(int** matriz_entrada, int size, int tam_pop, int MAX)
{
	int **populacao = malloc(sizeof(int*)*tam_pop), individuos = 0, *solution;
	for(int i = 0; i < tam_pop; i++)
	{
		solution = guloso(matriz_entrada, size, rand() % size, i, MAX);
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

int** seleciona_pais(int** matriz_entrada, int size, int** populacao, int tam_pop, int size_s, int iter, int val_a)
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
		if(fitness(peso_total(torneio[i], size_s, matriz_entrada), val_a) > fitness(peso_total(pais[0], size_s, matriz_entrada), val_a))
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
			if(fitness(peso_total(torneio[i], size_s, matriz_entrada), val_a) > fitness(peso_total(pais[1], size_s, matriz_entrada), val_a))
			{
				pais[1] = torneio[i];
			}	
		}
		
	}
	free(torneio);
	
	return pais;
}

int inserir_filho(int** matriz_entrada, int size_s, int** populacao, int tam_pop, int* filho, int val_a)
{
	int pior_indice = 0, pior_membro = fitness(peso_total(populacao[0], size_s, matriz_entrada), val_a), membro_aux;
	for(int k = 0; k < tam_pop; k++)
	{
		membro_aux = fitness(peso_total(populacao[k], size_s, matriz_entrada), val_a);
		if(membro_aux <= pior_membro)
		{
			pior_membro = membro_aux;
			pior_indice = k;
		}
	}
	if(fitness(peso_total(filho, size_s, matriz_entrada), val_a) > pior_membro)
	{
		free(populacao[pior_indice]);
		populacao[pior_indice] = filho;
		return 1;
	}
	free(filho);
	return 0;
}

int** cruzamento(int* pai_elite, int* pai_normal, int size_s, int iter)
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

int* genetico(int **matriz_entrada, int size, int tam_pop, int target, int limite, int cruzamentos, int MAX)
{
	int size_s = size - 1, *saida;
	int **populacao = gerar_populacao(matriz_entrada, size, tam_pop, MAX), procura = 0, iter = 0,
	 **pais, **filhos, k, **filhos_totais, size_totais, inseriu;
	int val_a = pesa_arestas(matriz_entrada, size, MAX);
	while(procura < limite)
	{
		srand(current_timestamp());
		filhos_totais = malloc(sizeof(int*)*cruzamentos*2);
		size_totais = 0;
		inseriu = 0;
		for(int i = 0; i < cruzamentos; i++)
		{
			pais = seleciona_pais(matriz_entrada, size, populacao, tam_pop, size_s, (iter*iter) + i, val_a);
			filhos = cruzamento(pais[0], pais[1], size_s, iter);
			filhos_totais[size_totais] = filhos[0];
			size_totais += 1;
			filhos_totais[size_totais] = filhos[1];
			size_totais += 1;
			free(filhos);
			free(pais);
		}

		for(int i = 0; i < size_totais; i++)
		{
			if(fitness(peso_total(filhos_totais[i], size_s, matriz_entrada), val_a) == target)
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
			else if(inserir_filho(matriz_entrada, size_s, populacao, tam_pop, filhos_totais[i], val_a))
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

	populacao = ordena_lista(matriz_entrada, populacao, size, size_s, tam_pop, val_a);

	saida = copia_lista(populacao[0], size_s);
	for(k = 0; k < tam_pop; k++)
	{
		free(populacao[k]);
	}
	free(populacao);
	return saida;
}

void roda_tudo(char *str, int tamp, int crz, int lmt, char *resp){
	int MAX;
	int num_cidades;
	int **matriz;
	int opt;
	int i, j;
	int peso;
	clock_t begin;
	int *solution;
	time_t end;
	double time_spent;
	system("@cls||clear");
	int erou = 1;

	if(!strcmp("br17", str))
	{
		MAX = 9999;
		printf("Resultado de br17.atsp.txt: \n");
		num_cidades = 17;
		matriz = matriz_cidades("br17.atsp.txt", num_cidades);
		opt = 39;
	} else if(!strcmp("ft53", str))
	{
		MAX = 9999999;
		printf("Resultado de ft53.atsp.txt: \n");
		num_cidades = 53;
		matriz = matriz_cidades("ft53.atsp.txt", num_cidades);
		opt = 6905;
	} else if(!strcmp("ft70", str))
	{
		MAX = 9999999;
		printf("Resultado de ft70.atsp.txt: \n");
		num_cidades = 70;
		matriz = matriz_cidades("ft70.atsp.txt", num_cidades);
		opt = 38673;
	} else if(!strcmp("ry48p", str))
	{
		MAX = 9999999;
		printf("Resultado de ry48p.atsp.txt: \n");
		num_cidades = 48;
		matriz = matriz_cidades("ry48p.atsp.txt", num_cidades);
		opt = 14422;
	} else if(!strcmp("ftv38", str))
	{
		MAX = 100000000;
		printf("Resultado de ftv38.atsp.txt: \n");
		num_cidades = 38;
		matriz = matriz_cidades("ftv38.atsp.txt", num_cidades);
		opt = 1530;
	} else if(!strcmp("ftv64", str))
	{
		MAX = 100000000;
		printf("Resultado de ftv64.atsp.txt: \n");
		num_cidades = 64;
		matriz = matriz_cidades("ftv64.atsp.txt", num_cidades);
		opt = 1839;
	} else if(!strcmp("ftv70", str))
	{
		MAX = 100000000;
		printf("Resultado de ftv70.atsp.txt: \n");
		num_cidades = 70;
		matriz = matriz_cidades("ftv70.atsp.txt", num_cidades);
		opt = 1950;
	} else if(!strcmp("ftv170", str))
	{
		MAX = 100000000;
		printf("Resultado de ftv170.atsp.txt: \n");
		num_cidades = 170;
		matriz = matriz_cidades("ftv170.atsp.txt", num_cidades);
		opt = 2755;
	} else {
		erou = 0;
		printf("Instancia invalida, evite digitar valores errados\n");
	}

	if(erou)
	{
		begin = clock();
		if(!strcmp("y", resp))
		{
			solution = genetico(matriz, num_cidades, tamp, 39, lmt, crz, MAX);
		} else {
			solution = genetico(matriz, num_cidades, num_cidades * 2, 39, 20, num_cidades, MAX);	
		}
		
		end = clock();
		peso = peso_total(solution, num_cidades-1, matriz);
		for(i = 0; i < num_cidades; i++)
		{
			free(matriz[i]);
		}
		printf("Melhor solucao: \n");
		for(i = 0; i < num_cidades-2; i++)
		{
			printf("%d -> ", solution[i]);
		}
		printf("%d\n", solution[num_cidades-2]);
		printf("Peso total: %d\n", peso);
		free(matriz);
		free(solution);
		time_spent = 0.0;
		time_spent += (double)(end - begin)/CLOCKS_PER_SEC;
		printf("TEMPO DE EXECUÇÃO: %f\n", time_spent);
	}
}

int main(int* argc, char* argv[])
{	
	while(1)
	{
		printf("\n");
		printf("\n");
		printf("Instancias: \n");
		printf("br17\t");
		printf("ft53\t");
		printf("ft70\n");
		printf("ry48p\t");
		printf("ftv38\t");
		printf("ftv64\n");
		printf("ftv70\t");
		printf("ftv170\n");

		char instancia[100];
		char resp[100];
		int cruzamentos;
		int limite;
		int tam_pop;
		printf("Digite o nome da instancia desejada ou 0 para sair: ");
		scanf("%s", instancia);
		if(!strcmp("0", instancia))
		{
			break;
		}

		system("@cls||clear");
		while(1)
		{
			printf("Valores default: \n");
			printf("Tamanho da populacao = numero de cidades * 2\n");
			printf("Limite de iteracoes sem melhoria = 20\n");
			printf("Cruzamentos realizados = numero de cidades\n");
			printf("\n");
			printf("O numero de cidades pode ser identificado pelo nome da instancia, exemplo: \n");
			printf("br17 - Numero de cidades = 17\n");
			printf("Default: \n");
			printf("Tamanho da populacao = 34\n");
			printf("Limite de iteracoes sem melhoria = 20\n");
			printf("Cruzamentos realizados = 17\n");
			printf("CUIDADO! Recomenda-se utilizar os valores default!\n");
			printf("Valores muito altos podem estourar a memoria e gerar falhas de segmentacao\n");
			printf("Deseja alterar esses valores (y/n)?  ");
			scanf("%s", resp);
			if(!strcmp("y", resp))
			{
				system("@cls||clear");
				printf("Digite apenas numeros por favor, caso contrario a aplicacao ira parar!\n");
				printf("Quantidade de Cruzamentos?  ");
				scanf("%d", &cruzamentos);
				printf("\n");
				printf("Limite de iteracoes sem melhoria?  ");
				scanf("%d", &limite);
				printf("\n");
				printf("Tamanho da populacao?  ");
				scanf("%d", &tam_pop);
				printf("\n");
				break;
			}else if(!strcmp("n", resp))
			{
				break;
			}else{
				system("@cls||clear");
			}
		}
		
		roda_tudo(instancia, tam_pop, cruzamentos, limite, resp);
	}
	
    return 0;
}

