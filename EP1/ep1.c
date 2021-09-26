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

  ep1.c

  Referências:

  - Lista dinamica de argumentos baseada em:

        https://www.cprogramming.com/tutorial/c/lesson17.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define FNMAX 200
#define FUNCMAX 50
#define GRATIO 1.618033988749894848
#define TWOSQUARED 1.414213562373095048

/* FUNCOES DE UTILIDADE */

/* escreve a string funcao e os seus parametros em um arquivo fp.
o formato de escrita eh o do postscript. utiliza a biblioteca
atdarg para criar uma lista dinamica de argumentos. */
void escreveFuncao(FILE *fp, char funcao[FUNCMAX], int num, ... ) {

    va_list args;

    int i;

    /* inicia a lista */
    va_start(args, num);

    /* acessa os arguemtos e os escreve em seguida */
    for (i = 0; i < num; i++) {
        fprintf(fp, "%.2f", va_arg(args, double));
        fprintf(fp, " ");
    }
        
    /* libera a memoria da lista */
    va_end(args);
    
    fprintf(fp, "%s", funcao);
    fprintf(fp, " ");
}

/* funcao da que escreve uma Curva de Koch de distancia Dist
e ordem "ordem" em uma arquivo fp*/
void estrelaDeKoch(double Dist, int ordem, FILE *fp) {
    
    /* se a ordem for diferente de 0 ele chamas as Curvas
       de Koch recursivamente, se não ele imprime um segmento */
    if(ordem) {
        estrelaDeKoch(Dist / 3, ordem - 1, fp);
        escreveFuncao(fp, "rotate", 1, -60.0);
        estrelaDeKoch(Dist / 3, ordem - 1, fp);
        escreveFuncao(fp, "rotate", 1, 120.0);
        estrelaDeKoch(Dist / 3, ordem - 1, fp);
        escreveFuncao(fp, "rotate", 1, -60.0);
        estrelaDeKoch(Dist / 3, ordem - 1, fp);
    } else {
        escreveFuncao(fp, "rlineto", 2, Dist, 0.0);
    }
    
}

/* funcao que escreve a arvore H
de tamanho Dist no arquivo fp */
void arvoreH(double dist, int ordem, FILE *fp) {

    /* move para os "vertices" do H para chamar a funcao recursivamente.
    em 2 desses movimentos as linhas verticais do H sao escritas */
    escreveFuncao(fp, "rmoveto", 2, dist / 2, -(dist / 2));
    if(ordem > 0) 
        arvoreH(dist/2, ordem - 1, fp);

    escreveFuncao(fp, "rlineto", 2, 0.0, dist);
    if(ordem > 0) 
        arvoreH(dist/2, ordem - 1, fp);

    escreveFuncao(fp, "rmoveto", 2, -dist, 0.0);
    if(ordem > 0) 
        arvoreH(dist/2, ordem - 1, fp);

    escreveFuncao(fp, "rlineto", 2 , 0.0, -dist);
    if(ordem > 0) 
        arvoreH(dist/2, ordem - 1, fp);

    /* escreve a linha horizontal do H e traz o referencial para o centro
    da figura novamente */
    escreveFuncao(fp, "rmoveto", 2, 0.0, dist / 2);
    escreveFuncao(fp, "rlineto", 2, dist, 0.0);
    escreveFuncao(fp, "rmoveto", 2, -(dist / 2), 0.0);
}

/* OBS: o meu fractal consiste em uma recursao de quadrados
coloridos de preto inscritos em circinferencias vazadas de
forma que novos que essa forma se repete em cada novo
vertice nao ocupado da forma anterior. Alem disso, em
cada chamada recursiva o lado do quadado eh dividido 
pelo numero de ouro(phi) */

