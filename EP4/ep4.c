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

  ep4 - ep4.c
    
  Referências:

  - queue.c, queue.h. posicao.h baseados em:

        https://www.ime.usp.br/~cris/mac0122/programas/distancias/queue5/

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include<stdio.h>
#include<stdlib.h>

#include "posicao.h"
#include "queue.h"

#define CEMAX 1000
#define MMAX 1000
#define NMAX 1000

/* PREOCUPACAO COM O POSICAO DAS CENOURAS OU DO COELHO */

int le_arquivo(char nomearq[100], int *m, int *n, int *t, int matriz[MMAX][NMAX], posicao p[CEMAX]);

void copia_matriz_int(int m, int n, int origem[MMAX][NMAX], int destino[MMAX][NMAX]);

int mesma_posicao(posicao p1, posicao p2);

void calcula_caminho(int m, int n, int numCeno, posicao cenouras[CEMAX], int lab_original[MMAX][NMAX], posicao hebert);

void adiciona_posicoes(int m, int n, int linha, int coluna, int matriz[MMAX][NMAX], Queue *q);

void faz_caminho(int m, int n, int lab[MMAX][NMAX], int linha, int coluna, int contador);

int prepara_labirinto(int m, int n, int matriz[MMAX][NMAX], int num_ceno, posicao cenouras[CEMAX], posicao hebert);

void imprime_labirinto_caminho(int m, int n, int matriz[MMAX][NMAX]);

void imprime_labirinto(int m, int n, int labirinto[MMAX][NMAX]);

int main() {

    int n, m, opcao, lab = 0, numCeno;
    char nomearq[100];
    int labirinto[MMAX][NMAX];
    posicao cenouras[CEMAX], hebert;

    printf("*************************\n");
    printf("Carrotsvile search engine\n");
    printf("*************************\n\n");

    while(1) {
        printf("0: carregar um novo labirinto e posição inicial do Herbert\n");
        printf("1: dar nova posicao inicial do Herbert no mesmo labirinto\n");
        printf("2: sair do programa\n\n");

        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        printf("\n");

        if(opcao == 2)
            break;
        else if(opcao == 0) {

            lab = 1;
            printf("Digite o nome do arquivo com o labirinto: ");
            fflush(stdin);
            scanf("%s", nomearq);
            printf("\n");

            if(le_arquivo(nomearq, &m, &n, &numCeno, labirinto, cenouras)) {
                printf("\nERRO: Operacao abortada devido a erro na leitura ou dados inconsistentes\n\n");
                break;
            }
            printf("Labirinto: \n");
            imprime_labirinto(m, n, labirinto);
            printf("\n");

            printf("Digite a posicao inicial do Herbert: ");
            scanf("%d", &hebert.linha);
            scanf("%d", &hebert.coluna);

            calcula_caminho(m, n, numCeno, cenouras, labirinto, hebert);

        } else if(opcao == 1 && lab) {
            
            printf("Digite a posicao inicial do Herbert: ");
            scanf("%d", &hebert.linha);
            scanf("%d", &hebert.coluna);

            calcula_caminho(m, n, numCeno, cenouras, labirinto, hebert);

        } else {
            printf("Opcao invalida! Tente novamente.\n\n");
        }

    }

    printf("Tchau, Herbert!\n");

    return 0;
}

/*
 * Recebe o tamanho do labirinto, o labirinto original e
 * imprime ele da forma como esta.
 */
void imprime_labirinto(int m, int n, int labirinto[MMAX][NMAX]) {
    int i, j;

    for(i = 1; i <= m; i++) {
        for(j = 1; j <= n; j++) {
            printf("%d ", labirinto[i][j]);
        }
        printf("\n");
    }
}

/*
 * Recebe o nome de arquivo contendo as informacoes sobre o 
 * labirinto e determina os parametros m, n, a matriz
 * que do labirinto, o numero e as posicoes das cenouras em 
 * um vetor de posicoes.
 */
