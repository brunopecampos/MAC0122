/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO

  NO ENTANTO, VOCE PODE ACRESCENTAR ALGO QUE ACHAR CONVENIENTE
  COMO PROTOTIPOS DE NOVAS FUNCOES.

  Aqui estao os prototipo das funcoes de entrada e saida 
  de filmes.
  
*/

#ifndef _IOFILMES_H
#define _IOFILMES_H

#include "util.h"   /* Bool */
#include "filmes.h" /* ListaFilmes, Filme */
#include "main.h"

void
carregueListaFilmes(ListaFilmes *lst, Bool semRepeticoes);

void
graveListaFilmes(ListaFilmes *lst);

void
insereFilme(ListaFilmes *lst);

void
removeFilme(ListaFilmes *lst, char *pal);

void
mostreFilme(Filme *flm);

void
ordeneMergeSortFilmes(ListaFilmes *lst, Criterio crit);

void
mostreListaFilmes(ListaFilmes *lst);

void
mostrePioresFilmes(ListaFilmes *lst);

void
mostreMelhoresFilmes(ListaFilmes *lst);

void
limpeListaFilmes(ListaFilmes *lst);

Filme*
procuraFilme(ListaFilmes *lst, char *pal);


#endif
