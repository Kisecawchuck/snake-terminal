main : egg.o player.o renders.o main.o
	cc egg.o player.o renders.o main.o -o main -Wall

main.o : main.c
	cc -c main.c -o main.o

egg.o : egg.c
	cc -c egg.c -o egg.o

player.o : player.c
	cc -c player.c -o player.o

renders.o : renders.c
	cc -c renders.c -o renders.o

clean :
	rm -rf *.o main