int le_arquivo(char nomearq[100], int *m, int *n, int *t, int matriz[MMAX][NMAX], posicao p[CEMAX]) {
    FILE *fp;
    int i, j;

    if (!(fp = fopen(nomearq, "r"))) {
        printf("Erro ao abrir o arquivo %s\n", nomearq);
        return 1;
    }
    
    fscanf(fp, "%d", m);
    fscanf(fp, "%d", n);

    for(i = 1; i <= *m; i++) {
        for(j = 1; j <= *n; j++) {
            fscanf(fp, "%d", &matriz[i][j]);
        }
    }

    fscanf(fp, "%d", t);

    for(i = 0; i < *t; i++) {
        fscanf(fp, "%d", &p[i].linha);
        fscanf(fp, "%d", &p[i].coluna);
    }

    fclose(fp);

    return 0;
}

/*
 * Recebe as dimensoes das matrizes, a matriz de origem e 
 * copia seu conteudo para a matriz de destino
 */
void copia_matriz_int(int m, int n, int origem[MMAX][NMAX], int destino[MMAX][NMAX]) {
    int i, j;
    for(i = 1; i <= m; i++)
        for(j = 1; j <= n; j++)
            destino[i][j] = origem[i][j]; 
}

/*
 * Recebe o tamanho do labirinto, o labirinto original, as posicoes
 * das cenouras, o numero de cenouras e a posicao do herbert e calcula
 * o menor caminho ate a cenoura.
 */
void calcula_caminho(int m, int n, int numCeno, posicao cenouras[CEMAX], int lab_original[MMAX][NMAX], posicao hebert) {
    int lab_aux[MMAX][NMAX], i, result = -1;
    posicao posi_aux;
    Queue q = queueInit(MMAX);
    Queue p = queueInit(MMAX);

    /* faz uma copia do labirinto do arquivo para um labirinto auxiliar */
    copia_matriz_int(m, n, lab_original, lab_aux);
    
    /* prepara o labirinto para ser usado nas funcoes seguintes.
       Verifica se a cenoura esta na mesma casa que hebert */
    if(prepara_labirinto(m, n, lab_aux, numCeno, cenouras, hebert)) {

        /* inicia a fila com a posicao de Herbert */
        queuePut(q, hebert);

        for (i = 1; !queueEmpty(q); i++) {

            /* coloca todos os elementos de q em p */
            while(!queueEmpty(q)) {
                queuePut(p, queueGet(q));
            }
            while(!queueEmpty(p)) {
                posi_aux = queueGet(p);

                /* verifica se Hebert esta na posicao analisada */
                if( lab_aux[posi_aux.linha][posi_aux.coluna] != -3 ) {
                    /* coloca a distancia ate Hebert na posicao */
                    lab_aux[posi_aux.linha][posi_aux.coluna] = i;
                    /* adicona as posicoes adjacentes a posicao analisada em q */
                    adiciona_posicoes(m, n, posi_aux.linha, posi_aux.coluna, lab_aux, &q);
                } else {
                    queueFree(q);
                    queueFree(p);
                    faz_caminho(m, n, lab_aux, posi_aux.linha, posi_aux.coluna, i - 1);
                    result = i - 1;
                    break;
                }
            }
            if(result != -1)
                break;
        }

        if(result == -1) {
            printf("\nO Herbert não achou nenhuma cenoura!!! :(\n\n");

        } else {
            printf("\nO Herbert achou uma cenoura em %d passos!\n\n", result);
        }

    } else {
        printf("\nO Herbert ja esta em cima de uma cenoura!\n\n");
    }

    imprime_labirinto_caminho(m, n, lab_aux);
}

/*
 * Recebe o tamanho do labirinto, o labirinto, o numero de cenouras, as posicoes
 * delas e a posicao do hebert. Faz uma conversao da matriz do arquivo
 * para uma forma mais facil de lidar para as demais funcoes. Segue o 
 * seguinte esquema:
 * 
 * 1: herbert 
 * -3: cenoura
 * -2: caminho ate a cenoura (sera feito na funcao faz_caminho)
 * -1: barreira
 * natural sem 1: numero de casas ate o labirinto (sao adicionados em calcula_caminho)
 */
