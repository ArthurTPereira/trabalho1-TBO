#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "euclidian_distance.h"
#include "vetor.h"

#define SUCCESS 0
#define ERROR -1

struct distancias { // isso aqui vai ficar temporario eu acho
    double distancia;
    int linha;
    int coluna;
};

int main(int argc, char* argv[]) {

    // Verifica se o numero de argumentos eh valido
    if (argc != 4) {
        printf("Numero de argumentos invalido!\n");
        return ERROR;
    } 

    // Declara as variaveis que armazenam os argumentos
    char* nomeArquivoEntrada = argv[1];
    char* nomeArquivoSaida = argv[3];
    int k = atoi(argv[2]);

    // Abre o arquivo de entrada
    FILE* arquivoEntrada = fopen(nomeArquivoEntrada, "r");
    if (arquivoEntrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada!\n");
        return ERROR;
    }

    // Declara o buffer que armazena as linhas do arquivo de entrada
    size_t bufferSize = 64;
    char * buffer = (char*) malloc(bufferSize * sizeof(char));

    // Verifica se o buffer foi alocado corretamente
    if (buffer == NULL) {
        printf("Erro ao alocar memoria para o buffer!\n");
        return ERROR;
    }

    // Le o arquivo de entrada e calcula a dimensao M dos pontos
    getline(&buffer, &bufferSize, arquivoEntrada);
    int m = calculaM(buffer);

    // Volta o arquivo de entrada para o começo
    rewind(arquivoEntrada);

    int v_max = 1; // tamanho maximo do vetor das linhas do arquivo de entrada
    int nPontos = 0; // Numero de pontos atual

    // Coleta as linhas do arquivo de entrada e preenche em um vetor de strings
    char** pontos = preencheVetorLinhas(buffer, &bufferSize, arquivoEntrada, &nPontos, &v_max);

    // Fecha o arquivo de entrada
    fclose(arquivoEntrada);

    int n = nPontos-1;
    int tamanho = (n)*(n-1)/2;

    // vetor de distancia
    VetorDistancia** distancias = alocaVetorDistancia(tamanho);

    double vet1[m];
    double vet2[m];

    char** nomesPontos = (char**) malloc((nPontos-1) * sizeof(char*));

    
    int v = 0;
    char* pt;
    int cont = 0;
    char temp[1000];
    for (int i = 0; i < nPontos-1; i++) {
        strcpy(temp, pontos[i]);
        pt = strtok(temp,",");
        nomesPontos[i] = (char*) malloc((strlen(pt)+1) * sizeof(char));
        if (nomesPontos[i] == NULL) {
            printf("Erro ao alocar memoria para o nome!\n");
            return ERROR;
        }
        strcpy(nomesPontos[i], pt);
        pt = strtok(NULL,",");
        while (pt) {
            vet1[cont] = strtold(pt, NULL);
            cont++;
            pt = strtok(NULL,",");
        }
        cont = 0;
        for (int j = i+1; j < nPontos-1; j++) {
            strcpy(temp, pontos[j]);
            pt = strtok(temp,",");
            pt = strtok(NULL,",");
            while (pt) {
                vet2[cont] = strtod(pt, NULL);
                cont++;
                pt = strtok(NULL,",");
            }
            cont = 0;
            atribuiDistancia(distancias,v,i,j,euclidian_distance(vet1, vet2, m));
            v++;
        }
    }

    // ordena o vetor de distancia
    qsort(distancias, tamanho, sizeof(VetorDistancia*), comparaDistancia);

    // Abre o arquivo de saida
    FILE* arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (arquivoSaida == NULL) {
        printf("Erro ao abrir o arquivo de saida!\n");
        return ERROR;
    }

    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivoSaida, "%.15f\n", distancias[i]->distancia);
    }

    fclose(arquivoSaida);

    // Libera os vetores
    liberaVetorPontos(pontos, nPontos);
    liberaVetorDistancia(distancias, tamanho);
    liberaVetorNomes(nomesPontos, nPontos-1);


    return SUCCESS;
}