all: server_interface

server_interface:	server_funcs.o	server_interface.o server_support_funcs.o
	gcc server_funcs.o	server_interface.o server_support_funcs.o -o	server_interface -lpthread -lncurses

debug: server_funcs.o	server_interface.o server_support_funcs.o
	gcc server_funcs.o	server_interface.o server_support_funcs.o -g -o	server_interface -lpthread

server_support_funcs.o: ../server_support_funcs/server_support_funcs.h ../server_support_funcs/server_support_funcs.c
	gcc -c ../server_support_funcs/server_support_funcs.c

server_interface.o:	server_interface.c
	gcc -c server_interface.c

server_funcs.o:	server_funcs.c	server_funcs.h
	gcc -c server_funcs.c

clean:
	rm *.o server_interface

clean.o:
	rm *.o
