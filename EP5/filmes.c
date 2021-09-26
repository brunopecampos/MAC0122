/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Bruno Pereira Campos
  NUSP: 11806764

  IMDB: filmes.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/


/*----------------------------------------------------------*/
/* filmes.h e a interface para as funcoes neste modulo      */
#include "filmes.h" 

/*----------------------------------------------------------*/
#include <stdlib.h>  /* NULL, free() */
#include <stdio.h>   /* printf(), scanf() */ 
#include <string.h>  /* strlen(), strncpy(), strcmp(), strtok() */

#include "util.h"    /* Bool, mallocSafe() */
#include "iofilmes.h"
#include "st.h"      /* freeST(), initST(), putFilmeST(), getFilmeST(),
                        showST(), freeST() */

/*----------------------------------------------------------------------
 *  crieFilme
 *
 *  Recebe informacoes dobre um filme 
 *
 *      - DIST  : distribuicao de notas
 *      - VOTOS : numero de votos
 *      - NOTA  : nota do filme 
 *      - ANO   : ano de producao do filme
 *
 *  e cria uma celula do tipo Filme para armazenar essa informacao. 
 *  A funcao retorna o endereco da celula criada.
 */
Filme *
crieFilme (char dist[], int votos, float nota, char *nome, int ano)
{
    Filme *flm;
    int    len = strlen(nome) + 1; /* +1 para o '\0' */
    
    flm = mallocSafe(sizeof *flm);
    
    strncpy(flm->dist, dist, TAM_DIST+1); /* +1 para o '\0' */
    
    flm->votos = votos;
    flm->nota  = nota;
    
    flm->nome = mallocSafe(len*sizeof(char));
    strncpy(flm->nome, nome, len);
    
    flm->ano  = ano;

    flm->prox = flm->ant = NULL; /* paranoia */
    
    return flm;
}

/*----------------------------------------------------------------------
 *  crieListaFilmes
 * 
 *  Cria uma estrutura que representa lista de filmes vazia.
 *  A funcao retorna o endereco dessa estrutura.
 *
 *  Um lista de filmes e representada por uma lista encadeada 
 *  duplamente ligada com cabeca. 
 */
ListaFilmes *
crieListaFilmes()
{
    ListaFilmes *lst;
    lst = mallocSafe(sizeof(ListaFilmes));

    Filme *cab;
    cab = mallocSafe(sizeof(Filme));

    lst->cab = cab;

    cab->prox = cab;
    cab->ant = cab;
    

    return lst;
}

/*----------------------------------------------------------------------
 *  libereListaFilmes(lst)
 *
 *  Recebe um ponteiro lst para uma estrutura que representa uma lista 
 *  de filmes e libera toda a memoria alocada para a lista.
 *
 *  Esta funcao utiliza a funcao libereFilme().
 */

void
libereListaFilmes(ListaFilmes *lst)
{
    Filme* i;
    while (lst->cab->prox != lst->cab)
    {
        i = lst->cab->prox;
        
        lst->cab->ant = i->ant;
        i->ant->prox = lst->cab;
        libereFilme(i);
        i = i->prox;
    }
    i = lst->cab;
    lst->cab = NULL;
    libereFilme(i);
    free(lst);
    
}

/*----------------------------------------------------------------------
 *  libereFilme
 *
 *  Recebe um ponteiro FLM para uma estrutura que representa um 
 *  filme e libera a area alocada.
 *
 */
void 
libereFilme(Filme *flm)
{
    free(flm);
}

/*----------------------------------------------------------------------
 *  insiraFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao insere o filme na lista.
 *  
 */
void 
insiraFilme(ListaFilmes *lst, Filme *flm)
{
    lst->nFilmes++;
    flm->ant = lst->cab->ant;
    flm->prox = lst->cab;
    lst->cab->ant->prox = flm;
    lst->cab->ant = flm;
}

/*---------------------------------------------------------------------
 *  contemFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao retorna  TRUE se o filme esta na lista e 
 *  FALSE em caso contrario. 
 *
 *  Consideramos que dois filmes f e g sao iguais se
 *
 *       - f->nome e' igual a g->nome 
 *       - f->nota e' igual a g->nota
 *       - f->ano  e' igual a g->ano 
 *
 *  Para comparar dois nomes voce pode usar alguma funcao da 
 *  bibliteca do c  como strcmp, strncmp (string,h) 
 *  ou a funcao strCmp (util.h).
 *
 */
Bool 
contemFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *i;
    i = lst->cab->prox;
    while(i != lst->cab) {
        if( ( !strcmp(i->nome, flm->nome) ) && ( i->nota == flm->nota ) && (i->ano == flm->ano)) {
            return TRUE;
        }
        i = i->prox;
    }

    return FALSE;
}

