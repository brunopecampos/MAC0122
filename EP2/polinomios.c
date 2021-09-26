/*    
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO-PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Bruno Pereira Campos
  NUSP: 11806764

  ep2 - polinomios.c

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* Obervacoes:
   - Sao implementadas apenas listas sem cabeca.
   Assim, todas as primeiras insercoes tem que ser
   tratadas de forma especial em cada loop.
   - O funcionamento das funcoes de operacao 
   aritimetica dependem de os polinomios estarem em 
   ordem crescente. Todos eles sao ordenados pela 
   chamada da funcao ordena() em leia().
   - Esse EP foi enviado mais de uma semana antes 
   da sua data de entrega, no entanto, depois dos 
   comentarios da professora a respeito da documentacao,
   resolvi deixa-lo com comentarios mais precisos e 
   fazer essas observacoes no inicio. O codigo em si
   nao foi alterado em relacao a versao anterior enviada.
   */

#include <stdio.h>
#include <stdlib.h>

#include "polinomios.h"

/* declaracao da celula das listas encadeadas */
struct Termo {
    float coef;
    unsigned int expo;
    struct Termo* prox;
};   
typedef struct Termo termo;

/* celula de uma lista encadeada de polinomios */
struct polinomio_pointer
{
  termo * pol;
  struct polinomio_pointer* prox;
};
typedef struct polinomio_pointer pol_pointer;


/* prototipo de eventuais funcoes auxiliares */
void *mallocSafe (int nbytes);

termo *novo_termo(float coef, int expo);

void ordena(termo **p);

void remove_primeiro(termo **p);

void transpoe_primeiro(termo **origem, termo **dest);

void inverta(termo **p);

void retifica(termo **p);

void remove_termo(termo **p);
/* implementacao das funcoes da biblioteca polinomios.h */

polinomio cria() {
  termo *p = novo_termo(0, 0);
  return (void *)p;
}

polinomio leia() {
  float coef;
  int expo, primeiro = 1;
  termo* ini;
  termo* p;

  while(1) {
    scanf("%f", &coef);
    if(!coef){
      break;
    }
    scanf("%d", &expo);

    /* cria e adiciona os termos na lista um a um */
    if(primeiro) {
      p = novo_termo(coef, expo);
      ini = p;
      primeiro = 0;
    } else {
      p->prox = novo_termo(coef, expo);
      p = p->prox;
    }
  }

  /* caso nao tenha sido informados
     nenhum termo, retorna o polinomio nulo */
  if(primeiro)
    return cria();

  /* ordena o polinomio antes de retorna-lo */
  ordena(&ini);

  return (polinomio *)ini;
}

polinomio copia(polinomio p) {
  termo *q = (termo *)p;
  termo *r;
  termo *ini;
  int primeiro = 1;

  while(q != NULL) {
    
    /* cria e copia os termos um a um */
    if(primeiro) {
      r = novo_termo(q->coef, q->expo);
      /* salva o endereco inicial do novo polinomio */
      ini = r;
      primeiro -= 1;
    }
    else {
      r->prox = novo_termo(q->coef, q->expo);
      r = r->prox;
    }

    q = q->prox;
  }

  return (polinomio *)ini;
} 

void impr(char c, polinomio p) {
  termo *pol;

  printf("\n%c", c);
  printf("(x) = ");

  for(pol = (termo *)p; pol != NULL; pol = pol->prox) {
    /* imprime o termo */
    if(pol->coef != 0) {
      printf("%.2f x^%d", pol->coef, pol->expo);
    }
    /* se o coeficiente do termo for nulo, imprime 0 */
    else {
      printf("0");
    }
    if(pol->prox != NULL) {
        printf(" + ");
    }
  }

  printf("\n");
}
/* 
   Recebe dois polinomios p e q e retorna uma polinio que 
   corresponde a soma dos dois. Funcinona da seguinte forma:
   
   - Os dois polinomios ja vem ordenados
   - O polinomio que tem o termo de menor grau 
   eh chamado de base e o outro de transposicao
   - Os termos da transposicao sao removidos e encaixados/somados
   em suas devidas posicoes na base, de forma que a soma,
   no final do processo, e a propria base e ela esta
   ordenada.

*/
polinomio soma(polinomio p, polinomio q) {

  termo *p_copia = (termo *)copia(p);
  termo *q_copia = (termo *)copia(q);
  termo *base, *tranposicao, *atual_base, *proximo_base, *atual_trans;

  /* escolhe qual polinomio sera a base */
  if(p_copia->expo <= q_copia->expo) {
    base  = p_copia;
    tranposicao = q_copia;
  } else {
    base = q_copia;
    tranposicao = p_copia;
  }

  /* comeca nas posicoes iniciais da base e
     de tranposicao */
  atual_base = base;
  atual_trans = tranposicao;
  proximo_base = atual_base->prox;

  /* repete o processo ate chegar ao final de 
     qualquer um dos polinomios */
  while((atual_base != NULL) && (atual_trans != NULL)) {

    /* verifica se atual_base esta no ultimo termo da base */
    if(proximo_base == NULL) {

      while(atual_trans != NULL) {
        /* soma os termos se tiverem o mesmo expoente */
        if(atual_trans->expo == atual_base->expo) {
          atual_base->coef = atual_base->coef + atual_trans->coef;
          remove_primeiro(&atual_trans);
          continue;
        } 
        /* imenda o final da base com o que restou da tranposicao */
        atual_base->prox = atual_trans;
        break;
      }

    } else {
      while((atual_trans != NULL) && (atual_trans->expo < proximo_base->expo)) {
        /* soma os termos se tiverem o mesmo expoente */
        if(atual_trans->expo == atual_base->expo) {
          atual_base->coef = atual_base->coef + atual_trans->coef;
          remove_primeiro(&atual_trans);
        }
        /*remove o termo da transposicao colocando-o na base */ 
        else {
          transpoe_primeiro(&atual_trans, &atual_base);
          atual_base = atual_base->prox;
        }
      }
    }

    /* vai para o proximo termo da base enquanto
       ele nao for nulo*/
    atual_base = proximo_base;
    if(proximo_base != NULL)
      proximo_base = proximo_base->prox;
    
  }

  retifica(&base);
  return (polinomio *)base;
}

