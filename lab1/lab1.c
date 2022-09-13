#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>


#define ELEMENTOS 10000
#define NTHREADS  2


float vetor[ELEMENTOS];
int elm_grupo = ELEMENTOS / NTHREADS;

void * aumenta10(void * arg){
    int *grupo = (int*) arg;
    printf("Grupo: %d\n", grupo);
    printf("+elm: %d\n", elm_grupo+grupo);
    for(int i= grupo; i < grupo+elm_grupo ; i++){
        vetor[i]+=vetor[i]*0.5;
        printf("Grupo %f -",vetor[i]);
    }
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid[NTHREADS];
    int grupo = 0;
    printf("%d", elm_grupo);
    for(int i=0; i < ELEMENTOS ; i++){
        vetor[i]=100.0;
    }
    for(int i=0; i < NTHREADS ; i++){
        if(pthread_create(&tid[i], NULL, aumenta10, (void *) grupo))
            printf("ERRO -- pthread\n");
        grupo += elm_grupo;
    }
    printf("final2\n");
    pthread_exit(NULL);
    return 0;
}