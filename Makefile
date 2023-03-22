all:
	gcc -o cliente socket_cli.c
	gcc -o servidor socket_srv.c