polinomio subt(polinomio p, polinomio q) {
  return soma(p, nega(q));
}

polinomio nega(polinomio p) {
  
  termo *q = (termo *)copia(p);
  termo *ini;
  ini = q;
  while(q != NULL) {
    q->coef *= -1;
    q = q->prox;
  }

  retifica(&ini);
  return (polinomio *)ini;
}

/* Recebe dois polinomios p e q e retorna um que representa o 
   produto entre eles. Faz isso da seguinte forma:

   - Cria uma lista de polinomios que guarda
   os polinomios do produto de cada termo.
   - Soma todas esses polinomios */

polinomio mult(polinomio p, polinomio q) {
  termo *r = (termo *) p;
  termo *s = (termo *) q;
  termo *j;

  pol_pointer* pol_list;
  pol_pointer* pol_list_iterator;

  int primeiro_pol;
  int primeiro_termo;

  for(primeiro_pol = 1; r != NULL; r = r->prox) {

    /* cria um polinomio na lista de polinomios */
    if(primeiro_pol) {
      pol_list = mallocSafe(sizeof(pol_pointer));
      pol_list->prox = NULL;
      pol_list_iterator = pol_list;
      primeiro_pol = 0;
    }
    else {
      pol_list_iterator->prox = mallocSafe(sizeof(pol_pointer));
      pol_list_iterator->prox->prox = NULL;
      pol_list_iterator = pol_list_iterator->prox;
    }

    /* Adiciona os termos das listas de polinomios 
       que correspondem ao produto de um dos termos de p por
       todos os outros de q. */
    s = (termo *) q;
    for(primeiro_termo = 1; s != NULL; s = s->prox) {
      if(primeiro_termo) {
        pol_list_iterator->pol = novo_termo(r->coef * s->coef, r->expo + s->expo);
        j = pol_list_iterator->pol;
        primeiro_termo = 0;
      }
      else {
        j->prox = novo_termo(r->coef * s->coef, r->expo + s->expo);
        j = j->prox;
      }
    }
  }

  /* soma e libera todos os polinomios da lista de polinomios */
  for(pol_list_iterator = pol_list->prox; pol_list_iterator != NULL; ) {
    r = pol_list->pol;
    s = pol_list_iterator->pol;

    pol_list->pol =  soma(pol_list->pol, pol_list_iterator->pol);

    pol_list_iterator = pol_list_iterator->prox;

    libera((polinomio *)r);
    libera((polinomio *)s);
  }

  /*libera a lista de polinomios */
  r = (termo *)pol_list->pol;
  libera((polinomio *) pol_list);
  
  retifica(&r);
  return r;
}

/* Receve dois polinomios p e q e retorna o quociente entre os dois
   Faz isso por meio do algoritimo comum de divisao de polinomios */