int prepara_labirinto(int m, int n, int matriz[MMAX][NMAX], int num_ceno, posicao cenouras[CEMAX], posicao hebert) {
    int i, j, result = 1;

    /* troca 1 por -1 (barreiras) */
    for(i = 1; i <= m; i++) {
        for(j = 1; j <= n; j++) {
            if(matriz[i][j] == 1)
                matriz[i][j] = -1;
        } 
    }

    /* Verifica se alguma cenoura esta na mesma posicao que hebert
       colocas as posicoes onde tem cenouras como -3*/
    for(int i = 0; i < num_ceno; i++) {
        if(cenouras[i].linha == hebert.linha && cenouras[i].coluna == hebert.coluna) {
            result = 0;
        } else {
            matriz[cenouras[i].linha][cenouras[i].coluna] = -3;
        }
    }

    /* coloca a posicao de Hebert como 1 */
    matriz[hebert.linha][hebert.coluna] = 1;

    return result;
}

/*
 * Recebe o tamanho do labirinto, uma posicao a ser analisada,
 * e uma fila. Adiciona a fila todas as possiveis posicoes que
 * podem ser acessadas pela posicao atual (cima, baixo
 * direita, esquerda).
 */
void adiciona_posicoes(int m, int n, int linha, int coluna, int matriz[MMAX][NMAX], Queue *q) {
    posicao nova_posicao;

    if(linha + 1 <= m) {
        if( matriz[linha + 1][coluna] == 0 ||  matriz[linha + 1][coluna] == -3) {
            nova_posicao.linha = linha + 1;
            nova_posicao.coluna = coluna;
            queuePut(*q, nova_posicao);
        }
    }
    if(linha - 1 > 0) {
        if( matriz[linha - 1][coluna] == 0 || matriz[linha - 1][coluna] == -3) {
            nova_posicao.linha = linha - 1;
            nova_posicao.coluna = coluna;
            queuePut(*q, nova_posicao);
        }
    }
    if(coluna + 1 <= n) {
        if( matriz[linha][coluna + 1] == 0 || matriz[linha][coluna + 1] == -3) {
            nova_posicao.linha = linha;
            nova_posicao.coluna = coluna + 1;
            queuePut(*q, nova_posicao);
        }
    }
    if(coluna - 1 > 0) {
        if( matriz[linha][coluna - 1] == 0 || matriz[linha][coluna - 1] == -3) {
            nova_posicao.linha = linha;
            nova_posicao.coluna = coluna - 1;
            queuePut(*q, nova_posicao);
        }
    }
}

/*
 * Recebe o tamanho do labirinto, o labirinto, uma posicao a ser analisada e um 
 * contador de passos. Escreve o caminho dessa posicao (com -2's) ate a posicao do hebert (1).
 */
void faz_caminho(int m, int n, int matriz[MMAX][NMAX], int linha, int coluna, int contador) {

    if(contador > 1) {
        if(linha + 1 <= m) {
            if( matriz[linha + 1][coluna] == contador) {
                matriz[linha + 1][coluna] = -2;
                faz_caminho(m, n, matriz, linha + 1, coluna, contador - 1);
            }
        }
        if(linha - 1 > 0) {
            if(matriz[linha - 1][coluna] == contador) {
                matriz[linha - 1][coluna] = -2;
                faz_caminho(m, n, matriz, linha - 1, coluna, contador - 1);
            }
        }
        if(coluna + 1 <= n) {
            if(matriz[linha][coluna + 1] == contador) {
                matriz[linha][coluna + 1] = -2;
                faz_caminho(m, n, matriz, linha, coluna + 1, contador - 1);
            }
        }
        if(coluna - 1 > 0) {
            if(matriz[linha][coluna - 1] == contador) {
                matriz[linha][coluna - 1] = -2;
                faz_caminho(m, n, matriz, linha, coluna - 1, contador - 1);
            }
        }
    }
}

/*
 * Recebe o labirinto e seu tamanho e o imprime de acordo com as 
 * relacoes:
 * 
 *  1: Herbert
 * -3: cenoura
 * -2: caminho ate a cenoura
 * -1: barreira
 * natural sem 1: vazio
 */
void imprime_labirinto_caminho(int m, int n, int matriz[MMAX][NMAX]) {
    int i, j;
    char c;

    for(i = 1; i <= m; i++) {
        for(j = 1; j <= n; j++) {
            switch (matriz[i][j]) {
                case 1:
                    c = 'H';
                    break;
                case -3:
                    c = 'C';
                    break;
                case -2:
                    c = '*';
                    break;
                case -1:
                    c = '1';
                    break;
                default:
                    c = '0';
                    break;
            }
            printf("%c ", c);
        } 
        printf("\n");
    }

    printf("\n");
}