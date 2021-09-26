#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

enum criterio
{
    NOTA,
    NOME
};
typedef enum criterio Criterio;

typedef struct celFilme Filme;
struct celFilme 
{
    int valor;
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

void * 
mallocSafe(size_t n)
{
    void * p;

    p = malloc(n);
    if (p == NULL) 
    {
        printf("ERRO na alocacao de memoria.\n\n");
        exit(EXIT_FAILURE);
    }

    return p;
}


void mostreFilme(Filme *flm)
{
    if (flm == NULL) 
    {
        printf("NULL recebido como parametro");
    }
    printf("[%d] ",
           flm->valor);

}

void 
mostreListaFilmes(ListaFilmes *lst)
{
    Filme* aux = lst->cab->prox;
    while(aux != lst->cab) {
        mostreFilme(aux);
        aux = aux->prox;
    }
    printf("\n\n");
}

Filme *
crieFilme (int valor)
{
    Filme *flm;
    
    flm = mallocSafe(sizeof *flm);
    
    flm->valor = valor;
    
    return flm;
}

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

void 
libereFilme(Filme *flm)
{
    free(flm);
}

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

void 
insiraFilme(ListaFilmes *lst, Filme *flm)
{
    lst->nFilmes++;
    flm->ant = lst->cab->ant;
    flm->prox = lst->cab;
    lst->cab->ant->prox = flm;
    lst->cab->ant = flm;
}

void insiraFilmePosicao(ListaFilmes *lst, Filme* flm, Filme* posicao) {
    lst->nFilmes++;
    flm->ant = posicao;
    flm->prox = posicao->prox;
    posicao->prox->ant = flm;
    posicao->prox = flm;
}

void 
removaFilme(ListaFilmes *lst, Filme *flm)
{
    lst->nFilmes--;
    flm->ant->prox = flm->prox;
    flm->prox->ant = flm->ant;
    /* Libera Filme */
}

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

int comparaFilmes(Filme* a, Filme* b, int opcao, Criterio criterio) {
    if(opcao) {
        if(a->valor <= b->valor)
            return 1;
        else 
            return 0;
    } else {
        /* compara nomes de filmes TODO */
        return 0;
    }
}

void intercalaFilmes(Filme *p, Filme *q, Filme *r, ListaFilmes* lst, Criterio criterio) {
    ListaFilmes *lst_aux = crieListaFilmes();
    Filme *ini = p->ant;
    Filme *fim = r;
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
        if(comparaFilmes(i, j, 1, criterio)) {
            k = i->prox;
            trocaFilmeDeLista(lst_aux, lst, i, fim->ant);
            i = k;
        } else {
            k = j->ant;
            trocaFilmeDeLista(lst_aux, lst, j, fim->ant);
            j = k;
        }
    }
    
    libereListaFilmes(lst_aux);
}


void 
mergeSortFilmes(ListaFilmes *lst, Criterio criterio, int p, int r)
{
    if (p < r-1) {
        int q = (p + r)/2;
        
        mergeSortFilmes(lst, criterio, p, q);
        mergeSortFilmes(lst, criterio, q, r);

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
        intercalaFilmes(p_flm, q_flm, r_flm, lst, criterio);
    }
}


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

int 
leiaString(char str[], int size)
{
    int len;
    char s[256];

    /* remove qualquer newline que estava no buffer */
    scanf(" ");

    /* leitura do string: ler info sobre fgets() */
    fgets(s, 256, stdin);

    /* sobreescreve  um possivel newline do final com '\0' */
    len = strlen(s); 
    if (s[len-1] == '\n') 
    {
        len--;
        s[len] = '\0';
    }

    if (len > size)
    {
        s[size-1] = '\0';
        len = size-1;
    }
    strncpy(str, s, size);

#ifdef DEBUG
    printf("AVISO: leiaString: string lido '%s' tem %d caracteres\n", s, len);
#endif
    return len;
}

char *
toLowerCase(char str[]) {
    int i;
    char *newStr = mallocSafe(strlen(str) * sizeof(char));
    for(i = 0; i < strlen(str); i++) {
        *(newStr + i)  = tolower(str[i]); 
    }

    return newStr;
}

int main () {
    /*ListaFilmes *list = crieListaFilmes();
    int vector[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    int i;
    for(i = 0; i < 12; i++) 
        insiraFilme(list, crieFilme(vector[i]));
    mostreListaFilmes(list);
    inverteListaFilmes(list);
    mostreListaFilmes(list);*/
    char *a;
    leiaString(a, 256);
    char *b = toLowerCase(a);
    printf("%s\n", b);

}