/*----------------------------------------------------------------------
 *  removaFilme
 *
 *  Remove da lista de filmes LST o filme apontado por FLM.
 *    
 *  Pre-condicao: a funcao supoe que o filme FLM esta 
 *                na lista LST.
 */
void 
removaFilme(ListaFilmes *lst, Filme *flm)
{
    lst->nFilmes--;
    flm->ant->prox = flm->prox;
    flm->prox->ant = flm->ant;
    /* Libera Filme */
}


/*----------------------------------------------------------------------
 *  mergeSortFilmes
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo mergeSort recursivo adaptado para listas encadeadas
 *  duplamente ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 *
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */

void 
mergeSortFilmes(ListaFilmes *lst, Criterio crit, int p, int r)
{
    if (p < r-1) {
        int q = (p + r)/2;
        
        mergeSortFilmes(lst, crit, p, q);
        mergeSortFilmes(lst, crit, q, r);

        int i;
        Filme *j, *p_flm, *r_flm, *q_flm;
        j = lst->cab->prox;
        for(i = 1; i <= r; i++) {
            if(i == p)
                p_flm = j;
            if(i == q)
                q_flm = j;
            if(i == r)
                r_flm = j;
            j = j->prox;
        }
        intercalaFilmes(p_flm, q_flm, r_flm, lst, crit);
    }
}

/*----------------------------------------------------------------------
 *  quickSortFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo quickSort adaptado para listas encadeadas duplamente
 *  ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 * 
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
quickSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    AVISO(quickSortFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}

/*----------------------------------------------------------------------
 *  hashFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e distribui as palavras que
 *  ocorrem nos nomes do filmes em uma tabela de dispersao 
 *  (hash table):
 *
 *     http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 *
 *  Antes de inserir uma palavra na tabela de dispersao todas a
 *  letras da palavra devem ser convertidas para minusculo. Com
 *  isto faremos que a busca de filmes que possuam uma dada
 *  palavra em seu nome nao seja 'case insensitive'. Para essa
 *  tarefa a funcao tolower() pode ser utilizada.
 *
 *  Esta funcao utiliza as funcoes freeST(), initST() e putFilmeST()
 *
 *  Para obter as palavras podemos escrever uma funcao peguePalavra()
 *  inspirada na funcao pegueNome do modulo lexer.c do EP3/EP4 ou
 *  ainda utilizar a funcao strtok() da biblioteca string.h:
 *  
 *      http://linux.die.net/man/3/strtok    (man page)
 *
 */