polinomio quoc(polinomio p, polinomio q) {
  termo *r = (termo *)copia(p);                             
  termo *s = (termo *)copia(q);
  termo *aux;
  termo *produto;
  termo* quociente_lista;
  termo* quociente;
  termo* subtracao;

  if(s->expo == 0 && s->expo ==0 && s->prox == NULL) {
    printf("ERRO: Indeterminacao!\n");
    exit(1);
  }

  retifica(&r);
  retifica(&s);

  /* coloca dividendo e o divisor em ordem inversa */
  inverta(&r);
  inverta(&s);

  /* faz o processo de divisao ate que o expoente
     do que restou seja menor que o do divisor */
  int primeiro = 1;
  while(r->expo >= s->expo) {

    /* acha um dos termos do quociente e o
       coloca numa lista  */
    if(primeiro) {
      quociente_lista = novo_termo((r->coef / s->coef), (r->expo - s->expo));
      quociente = quociente_lista;
      primeiro = 0;
    } else {
      quociente->prox = novo_termo((r->coef / s->coef), (r->expo - s->expo));
      quociente = quociente->prox;
    }

    /* coloca o dividendo e o divisor em ordem,
       para o funcionamento certo das outras 
       funcoes aritimeticas */
    inverta(&r);
    inverta(&s);
    produto = (termo *)mult((polinomio *)s, (polinomio *)quociente);
    subtracao = (termo *)subt((polinomio *)r, (polinomio *)produto);

    /* iguala o dividendo a diferenca que restou
       da operacao com o termo atual do quociente */
    libera(produto);
    aux = r;
    r = subtracao;
    libera(aux);

    /* retifica o dividendo para tirar os termos
       de alto grau mas com coeficiente 0, para 
       evitar problemas na condicao do while */
    retifica(&r);

    /* coloca o dividendo e o divisor na ordem 
       inversa para recomeçar o processo */
    inverta(&r);
    inverta(&s);

  }

  if(primeiro) {
    quociente_lista = novo_termo(0, 0);
  }
  
  inverta(&quociente_lista);
  return (polinomio *)quociente_lista;
}

/* utiliza a relacao:
   dividendo = divisor * quociente + reto */
polinomio rest(polinomio p, polinomio q) {
  return subt(p, mult(q, quoc(p, q)));
}

void libera(polinomio p) {
  termo *p_termo = (termo *)p;
  termo *r;
  termo *q;

  /* enquanto o polinomio tem mais de um termo */
  while(p_termo->prox != NULL) {
    /* q vai pro inicio do polinomio */
    q = p_termo;
    /* q vai para a penultima posicao do polinomio */
    while(q->prox->prox != NULL) {
      q = q->prox;
    }
    /* ultima posicao e liberada */
    r = q->prox;
    q->prox = NULL;
    free(r);
  }

  /* libera o unico termo final do polinomio */
  free(p_termo);
}

/* Implementacao das funcoes auxiliares */
void *mallocSafe (int nbytes) {
    void *ptr;

    ptr = malloc(nbytes);
    if(ptr == NULL) {
        printf("Erro ao alocar a memória!");
        exit(1);
    }

    return ptr;
}

/* cria um novo termo */
termo *novo_termo(float coef, int expo) {
  termo *p = mallocSafe(sizeof(termo));
  p->coef = coef;
  p->expo = expo;
  p->prox = NULL;
  return p;
}

/* ordena um polinomio por meio do 
   algoritimo selection sort */
void ordena(termo** p) {
  termo *q = (*p);
  termo *r;
  termo *s;
  int menor, aux;

  while(q->prox != NULL) {

    r = q->prox;

    menor = q->expo;
    /* acha o menor termo */
    while(r != NULL) {
      if(r->expo < menor) {
        menor = r->expo;
        s = r;
      }
      r = r->prox;
    }
    /* se o menor termo for nao for o que 
       esta mais a esquerda, troca ele 
       com o que esta mais a esquerda */
    if(menor != q->expo) {
      aux = q->coef;
      q->coef = s->coef;
      s->coef = aux;

      aux = q->expo;
      q->expo = s->expo;
      s->expo = aux;
      
    }

    q = q->prox;
  }

}

/* remove o primeiro termo de um polinomio */
void remove_primeiro(termo **p) {
  termo *q = *p;
  *p = (*p)->prox;
  free(q);
}

/* insere o primeiro termo de um polinomio na segunda 
   posicao de outro, removendo o primeiro termo 
   do segundo */
void transpoe_primeiro(termo **p, termo **q) {
  termo *r, *s;

  /* r e s apontam para os proximos termos
     de q e p*/
  r = (*q)->prox;
  s = (*p)->prox;

  /* troca os valores de p e q */
  (*q)->prox = (*p);
  (*p)->prox = r;
  (*p) = s;

}

/* inverte a ordem do polinomio
   OBS: copiada do slide da Cris*/
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

/* remove um termo que nao seja o primeiro */
void remove_termo(termo **p) {
  termo *aux = (*p)->prox;
  (*p)->prox = (*p)->prox->prox;
  free(aux);
}

/* tira alguns execessos do polimio
   como termos de mesmo expoente e 
   termos de coeficiente 0 */
void retifica(termo **p) {
  termo *q = (*p);

  while(q->prox != NULL) {
    /* remove os termos de coeficiente 0 */
    if( (q->prox->coef == 0) ) {
      remove_termo(&q);
    }
    /* junta os termos de mesmo expoente */
    else if(q->expo == q->prox->expo) {
      q->coef = q->coef + q->prox->coef;
      remove_termo(&q);
    }
    else {
      q = q->prox;
    }
  }

  /* caso seja um polinomio nulo,
     faz o expoente do termo unico
     tambem ser nulo */
  if((*p)->prox == NULL && ((*p)->coef == 0)) {
    (*p)->expo = 0;
  }
  /* se o primeiro termo for nulo, remove ele */
  else if((*p)->coef == 0) {
    remove_primeiro(&(*p));
  }
}
