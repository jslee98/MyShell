CC=gcc
CFLAGS=-Wall

shell.o: main.o myshell.o history.o
	$(CC) $(CFLAGS) -o shell.o main.o myshell.o history.o

main: main.c history.h myshell.h
	$(CC) $(CFLAGS) -c main.o main.c history.h myshell.h

myshell: myshell.c myshell.h history.h
	$(CC) $(CFLAGS) -c myshell.o myshell.c myshell.h history.h

history: history.c history.h myshell.h
	$(CC) $(CFLAGS) -c history.o history.c history.h myshell.h
