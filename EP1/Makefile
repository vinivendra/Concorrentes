##############################################
#  EP1 - MAC0438 - Programacao Concorrente   #
# Alunos:									 #
#	Helena Almeida Victoretti - 7991124		 #
#	Vinicius Vendramini - 799 				 #
##############################################


ep1: ep1.o pista.o thread.o
	gcc -pthread ep1.o pista.o thread.o -o ep1

ep1.o: ep1.c pista.h thread.h ep1.h
	gcc -Wall -pthread -c ep1.c 

thread.o: thread.c thread.h pista.h
	gcc -Wall -pthread -c thread.c

pista.o: pista.c pista.h
	gcc -Wall -c pista.c


.PHONY: clean
clean:
	rm -f *.o