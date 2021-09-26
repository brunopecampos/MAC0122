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

  IMDB: main.c 


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*        OBERSERVACOES
 -SO FORAM FEITAS AS FUNCOES NAO OPICIONAIS
 -TODAS AS FUNCOES ADICIONADAS POR MIM ESTAO NO FINAL DOS ARQUIVOS .C REFERENTES
 -HA WARNINGS NO ARQUIVO "st.c" POREM EU NAO MEXI NELE, ENTAO ASSUMI QUE PODEM
  SER DESCONSIDERADOS
*/

#include <stdio.h>  /* printf(), scanf() */ 
#include <time.h>   /* CLOCKS_PER_SECOND */

#include "main.h"

#include "filmes.h"   /* crieListaFilmes(), crieFilme(), 
                         inseraFilme(),  mergeSortFilmes() */
#include "iofilmes.h" /* carregueListaFilmes(), graveListaFilmes(), 
                         mostreFilme(), mostreListaFilmes() */
#include "util.h"     /* mallocSafe(), leiaString() */
#include "st.h"       /* initST(), putFilmeST(), getFilmeST(), showST(),
                         freeST() */

/*------------------------------------------------------------------- 
  P R O T O T I P O S 
*/

/* A funcao leiaOpcao so e usada neste arquivo. */
static char  leiaOpcao(); 


/*------------------------------------------------------------------- 
  M A I N 
*/
int 
main(int argc, char *argv[])
{
    ListaFilmes *lst = crieListaFilmes(); 
    char         opcao;
    clock_t start, end; /* usadas para medir tempo de processamento */
    double elapsed;
    char palavra[TAM_STR];
    /* declaracao das demais variaveis do main */

    /*------------------------------------------------------------*/
    /* imprima o cabecalho */
    printf("MAC0122 2020 - EP5\n");
    printf("The Internet Movie Database (%s, %s)\n", __DATE__, __TIME__);
    printf("[GCC %s] on %s\n", __VERSION__, SYSTEM);

    /*-----------------------------------------------------*/
    /* iterara ate o usuario digitar 'x' para sair         */
    do 
    { 

        opcao = leiaOpcao();

        /* comeca a cronometrar */
        start = clock(); 

        switch (opcao) 
        {
            /*---------------------------------------------*/
        case CARREGAR_SER:
        {
            carregueListaFilmes(lst,FALSE);
            break;
        }  

        /*---------------------------------------------*/
        case CARREGAR_ER:
        {
            carregueListaFilmes(lst,TRUE);
            break;
        }  

        /*---------------------------------------------*/
        case GRAVAR:
        {
            graveListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case PROCURAR:
        {
            procuraFilme(lst, palavra);
            break;
        }

        /*---------------------------------------------*/
        case HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case PROCURAR_HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_HASH: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case LIMPAR_HASH: /* opcional */
        {
            break;
        }

        /* To-do terminar esse trem */
        /*---------------------------------------------*/
        case INSERIR:
        {
            insereFilme(lst);
            break;
        }

        /*---------------------------------------------*/
        case REMOVER:
        {
            removeFilme(lst, palavra);
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOTA_M:
        {
            ordeneMergeSortFilmes(lst, NOTA);
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOME_M:
        {
            ordeneMergeSortFilmes(lst, NOME);
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOTA_Q: /* opcional */
        {

            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOME_Q: /* opcional */
        {
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR:
        {
            mostreListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_MENOR:
        {
            mostreMelhoresFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_MAIOR:
        {
            mostrePioresFilmes(lst);
            break;
        }
      
        /*---------------------------------------------*/
        case LIMPAR:
        {
            limpeListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case SAIR:
        {
            libereListaFilmes(lst);
            break;
        }

        default :
        {
            printf("main: opcao '%c' nao reconhecida\n", opcao);
            break;
        }
        
        } /* switch */
        
        /* trava o cronometro */  
        end = clock();
        
        /* calcula o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\n(%g segundos)\n", elapsed);
    } 
    while (opcao != SAIR);


    return 0;
}


/*---------------------------------------------------------------------
  leiaOpcao()
   
  Le e retorna o caractere correspondente a opcao do usuario.
*/

char 
leiaOpcao()
{
    char opcao;
    char listaOpcoes1[] = 
        "\n================================================"
        "======================\n"
        "  (c) carregar um arquivo de dados sem eliminar repeticoes\n"
        "  (C) carregar um arquivo de dados eliminando repeticoes\n"
        "  (g) gravar   a lista atual em um arquivo\n"
        "  (p) procurar a nota de um filme\n"
        "  (h) criar    a  TS com as palavras em nomes de filmes (opcional)\n"
        "  (P) procurar na TS a nota de um filme (opcional)\n"
        "  (M) mostrar  a  TS (opcional)\n"
        "  (L) limpar   a  TS (opcional)\n"
        "  (i) inserir  um filme\n"
        "  (r) remover  um filme\n";
    char listaOpcoes2[] = 
        "  (o) ordenar  a lista de filmes por nota (mergeSort)\n"
        "  (O) ordenar  a lista de filmes por nome (mergeSort)\n"
        "  (q) ordenar  a lista de filmes por nota (quickSort, opcional)\n"
        "  (Q) ordenar  a lista de filmes por nome (quickSort, opcional)\n"
        "  (m) mostrar  todos os filmes\n"
        "  (<) mostrar  N filmes com nota _menor_ que X e pelo menos V votos\n"
        "  (>) mostrar  N filmes com nota _maior_ que X e pelo menos V votos\n"
        "  (l) limpar   a lista de filmes\n"
        "  (x) sair     do programa\n\n";

    printf("%s",listaOpcoes1);
    printf("%s",listaOpcoes2);
    printf("Digite uma opcao: ");
    scanf(" %c", &opcao);

    /* main.c: warning: string length ‘679’ is greater than the length 
       ‘509’ ISO C90 compilers are required to support 
       [-Woverlength-strings] */

    return opcao;
}


