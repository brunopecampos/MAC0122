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

  ep3 - ep3.c
    
  Referências:

  - Funcao infixa_para_posfixa(), stack.c, stack.h. item.h baseados em:

        https://www.ime.usp.br/~cris/mac0122/programas/polonesa/stack2/

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"polinomios.h"
#include"stack.h"
#include"item.h"

void inicializa_pols(polinomio p[26]);

/* Perguntar no fórum */
void descarta_resto_da_linha();

void executa_comando(char pol, char ope, polinomio p[26]);

polinomio calcula_expressao (char* expr, polinomio p[26], int len);

polinomio acha_parametros(char* expr, polinomio p[26], int len);

void infixa_para_posfixa (char infixa[100], char posfixa[100]);




int main() {
    
    polinomio var[26];
    char pol;
    char ope;

    inicializa_pols(var);

    while(1) {

        printf("Digite uma expressão ou quit para sair do programa:\n");
        printf("> ");
        scanf("%c", &pol);
        scanf("%c", &ope);

        if(pol == 'q' && ope == 'u') {
            descarta_resto_da_linha();
            break;
        } 
        executa_comando(pol, ope, var);
    }

    printf("Tchau!\n");
    return 0;
}

/*
 * Recebe um vetor de polinomios com 26 posicoes
 * e inicializa cada uma delas com um pol. nulo
 */
void inicializa_pols(polinomio p[26]) {
    int i;
    for(i = 0; i < 26; i++) p[i] = cria();
}

/*
 * Le o final da string digitada ate chegar no 
 * char '\n'
 */
void descarta_resto_da_linha() {
  char c;
  
  do {
    scanf("%c", &c); 
  } while (c != '\n' && c != EOF);
}

/*
 * Recebe o vetor de polinomios (p), um char que
 * indica um dos polinomios de p e o char ope que 
 * diz qual operacao sera feita
 */
void executa_comando(char pol, char ope, polinomio p[26]) {
    char c;
    char infixa[100];
    char posfixa[100];
    polinomio aux;

    switch (ope)
    {
        case ':':
            scanf("%c", &c);
            libera(p[(int)(pol) - 97]);
            p[(int)(pol) - 97] = leia();

        case '?':
            impr(pol, p[(int)(pol) - 97]);
            descarta_resto_da_linha();
            break;
        
        case '=':
            scanf("%s", infixa);
            descarta_resto_da_linha();
            infixa_para_posfixa(infixa, posfixa);
            aux = calcula_expressao(posfixa, p, strlen(posfixa) - 1);
            libera(p[(int)(pol) - 97]);
            p[(int)(pol) - 97] = aux;
            impr(pol, p[(int)(pol) - 97]);
            break;

        default:
            printf("Erro de formato na expressão.\n");
            descarta_resto_da_linha();
            break;
    }
}

/*
 * Recebe uma expressao em notacao polonesa expr, o vetor
 * de polinomios p um numero len, que significa o quanto da 
 * expressao ainda nao foi processado pelo programa.
 */
polinomio calcula_expressao (char expr[100], polinomio p[26], int len) {


    if(len < 0) {
        printf("Erro de formato na expressão.\n");
        exit(1);
    }
    
    /* paramentros usados nas operacoes. Eles sao procurados
       por meio de acha_parametros() */
    polinomio param1;
    polinomio param2;

    switch ( *(expr + len) ){
        case '~':
            /* deixa os caracteres onde ja possou na string
               marcados por '#'*/
            *(expr + len) = '#';
            param1 = acha_parametros(expr, p, len - 1);
            return nega(param1);
            break;

        case '+':
            *(expr + len) = '#';
            param1 = acha_parametros(expr, p, len - 1);
            param2 = acha_parametros(expr, p, len -1);
            return soma(param2, param1);
            break;
        
        case '-':
            *(expr + len) = '#';
            param1 = acha_parametros(expr, p, len - 1);
            param2 = acha_parametros(expr, p, len -1);
            return subt(param2, param1);
            break;
        
        case '*':
            *(expr + len) = '#';
            param1 = acha_parametros(expr, p, len - 1);
            param2 = acha_parametros(expr, p, len -1);
            return mult(param2, param1);
            break;

        case '/':
            *(expr + len) = '#';
            param1 = acha_parametros(expr, p, len - 1);
            param2 = acha_parametros(expr, p, len -1);
            return quoc(param2, param1);
            break;

        case '%':
            *(expr + len) = '#';
            param1 = acha_parametros(expr, p, len - 1);
            param2 = acha_parametros(expr, p, len -1);
            return rest(param2, param1);
            break;
        
        default:
            /* caso "a=b" */
            if( (int)(*(expr + len)) >= 97 && (int)(*(expr + len)) <= 122 && len == 0 ) {
                printf("%c", *(expr + len));
                return copia(p[(int)(*(expr + len)) - 97]);
            }
            break;
    }
    
    printf("Erro de formato na expressão.\n");
    exit(1);
    return NULL;
}

/*
 * Recebe uma expressao em notacao polonesa expr, o vetor
 * de polinomios p um numero len, que significa o quanto da 
 * expressao ainda nao foi processado pelo programa.
 * 
 * Essa funcao acha os parametros para uma operacao. Caso ache
 * outra operacao antes, chama calcula_expressao recurviamente.
 */
polinomio acha_parametros(char expr[100], polinomio p[26], int len) {

    if(len < 0) {
        printf("Erro de formato na expressão.\n");
        exit(1);
    }

    char letra;
    int i = 0;

    /* pula os caracteres que ja foram usados */
    while( *(expr + len - i) == '#' )
        i++;

    letra = *(expr + len - i);

    /* verifica se o caracter eh uma expressao */
    if(letra == '+' || letra == '-' || letra == '*' || letra == '/' 
    || letra == '%' || letra == '~') {
        return calcula_expressao(expr, p, len - i);
    }

    
    *(expr + len - i) = '#';
    printf("expressao: %s\n", expr);
    return p[(int)(letra) - 97];
}

/*
 * Recebe uma string infixa que representa uma
 * uma expressao matematica convencional e 
 * retorna uma string dessa mesma expressao
 * em notacao polonesa posfixa 
 */
void infixa_para_posfixa (char infixa[100], char posfixa[100]) {
    int n = strlen(infixa);
    int i;
    int j = 0;
    char item;
    stackInit(n);

    for(i = 0; i < n; i++) {

        switch (infixa[i]) {
            case '(': 
                stackPush(infixa[i]);
                break;

            case ')':
                while ((item = stackPop()) != '(')  
                    posfixa[j++] = item;
            break;

            case '~':
                while (!stackEmpty() && (item = stackTop()) != '(' 
                    && item != '+' && item != '-' && item != '*'
                    && item != '/' && item != '%') 
                    posfixa[j++] = stackPop();
                stackPush(infixa[i]); 
                break;
            
            case '*':
            case '/': 
            case '%':
                while (!stackEmpty() && (item = stackTop()) != '(' 
                    && item != '+' && item != '-') 
                    posfixa[j++] = stackPop();
                stackPush(infixa[i]); 
                break;
            
            case '+': 
            case '-': 
                while (!stackEmpty() && (item = stackTop()) != '(')
                    posfixa[j++] = stackPop(); 
                stackPush(infixa[i]);
                break;

            default:  
                if (infixa[i] != ' ') 
                    posfixa[j++] = infixa[i];
        }
    }

    while (!stackEmpty()) 
        posfixa[j++] = stackPop();     
    
    posfixa[j] = '\0'; 

    stackFree();
    
}