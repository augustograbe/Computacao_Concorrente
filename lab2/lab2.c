#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//descomentar o define abaixo caso deseje imprimir uma versao truncada da matriz gerada no formato texto
#define TEXTO 1

#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>
#define BILLION 1000000000L

/* The argument now should be a double (not a pointer to a double) */
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif

int main(int argc, char*argv[]) {
   float *matriz_1, *matriz_2, *matriz_s; //matrizes que serão carregadas do arquivo, matriz de saída
   int linhas_1, colunas_1, linhas_2, colunas_2, linhas_s, colunas_s; //dimensoes das matrizes
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de entrada
   size_t ret; //retorno da funcao de leitura no arquivo de entrada

   double start, finish, elapsed; //variáveis para medida de tempo
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <arquivo entrada 1> <arquivo entrada 2> <arquivo saida>\n", argv[0]);
      return 1;
   }
   
   //---Leitura da matriz 1
   //abre o arquivo 1 para leitura binaria
   descritorArquivo = fopen(argv[1], "rb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo 1\n");
      return 2;
   }

   //le as dimensoes da matriz 1
   ret = fread(&linhas_1, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 1\n");
      return 3;
   }
   ret = fread(&colunas_1, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 1\n");
      return 3;
   }
   tam = linhas_1 * colunas_1; //calcula a qtde de elementos da matriz 1

   //aloca memoria para a matriz 1
   matriz_1 = (float*) malloc(sizeof(float) * tam);
   if(!matriz_1) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 1\n");
      return 3;
   }

   //carrega a matriz 1 de elementos do tipo float do arquivo 1
   ret = fread(matriz_1, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 1\n");
      return 4;
   }

    //---Leitura da matriz 2
   //abre o arquivo 2 para leitura binaria
   descritorArquivo = fopen(argv[2], "rb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo 2\n");
      return 2;
   }

   //le as dimensoes da matriz 2
   ret = fread(&linhas_2, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 2\n");
      return 3;
   }
   ret = fread(&colunas_2, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 2\n");
      return 3;
   }
   tam = linhas_2 * colunas_2; //calcula a qtde de elementos da matriz

   //aloca memoria para a matriz 2
   matriz_2 = (float*) malloc(sizeof(float) * tam);
   if(!matriz_2) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 2\n");
      return 3;
   }

   //carrega a matriz 2 de elementos do tipo float do arquivo 2
   ret = fread(matriz_2, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 2\n");
      return 4;
   }

    //verifica se é possível realizar a multiplicação entre as duas matrizes
    if (colunas_1 != linhas_2){
      fprintf(stderr, "Erro. Não é possível realizar a multiplicação das matrizes com essas dimensões\n");
      return 4;
    }
    tam = linhas_2 * colunas_1;

    //aloca mamoria para matriz de saída
    matriz_s = (float*) malloc(sizeof(float) * tam);
    if(!matriz_s) {
      fprintf(stderr, "Erro de alocao da memoria da matriz de saída\n");
      return 3;
    }
    //inicializa matriz de saída
     for(int i = 0; i < linhas_2; i++) { 
        for(int j = 0; j<  colunas_1; j++){
            matriz_s[i*colunas_1+j]=0.0;
        }
    }

    GET_TIME(start);
    //------------------multiplicação----------
    //multiplicação da matriz
    for(int i = 0; i < linhas_1; i++) { 
        for(int j = 0; j<  colunas_2; j++){
            for (int k = 0; k < linhas_2; k++) {
                matriz_s[i*colunas_1+j] += matriz_1[i*colunas_1+k] * matriz_2[k*colunas_1+j];
            }
        }
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("A multiplicacao da matriz levou %e segundos\n", elapsed);

    //-------------------Saida----------
   //imprime a matriz na saida padrao
   /*for(int i=0; i<linhas_2; i++) { 
      for(int j=0; j<colunas_1; j++)
        fprintf(stdout, "%.6f ", matriz_s[i*colunas_1+j]);
      fprintf(stdout, "\n");
   }*/


   //escreve a matriz no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(argv[3], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de linhas e de colunas
   ret = fwrite(&linhas_2, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da matriz
   ret = fwrite(&colunas_1, sizeof(int), 1, descritorArquivo);
   ret = fwrite(matriz_s, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }


   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   free(matriz_1);
   free(matriz_2);
   free(matriz_s);
   return 0;
}

