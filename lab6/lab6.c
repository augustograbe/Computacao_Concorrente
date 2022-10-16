#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define T1 4 //numero de threads 
#define T2 4 //numero de threads 
#define T3 4 //numero de threads

//variavel inteira inicializada com valor 0 (representando a nossa base de dados)
int bd=0;

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//entrada leitura
void InicLeit (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] quer ler\n", id);
   while(escr > 0) {
     printf("L[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_leit, &mutex);
     printf("L[%d] desbloqueou\n", id);
   }
   leit++;
   pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] terminou de ler\n", id);
   leit--;
   if(leit==0) pthread_cond_signal(&cond_escr);
   pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] quer escrever\n", id);
   while((leit>0) || (escr>0)) {
     printf("E[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_escr, &mutex);
     printf("E[%d] desbloqueou\n", id);
   }
   escr++;
   pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] terminou de escrever\n", id);
   escr--;
   pthread_cond_signal(&cond_escr);
   pthread_cond_broadcast(&cond_leit);
   pthread_mutex_unlock(&mutex);
}

//thread 1
void * tarefa1 (void * arg) {
  int *id = (int *) arg;
  while(1) {
    InicEscr(*id);
    bd++;
    FimEscr(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

//thread 2
void * tarefa2 (void * arg) {
  int *id = (int *) arg;
  while(1) {
    InicLeit(*id);
    if (bd%2==0)
      printf("O valor %d e par\n", bd);
    else
      printf("O valor %d e impar\n", bd);
    FimLeit(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

//thread 3
void * tarefa3 (void * arg) {
  int *id = (int *) arg;
  int boba1, boba2;
  while(1) {
    InicLeit(*id);//leitura
    printf("%d", bd);
    FimLeit(*id);
    /* faz alguma coisa inutil pra gastar tempo... */
    boba1=100; boba2=-100; while (boba2 < boba1) boba2++;
    InicEscr(*id);//escrita
    bd=*id;
    FimEscr(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}


//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[T1+T2+T3];
  int id[T1+T2+T3];

  //inicializa as variaveis de sincronizacao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_leit, NULL);
  pthread_cond_init(&cond_escr, NULL);

  //cria as threads T1
  for(int i=0; i<T1; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, tarefa1, (void *) &id[i])) exit(-1);
  } 
  
  //cria as threads T2
  for(int i=0; i<T2; i++) {
    id[i+T1] = i+1;
    if(pthread_create(&tid[i+T1], NULL, tarefa2, (void *) &id[i+T1])) exit(-1);
  } 

  //cria as threads T3
  for(int i=0; i<T3; i++) {
    id[i+T1+T2] = i+1;
    if(pthread_create(&tid[i+T1+T2], NULL, tarefa3, (void *) &id[i+T1+T2])) exit(-1);
  } 

  pthread_exit(NULL);
  return 0;
}