void
hashFilmes(ListaFilmes *lst)
{
    AVISO(hashFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}

/* 
 * FUNCOES ADICIONADAS POR MIM! 
 * FUNCOES ADICIONADAS POR MIM! 
 * FUNCOES ADICIONADAS POR MIM! 
*/

/*----------------------------------------------------------------------
 *  insiraFilmePosicao
 *
 *  recebe uma lista, um filme flm e um filme posicao. Coloca
 *  filme flm na lista flm logo depois do filme posicao
 */
void insiraFilmePosicao(ListaFilmes *lst, Filme* flm, Filme* posicao) {
    lst->nFilmes++;
    flm->ant = posicao;
    flm->prox = posicao->prox;
    posicao->prox->ant = flm;
    posicao->prox = flm;
}

/*----------------------------------------------------------------------
 *  copieFilme
 *
 *  Recebe um filme e retorna uma copia desse filme
 */
Filme* copieFilme(Filme* flm) {
    return crieFilme(flm->dist, flm->votos, flm->nota, flm->nome, flm->ano);
}

/*----------------------------------------------------------------------
 *  crieMelhoresFilmes
 *
 *  Recebe uma lista de filmes, um numero de filmes, um float nota 
 *  e um numero minimo de votos. Retorna uma lista com os (numero) melhores
 *  filmes que tem a nota maxima e o pelo menos o numero minimo de votos.
 */
ListaFilmes *
crieListaMelhoresFilmes(ListaFilmes *lst, int numero, float nota, int votos) {
    Filme *i;
    int contador = 0;
    i = lst->cab;
    ListaFilmes* novaLista = crieListaFilmes();

    for(i = lst->cab->ant; contador != numero && i != lst->cab; i = i->ant) {
        if(i->nota <= nota && i->votos >= votos) {
            contador++;
            insiraFilme(novaLista, copieFilme(i));
        }
    }

    if(!contador)
        return NULL;
    return novaLista;
}

/*----------------------------------------------------------------------
 *  criePioresFilmes
 *
 *  Recebe uma lista de filmes, um numero de filmes, um float nota 
 *  e um numero minimo de votos. Retorna uma lista com os (numero) piores
 *  filmes que tem a nota minima e o pelo menos o numero minimo de votos.
 */
ListaFilmes *
crieListaPioresFilmes(ListaFilmes *lst, int numero, float nota, int votos) {
    Filme *i;
    int contador = 0;
    i = lst->cab;
    ListaFilmes* novaLista = crieListaFilmes();

    for(i = lst->cab->prox; contador != numero && i != lst->cab; i = i->prox) {
        if(i->nota >= nota && i->votos >= votos) {
            contador++;
            insiraFilme(novaLista, copieFilme(i));
        }
    }

    if(!contador)
        return NULL;
    return novaLista;
}

/*----------------------------------------------------------------------
 *  listaEstaOrdenada
 *
 *  Recebe uma lista de filmes. Retorna 1 se ela estiver ordenada por
 *  nota o 0 caso contrario.
 */
int
listaEstaOrdenada(ListaFilmes *lst) {
    Filme* i;
    for(i = lst->cab->prox->prox; i != lst->cab; i = i->prox) {
        if(i->nota < i->ant->nota)
            return 0;
    }
    return 1;
}

/*----------------------------------------------------------------------
 *  inverteListaFilmes
 *
 *  Recebe uma lista de filmes e inverte todos os filmes dela
 *  de posicao.
 */
void 
inverteListaFilmes(ListaFilmes *lst)
{   
    Filme *i;
    Filme *j;
    Filme *k;

    i = lst->cab->prox;
    j = lst->cab->ant;
    k = i->ant;

    while(i != j) {

        /*caso seja numero par de filmes*/
        if(i == j->ant) {
            removaFilme(lst, i);
            insiraFilmePosicao(lst, i, j);
            break;
        }
        else
        {
            removaFilme(lst, i);
            insiraFilmePosicao(lst, i, j);
            removaFilme(lst, j);
            insiraFilmePosicao(lst, j, k);

            k = j;
            j = i->ant;
            i = k->prox;
        }
    }
}

/*----------------------------------------------------------------------
 *  trocaFilmeDeLista
 *
 *  Recebe duas listas de filmes (origem e dest) e dois filmes(flm e posicao).
 *  Remove o filme flm da origem e coloca ele depois do filmes posicao no dest.
 */
void trocaFilmeDeLista(ListaFilmes* origem, ListaFilmes* dest, Filme *flm, Filme *posicao) {
    removaFilme(origem, flm);

    /* insere no inicio do destino */
    if(posicao == NULL) {
        insiraFilme(dest, flm);
    }
    /* insere depois do filme posicao */
    else {
        insiraFilmePosicao(dest, flm, posicao);
    }
}

/*----------------------------------------------------------------------
 *  comparaFilmes
 *
 *  Recebe dois filmes (a e b), e um criterio crit. Se crit for NOTA,
 *  retorna 1 se a nota de a for menos que a de b e 0 caso contrario.
 *  Se o crit for NOME, retorna 1 se tiver o nome de a tiver valor
 *  lexicografico menor que o nome de b e 0 caso contrario.
 */
int comparaFilmes(Filme* a, Filme* b, Criterio criterio) {
    if(criterio == NOTA) {
        if(a->nota <= b->nota)
            return 1;
        return 0;
    } else {
        if(strCmp(a->nome, b->nome) <= 0) 
            return 1;
        return 0;
    }
}

/*----------------------------------------------------------------------
 *  intercalaFilmes
 *
 *  Recebe os filmes p, q e r, a lista de filmes lst e o criteri crit. Faz
 *  a intercalacao da lista entre as posicoes p e r, com meio em q de acordo
 *  com o crit.
 */
void intercalaFilmes(Filme *p, Filme *q, Filme *r, ListaFilmes* lst, Criterio crit) {
    ListaFilmes *lst_aux = crieListaFilmes();
    Filme *i;
    Filme *j;
    Filme *k;
    int nFilmes = lst->nFilmes;
    
    /* tranfere todos os filmes ate p */
    i = p;
    while(i != q) {
        j = i->prox;
        trocaFilmeDeLista(lst, lst_aux, i, NULL);
        i = j;
    }

    i = r->ant;
    k = q->ant;
    while (i != k){
        j = i->ant;
        trocaFilmeDeLista(lst, lst_aux, i, NULL);
        i = j;
    }

    i = lst_aux->cab->prox;
    j = lst_aux->cab->ant;
    while(lst->nFilmes != nFilmes) {
        if(comparaFilmes(i, j, crit)) {
            k = i->prox;
            trocaFilmeDeLista(lst_aux, lst, i, r->ant);
            i = k;
        } else {
            k = j->ant;
            trocaFilmeDeLista(lst_aux, lst, j, r->ant);
            j = k;
        }
    }
    
    libereListaFilmes(lst_aux);
}
