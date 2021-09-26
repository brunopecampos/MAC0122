# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /home/bruno/Documentos/2º Semestre Eng Comp/Princ. de Desenv. de Algo./EP3 
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep3

ep3: ep3.o polinomios.o stack.o
	gcc -o ep3 ep3.o polinomios.o stack.o

ep3.o: ep3.c polinomios.h stack.h item.h
	gcc $(CFLAGS) ep3.c -c 

polinomios.o: polinomios.c polinomios.h
	gcc $(CFLAGS) polinomios.c -c

stack.o: stack.c stack.h item.h
	gcc $(CFLAGS) stack.c -c

clean:
	rm -rf *.o *~ client
