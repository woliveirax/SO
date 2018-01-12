client:
	gcc client.c client.h -o client -lpthread -lncurses

clean:
		rm client

pipe:
	rm ../Data/pipes/Client_*

debug:
	gcc client.c client.h -g -o client -lpthread -lncurses
	gdb ./client
