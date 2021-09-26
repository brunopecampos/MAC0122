/*
  NAO MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO.

  NO ENTANTO, VOCE PODE ACRESCENTAR ALGO QUE ACHAR CONVENIENTE
  COMO PROTOTIPOS DE NOVAS FUNCOES.

  Aqui estao declarados o tipo Filme, ListaFilmes e os prototipo das 
  funcoes que manipulam a filmes e listas de filmes.
  
*/


#ifndef _FILMES_H
#define _FILMES_H

#include "util.h" /* Bool */
#include "main.h" /* Criterio */

#define TAM_DIST 10

/*----------------------------------------------------------*/
/* Estrutura Basica de uma Lista de Filmes                  */
/* Uma lista de filmes será uma lista __duplamente ligada__ */   
/* __com cabeca__ de objetos do tipo Filme                  */

typedef struct celFilme Filme;
struct celFilme 
{
    /* campos referentes a infos sobre o filme */
    char     dist[TAM_DIST+1]; /* distribuicao de notas do filme */
                               /* +1 para o '\0' */
    int      votos;       /* numero de votos */
    float    nota;        /* nota do filme */
    char    *nome;        /* nome do filme */
    int      ano;         /* ano  do filme */
   
    /* celula de uma lista duplamente ligada */
    Filme   *prox;    /* ponteiro para o proximo filme */
    Filme   *ant;     /* ponteiro para o filme anterior */
};

typedef struct listaFilmes ListaFilmes;
struct listaFilmes 
{
    int   nFilmes; /* no. de filmes na lista */ 
    Filme *cab;    /* ponteiro para a celula cabeca da lista de filmes */
};

/*----------------------------------------------------------*/
/* prototipos das funcoes que manipulam a lista de filmes
   e as estrutura dos filmes */

ListaFilmes *
crieListaFilmes();

ListaFilmes *
crieListaMelhoresFilmes(ListaFilmes *lst, int numero, float nota, int votos);

ListaFilmes *
crieListaPioresFilmes(ListaFilmes *lst, int numero, float nota, int votos);

Filme *
crieFilme(char *dist, int votos, float nota, char *nome, int ano);

int
listaEstaOrdenada(ListaFilmes *lst);

void 
libereListaFilmes(ListaFilmes *lst);

void 
libereFilme(Filme *flm);

Bool 
contemFilme(ListaFilmes *lst, Filme *flm);

void 
insiraFilme(ListaFilmes *lst, Filme *flm);

void
insiraFilmePosicao(ListaFilmes *lst, Filme* flm, Filme* posicao);

void 
removaFilme(ListaFilmes *lst, Filme *flm);

void 
mergeSortFilmes(ListaFilmes *lst, Criterio criterio, int p, int r);

void 
quickSortFilmes(ListaFilmes *lst, Criterio criterio);

void 
inverteListaFilmes(ListaFilmes *lst);

void
intercalaFilmes(Filme *p, Filme *q, Filme *r, ListaFilmes* lst, Criterio crit);

void 
hashFilmes(ListaFilmes *lst);
#endif
