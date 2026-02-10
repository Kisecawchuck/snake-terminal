main : egg.o list.o main.o
	cc egg.o list.o main.o -o main -Wall

main.o : main.c
	cc -c main.c -o main.o

egg.o : egg.c
	cc -c egg.c -o egg.o

list.o : list.c
	cc -c list.c -o list.o

clean :
	rm -rf *.o main

