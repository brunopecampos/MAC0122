
/* stackc: IMPLEMENTACAO DA PILHA */

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

static void *mallocSafe (unsigned int n);

/*
 * PILHA: uma implementacao com lista encadeada com cabeca
 */
typedef struct stackNode* Link;
struct stackNode { 
  Item conteudo; 
  Link prox; 
};

static Link topo;

void stackInit(int n) { 
  topo = (Link) mallocSafe(sizeof *topo);
  topo->prox = NULL;
}

int stackEmpty() { 
  return topo->prox == NULL; 
}

void stackPush(Item item) { 
  Link nova = (Link) mallocSafe(sizeof *nova);
  nova->conteudo = item;
  nova->prox = topo->prox;
  topo->prox = nova;
}

Item stackPop() { 
  Link p = topo->prox;
  Item item;

  if (p == NULL) { /* stackempty() */
    fprintf(stderr,"Putz, voce nao sabe o que esta fazendo!\n");
    exit(-1);
  }
  /* tudo bem, a pilha nao esta vazia... */
  item = p->conteudo;
  topo->prox = p->prox;
  free(p);
  return item;  
}

Item stackTop() {
  if (topo->prox == NULL) { /* stackempty() */
    fprintf(stderr,"Putz, voce nao sabe o que esta fazendo!\n");
    exit(-1);
  }
  
  /* tudo bem, a pilha nao esta vazia... */
  return  topo->prox->conteudo;
}

void stackFree() {
  while (topo != NULL) {
    Link p = topo;
    topo = topo->prox;
    free(p);
  }
}

void stackDump() {
  Link p = topo->prox;
  fprintf(stdout, "pilha: ");
  if (p == NULL) fprintf(stdout, "vazia.");
  while (p != NULL) {
    fprintf(stdout," %c", p->conteudo);
    p = p->prox;
  }
  fprintf(stdout,"\n");
}

static void *mallocSafe (unsigned int n) {
  void *p;
  
  p = malloc(n);
  if (p == NULL) {
    fprintf(stderr,"stack: malloc de %u bytes falhou.\n", n);
    exit (-1);
  }
  return p;
}
