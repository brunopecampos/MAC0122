#include<stdio.h>
#include<stdlib.h>

struct Termo {
    float coef;
    unsigned int expo;
    struct Termo* prox;
};   

typedef struct Termo termo;

void *mallocSafe (int nbytes) {
    void *ptr;

    ptr = malloc(nbytes);
    if(ptr == NULL) {
        printf("Erro ao alocar a memória!");
        exit(1);
    }

    return ptr;
}

termo *novo_termo(float coef, int expo) {
  termo *p = mallocSafe(sizeof(termo));
  p->coef = coef;
  p->expo = expo;
  p->prox = NULL;
  return p;
}

void transpoe_primeiro(termo **origem, termo **dest) {
  termo *q;
  termo *nova_origem;
  q = (*dest)->prox;
  nova_origem = (*origem)->prox;
  (*dest)->prox = (*origem);
  (*origem)->prox = q;
  (*origem) = nova_origem;

}

void impr(char c, termo *p) {
  termo *pol;

  printf("\n%c", c);
  printf("(x) = ");

  for(pol = (termo *)p; pol != NULL; pol = pol->prox) {
    if(pol->coef != 0) {
      printf("%.2f x^%d", pol->coef, pol->expo);
    }
    else {
      printf("0");
    }
    if(pol->prox != NULL) {
        printf(" + ");
    }
  }

  printf("\n");
}

void inverta(termo **ini) {
  termo *p, *q, *r;
  p = NULL;
  q = *ini;
  while (q != NULL) {
    r = q->prox;
    q->prox = p;
    p = q;
    q = r;
  }
  *ini = p;
}

void remove_termo(termo **p) {
  termo *aux = (*p)->prox;
  (*p)->prox = (*p)->prox->prox;
  free(aux);
}

void remove_primeiro(termo **p) {
  termo *q = *p;
  *p = (*p)->prox;
  free(q);
}

void retifica(termo **p) {
  
  termo *j = (*p);
  if((*p)->prox != NULL) {
    while(!j->expo) {
      remove_primeiro(&(*p));
      j = (*p);
    }
    
    while(j->prox != NULL) {
      if(j->expo == j->prox->expo) {
        j->coef = j->coef + j->prox->coef;
        remove_termo(&j);
      }
      else {
        j = j->prox;
      }
    }
  }
}

int main() {
    termo *base = novo_termo(0,0);
    base->prox = novo_termo(0, 0);
    base->prox->prox = novo_termo(3,3);
    base->prox->prox->prox = novo_termo(4,3);
    base->prox->prox->prox->prox = novo_termo(4,4);
    termo *trans = novo_termo(4,4);
    trans->prox = novo_termo(5,5);
    trans->prox->prox = novo_termo(6,6);
    printf("meu pau");
    retifica(&base);
    impr('q', base);
    return 0;
}