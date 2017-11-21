all: server_interface

server_interface:	server_funcs.o	server_interface.o
	gcc server_funcs.o	server_interface.o -o	server_interface

server_interface.o:	server_interface.c
	gcc -c server_interface.c

server_funcs.o:	server\server_funcs.c	server\server_funcs.h
	gcc -c server\server_funcs.c

clean:
	rm *.o server_interface.exe

clean.o:
	rm *.o
