#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <pthread.h>

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

float *matriz_1, *matriz_2, *matriz_s; //matrizes de entrada e de saída
int nthreads; //numerdo de threads

typedef struct{
   int id; //identificador da thread que irá processar
   int linhas_1; //dimensões das matrizes
   int linhas_2; 
   int colunas_1;
   int colunas_2;
} tArgs;

void * mult_matriz(void *arg) {
   tArgs *args = (tArgs*) arg;
   printf("Thread %d\n", args->id);
   int tlinhas = args->linhas_1/nthreads;
   for(int i = tlinhas*args->id; i < tlinhas*args->id + tlinhas; i++) { 
      for(int j = 0; j<  args->colunas_2; j++){
         for (int k = 0; k < args->linhas_2; k++) {
               matriz_s[i*args->colunas_1+j] += matriz_1[i*args->colunas_1+k] * matriz_2[k*args->colunas_1+j];
         }
      }
    }
   pthread_exit(NULL);
}

int main(int argc, char*argv[]) {
   //float *matriz_1, *matriz_2, *matriz_s; //matrizes que serão carregadas do arquivo, matriz de saída
   int linhas_1, colunas_1, linhas_2, colunas_2, linhas_s, colunas_s; //dimensoes das matrizes
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de entrada
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   pthread_t *tid;
   tArgs *args;

   double tempo_1, tempo_2, tempo_3, tempo_4; //variáveis para medida de tempo
   
   GET_TIME(tempo_1);
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

    GET_TIME(tempo_2);
    //------------------multiplicação concorrente----------
   //cricação das threads
   for(int i = 0; i<nthreads;i++){
      (args+i) -> id=i;
      (args+i) ->linhas_1; //dimensões das matrizes
      (args+i) ->linhas_2; 
      (args+i) ->colunas_1;
      (args+i) ->colunas_2;
      if(pthread_create(tid+i, NULL, mult_matriz, (void*) (args+i))){
         puts("Erro--pthread create"); return 3;
      }
   }
   //espera pelo teminio das threads
   for (int i = 0; i < nthreads; i++){
      pthread_join(*(tid+i), NULL);
   }
   

    //multiplicação da matriz
    /*for(int i = 0; i < linhas_1; i++) { 
        for(int j = 0; j<  colunas_2; j++){
            for (int k = 0; k < linhas_2; k++) {
                matriz_s[i*colunas_1+j] += matriz_1[i*colunas_1+k] * matriz_2[k*colunas_1+j];
            }
        }
    }*/
    GET_TIME(tempo_3);

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
   free(args);
   free(tid);
   GET_TIME(tempo_4);
   printf("A multiplicacao da matriz levou %e segundos\n", tempo_3-tempo_2);
   printf("O inicio do programa levou %e segundos\n", tempo_2-tempo_1);
   printf("O final do programa levou %e segundos\n", tempo_4-tempo_3);
   return 0;
}

