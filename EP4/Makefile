# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /home/bruno/Documentos/2º Semestre Eng Comp/Princ. de Desenv. de Algo./EP4 
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep4

ep4: ep4.o queue.o
	gcc -o ep4 ep4.o queue.o

ep4.o: ep4.c queue.h posicao.h
	 gcc $(CFLAGS) ep4.c -c

queue.o: queue.c queue.h posicao.h
	gcc $(CFLAGS) queue.c -c

clean:
	rm -rf *.o *~ client