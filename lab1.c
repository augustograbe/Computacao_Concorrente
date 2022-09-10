#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>


#define ELEMENTOS 10000
#define NTHREADS  2


int vetor[ELEMENTOS];

void * aumenta10(void * arg){
    int grupo = * (int*) arg;
    printf("Grupo: %d\n", grupo);
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid[NTHREADS];
    int elm_grupo = ELEMENTOS / NTHREADS;
    for(int i=0; i <= ELEMENTOS ; i+=elm_grupo){
        if(pthread_create(&tid[i-1], NULL, aumenta10, (void *)&i))
            printf("ERRO -- pthread\n");
    }

    printf("fim\n");
    pthread_exit(NULL);
    return 0;
}