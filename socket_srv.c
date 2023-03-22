#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <linux/in.h>
#include <string.h>
#include <unistd.h>

void main(void){
	struct sockaddr_in server_addr,  client_addr;
	int sd, sc;
	int size;
	int N;

	sd =  socket(AF_INET, SOCK_STREAM, 0); 
	bzero((char *)&server_addr, sizeof(server_addr)); //Asigna la dirección del socket
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;	
	server_addr.sin_port = htons(4200);
	bind(sd, &server_addr, sizeof(server_addr));

	listen(sd, 5);
	size = sizeof(client_addr);

	while (1){
		printf("esperando conexion\n");
		sc = accept(sd, (struct sockaddr *)&client_addr, &size);
		if (sc < 0)
			break;

		//Recibe la petición de la variable N, utilizada para definir el tamaño de las matrices.
		read(sc, &N,sizeof(N));
		printf("Tamaño de la matriz:%d\n ", N);
	
		//Inicializa las matrices A, B y C. Recibe las matrices A y B del cliente y las almacena.
		float A[N][N], B[N][N], C[N][N];
		read(sc, (char *)A, N * N * sizeof(float));
        read(sc, (char *)B, N * N * sizeof(float));

		printf("Matriz A en servidor:\n");
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				printf("%f ", A[i][j]);
			}
			printf("\n");
		}

		printf("Matriz B en servidor:\n");
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				printf("%f ", B[i][j]);
			}
			printf("\n");
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				C[i][j] = 0.0;
				for (int k = 0; k < N; k++) {
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}

		printf("Matriz C en servidor:\n");
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				printf("%f ", C[i][j]);
			}
			printf("\n");
		}
		
		//Envía el resultado y cierra la conexión.
		write(sc, (char *)C, N*N*sizeof(float));	
		close(sc);
    }

	printf("Error en accpet\n");
	close (sd);
	exit(0);
}	
