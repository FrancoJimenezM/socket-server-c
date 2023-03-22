#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <linux/in.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	
	if (argc != 3) {
        printf("Uso: %s <tamaño matriz> <ip>", argv[0]);
        return 1;
    }

	int N = atoi(argv[1]);
	const char * ip = argv[2];
	int sd;
	struct sockaddr_in server_addr;
	struct hostent *hp;
	// Inicialización de las matrices A, B y res que será la respuesta del servidor.
	float A[N][N], B[N][N];
	float res[N][N];
	
	// srand(time(NULL));
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			A[i][j] = (float) rand() / RAND_MAX * 100.0f;
			
			B[i][j] = (float) rand() / RAND_MAX * 100.0f;			
		}
	}

	printf("Matriz A:\n");
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			printf("%f ", A[i][j]);
		}
		printf("\n");
	}

	printf("Matriz B:\n");
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			printf("%f ", B[i][j]);
		}
		printf("\n");
	}

	sd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *)&server_addr, sizeof(server_addr)); //se obtiene y rellena la dirección del servidor
	hp = gethostbyname (ip);

	if (hp==NULL){
		printf("Error en la llamada gethostbyname");
		exit(0);
	}
        
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4200);

	/* se establece la conexi�n */
	if (connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("Error en la llamada connect");
		exit(0);
	}
	
	//Enviamos N, ingresado por el usuario.
	send(sd, &N, sizeof(N),0);
	//Enviamos la matriz A de nxn, anteriormente llenada con números aleatorios.
	send(sd, (char *) A, N * N *sizeof(float),0);
	//Enviamos la matriz B de nxn, anteriormente llenada con números aleatorios.
	send(sd, (char *) B, N * N *sizeof(float),0);

	//Lee los datos de C = AxB y lo almacena en res
	read(sd, (char *)&res, N*N*sizeof(float));	
	

	printf("Matriz resultado:\n");
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			printf("%f ", res[i][j]);
		}
		printf("\n");
	}
	close (sd);
	exit(0);
}
