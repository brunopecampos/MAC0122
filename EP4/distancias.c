/*
 *  distancias.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "posicao.h"
#include "queue.h"

/* 
 *  P A R T E   I.   P R O T O T I P O S
 */

int *distancias (int n, int **a, int c);
void *mallocSafe (unsigned int n);

/*  
 * P A R T E  II.  M A I N  
 */

int main(int argc, char *argv[]) {  
  FILE *fp_e; /* arquivo de estradas */
  FILE *fp_s; /* aqruivo de saida */

  char *nome_arq_e = NULL; /* nome do arquivo de estradas */
  char *nome_arq_s = NULL; /* nome do arquivo de saida    */
  
  int **a; /* a[i][j] == 1 se existe estrada de i a j */
  int no_c; /* numero de cidades */
  int origem = 0; /* cidade origem, default e' 0 */
  int *dist; /* dist[i] = comprimento do menor caminho de c a i */
  int i, j;
  
  if (argc == 1) {
    fprintf(stdout,"Uso: %s -c<cid_origem> -f <arq_estradas> [-s <arq-saida>]\n", argv[0]);
    return 0;
  }
  
  /* Examina os argumentos na linha de comando */
  for (i = 1; i < argc; i++) {
    if (sscanf(argv[i],"-c%d",&origem)==1);
    else if (strncmp(argv[i],"-f",2)==0) nome_arq_e = argv[++i];
    else if (strncmp(argv[i],"-s",2)==0) nome_arq_s = argv[++i];
    else {
      fprintf(stderr, "Uso: %s -c<cid_origem> -f <arq_estradas> [-s <arq-saida>]\n", argv[0]);
      return -1;
    }
  }
  
  if ((fp_e = fopen(nome_arq_e,"r")) == NULL) {
    fprintf(stderr,"\n%s: nao posso abrir arquivo %s.\n", argv[0], nome_arq_e);
    return EXIT_FAILURE;
  }
  
  if (nome_arq_s == NULL) fp_s = stdout; /* saida default e' a padrao */
  else if((fp_s = fopen(nome_arq_s,"w")) == NULL) {
    fprintf(stderr, "%s: arquivo de saida %s nao pode ser criado.\n", argv[0], nome_arq_s);
    return EXIT_FAILURE;
  }
  
  /* le numero de cidades */
  fscanf(fp_e,"%d ", &no_c);
  
  /* aloca matriz de estradas */
  a = (int**) mallocSafe(no_c * sizeof(int*));
  for (i = 0; i < no_c; i++)
    a[i] = (int*) mallocSafe(no_c * sizeof(int));
  
  /* leitura das estradas */  
  for (i = 0; i < no_c; i++) 
    for (j = 0; j < no_c; j++)
      if (fscanf(fp_e,"%d ", &a[i][j]) != 1) {
	fprintf(stderr,"\n%s: faltam dados no arquivo %s.\n", argv[0], nome_arq_e);
	return -1;
      }
  fclose(fp_e);
  
  dist = distancias(no_c, a, origem);

  /* escreve a resposta */
  fprintf(fp_s, "\nDistancias a partir da cidade %d:\n", origem);

  for (i = 0; i < no_c; i++) 
    fprintf(fp_s, "%d: %d\n", i, dist[i]);

  fclose(fp_s);
  
  /* free memoria alocada */
  for (i = 0; i < no_c; i++)
    free(a[i]);
  
  free(a);
  return EXIT_SUCCESS;
}

/*  
 * P A R T E III.  I M P L E M E N T A C A O   D A S   F U N C O E S
 */

int *distancias (int n, int **a, int c) {
  int j;
  int *d;
  
  Queue q = queueInit(n); /* declara e inicializa a fila */ 

  /* aloca vetor de distancias */
  d = (int*) mallocSafe(n * sizeof(int));
 
  /* inicializa o vetor de distancias */  
  for (j = 0; j < n; j++)  
    d[j] = n; /* distancia n = infinito */
  d[c] = 0;
  
  queuePut(q,c); /* coloca c na fila */
  
  /* queueempty(): a fila esta vazia */ 
  while (!queueEmpty(q)) {
    int i = queueGet(q); /* i recebe o 1o. elemento da fila */
    int di = d[i];
    
    for (j = 0; j < n; j++)
      if (a[i][j] == 1 && d[j] > di+1) {
	d[j] = di + 1;
	queuePut(q,j); /* coloca j na fila */
      }
  }
  
  queueFree(q);
  
  return d;
}

void *mallocSafe (unsigned int n) {
  void *p;

  p = malloc(n);
  if (p == NULL) {
    fprintf(stderr,"stack: malloc de %u bytes falhou.\n", n);
    exit (-1);
  }
  return p;
}
