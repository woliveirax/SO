all: server

server:	server_funcs.o	server.o
	gcc server_funcs.o	server.o -o	server

server.o:	server.c
	gcc -c server.c

server_funcs.o:	server\server_funcs.c	server\server_funcs.h
	gcc -c server\server_funcs.c

clean:
	rm *.o server.exe

clean.o:
	rm *.o
