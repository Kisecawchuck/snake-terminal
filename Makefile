CFLAGS	:= -Wall -Wextra -Wpedantic

main : egg.o player.o renders.o main.o
	cc $(CFLAGS) egg.o player.o renders.o main.o -o main

main.o : main.c
	cc $(CFLAGS) -c main.c -o main.o

egg.o : egg.c
	cc $(CFLAGS) -c egg.c -o egg.o

player.o : player.c
	cc $(CFLAGS) -c player.c -o player.o

renders.o : renders.c
	cc $(CFLAGS) -c renders.c -o renders.o

clean :
	rm -rf *.o main