/* funcao que escreve o meu fractal de lado "lado",
ordem "ordem", coordenadas xO e yO e posicao "posicao"
no arquivo fp */
void meuFractal(double lado, int ordem, FILE *fp, double xO, double yO, int posicao) {

    /* move para a posicao inicial da figura */
    escreveFuncao(fp, "moveto", 2, xO, yO);

    /* calcula o lado da nova chamada recursiva utilizando
    a constante do numero de outro definida como macro */
    double novoLado = lado / GRATIO;

    if(ordem > 0) {
        
        /*analisa qual a posicao serao chamadas as proximas recursoes */
        switch (posicao)
        {
            /* exemplo: se estiver na posicao 1 (superior direita, conta em sentido 
            horario), chama dos as posicoes superior direita(1), inferior direita(2),
            e superior esquerda(4). */
            case 1:
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 1);
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 2);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 4);
                break;
            case 2:
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 1);
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 2);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 3);
                break;
            case 3:
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 2);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 3);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 4);
                break;
            case 4:
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 1);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 3);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 4);
                break;
            break;

            /* no caso da posicao inicial, chama todas as posicoes (1, 2, 3, 4) 
            recursivamente, pois todas estao disponiveis */
            default:
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 1);
                meuFractal(novoLado, ordem - 1, fp, ((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 2);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, -((lado + novoLado) / 2) + yO, 3);
                meuFractal(novoLado, ordem - 1, fp, -((lado + novoLado) / 2) + xO, ((lado + novoLado) / 2) + yO, 4);
            break;
        }
    }

    /* escreve as linhas do quadrado e o colore */
    escreveFuncao(fp, "moveto", 2, xO + lado /2, yO + lado /2);
    escreveFuncao(fp, "rlineto", 2, 0.0, -lado);
    escreveFuncao(fp, "rlineto", 2, -lado, 0.0);
    escreveFuncao(fp, "rlineto", 2, 0.0, lado);
    escreveFuncao(fp, "rlineto", 2, lado, 0.0);
    escreveFuncao(fp, "fill", 0);
    
    /* escreve a circunferencia circunscrita no 
    quadrado e desenha a figura */
    escreveFuncao(fp, "arc", 5, xO, yO, (lado * TWOSQUARED) / 2.0, 0.0, 360.0);
    escreveFuncao(fp, "stroke", 0);
}

/* FUNCAO MAIN */

int main() {
    
    int desenho, ordem;
    char arq[FNMAX];
    
    printf(" 1- Estrela de Koch\n 2- Arvore H\n 3- Bruno's Fractal\n");
    printf("Escolha um desenho: ");
    scanf("%d", &desenho);
    printf("Ordem do desenho: ");
    scanf("%d", &ordem);
    printf("Nome do Arquivo de saida: ");
    scanf("%s", arq);

    FILE *fp;
    if (!(fp = fopen(arq, "w"))) {
        printf("Erro ao abrir/criar o arquivo %s\n", arq);
        exit(1);
    }

    switch (desenho)
    {
        case 1:
             /* move para o vertice inicial de onde sairao as 3 curvas
            de Kohn  */
            escreveFuncao(fp, "moveto", 2, 50.0, 250.0);

            /* chama as funcoes recursivas das 3 curvas de Koch de tamnho 500
            rotacionando a caneta em 60 graus entre as chamadas */
            estrelaDeKoch(500.0, ordem, fp);
            escreveFuncao(fp, "rotate", 1, 120.0);
            estrelaDeKoch(500.0, ordem, fp);
            escreveFuncao(fp, "rotate", 1, 120.0);
            estrelaDeKoch(500.0, ordem, fp);

            /* escreve o desenho */
            escreveFuncao(fp, "stroke", 0);
            break;
        case 2:
            /* move o referencial para o centro da folha */
            escreveFuncao(fp, "moveto", 2, 297.5, 421.0);

            /* chama a funcao recursiva com tamanho 200 */
            arvoreH(200.0, ordem, fp);

            /* escreve o desenho */
            escreveFuncao(fp, "stroke", 0);

            break;
        case 3:
            meuFractal(120, ordem, fp, 297.5, 421, 0);
            break;
        default:
            printf("Operacao nao valida. Execute Novamente.");
            break;
    }

    fclose(fp);

    return 0;
}