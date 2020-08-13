#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MAXWORKERS 4
#define ATIVO 1
#define INATIVO 0
#define NUMBERS 200
//Array de números, contador de n primos e total de numeros a serem analisados
int n[NUMBERS], nPrimos, tN;
//Indice do vetor
int iN = 0;
//Mutex
pthread_mutex_t controle;
//Funcao para verificar caso seja primo
int vPrimo(int n)
{
  int i;
  int nMetade;
  //Casos bases
  if(n == 0 || n == 1)
  {
    return 0;
  }
  else
  {
     nMetade = n / 2;
     //Verifica caso o numero seja divisivel por algum outro alem dele mesmo
     for(i = 2; i <= nMetade; i++)
     {
       if(n % i == 0)
       {
         return 0;
       }
     }
     return 1;
  }
}

//Funcao da thread
void* worker(void *arg)
{
  //Recebe o argumento por meio de uma var temporaria
  int *tmp = (int*)arg;
  int id = *tmp;
  //Enquanto todos os numeros não forem analisados a thread ira continuar analisando
  while(iN < tN)
  {
    //Trava o mutex para alterar a quantidade de numeros primos e o indice do vetor
    pthread_mutex_lock(&controle);
    nPrimos += vPrimo(n[iN]);
    iN++;
    pthread_mutex_unlock(&controle);
  }
  return NULL;
}

int main(int argc, char **argv)
{
  //Quantidades de threads necessarias e contadores i e j
  int nThreads, i, j;
  //Char auxiliador para leitura da entrada
  char c;
  //Thread e o id
  pthread_t thread[MAXWORKERS];
  int idThread[MAXWORKERS];
  //Instanciando os valores
  tN = i = 0;
  c = 'a';
  //Enquanto a entrada nao terminar em \n sera lido o valor inteiro
  while(c != '\n')
  {
    scanf("%d%c", &n[tN], &c);
    tN++;
  }

  //De acordo com a quantidade de numeros na entrada seram necessarios diferentes quantidades de threads
  //Caso a quantidade de numeros sejam menor do que a total
  if(tN <= MAXWORKERS)
  {
    //A quantidade de threads sera a mesma da quantidade de numeros da entrada
    nThreads = tN;
  }
  else
  {
    //Caso a quantidade de numeros da entrada seja maior o limite de threads sera o maximo
    nThreads = MAXWORKERS;
  }
  //Rotulando cada thread com um id
  for(j = 0; j < nThreads; j++)
  {
    idThread[j] = j;
  }
  //Disparando a quantidade de threads necessaria
  for(j = 0; j < nThreads; j++)
  {
    pthread_create(&thread[j], NULL, worker, (void*)(&idThread[j]));
  }
  //Espera todas as threads serem encerradas
  for(j = 0; j < nThreads; j++)
  {
    pthread_join(thread[j], NULL);
  }
  //Imprimi a quantidade de numeros primos
  printf("%d\n", nPrimos);

  return 0;
}