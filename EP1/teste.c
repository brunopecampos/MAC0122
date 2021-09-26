#include<stdio.h>
#include<stdlib.h>

FILE *abreArquivo(char arq[50]) {
    FILE *fp;

    if (!(fp = fopen(arq, "w"))) {
        printf("Erro ao abrir/criar o arquivo %s\n", arq);
        exit(1);
    }

    return fp;
}
int main() {
    FILE *fp = abreArquivo("mine.ps");
    fprintf(fp, "minecraft");
    fclose(fp);

    return 0;
}