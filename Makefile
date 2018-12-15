args = -std=c99 -J3 -O3 
all:
#	rm .
#	rm ./LogisticaVerdeEscalonamento.o
	gcc $(args) -c  LogisticaVerdeEscalonamento.c 
	gcc $(args) LogisticaVerdeEscalonamento.o  main.c -o escalonador -lm
clean:
	rm ./escalonador 
	rm ./LogisticaVerdeEscalonamento.o
