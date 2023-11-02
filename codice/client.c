#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BENVENUTO_CLIENT "find --> ricerca la disponibilità per una prenotazione\nbook --> invia una prenotazione\nesc --> termina il client\n"
#define BUFFER_SIZE 1024
#define nTavoli 16

// Invia al socket in input il messaggio dentro buffer
int invia(int j, char* buffer) {
	int len = htonl(strlen(buffer));
	int lmsg = htons(len);
	int ret;

	// Invio la dimensione del messaggio
	ret = send(j, (void*) &lmsg, sizeof(uint16_t), 0);
	// Invio il messaggio
	ret = send(j, (void*) buffer, len, 0);

	// Comunico l'esito
	return ret;
}

int main(int argc, char* argv[]){
	int ret, sd, i;

	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	char *clientCommand;

	// Set di descrittori da monitorare
	fd_set master;

	// Set dei descrittori pronti
	fd_set read_fds;

	// Descrittore max
	int fdmax;

	/* Creazione socket */
	sd = socket(AF_INET,SOCK_STREAM,0);
	
	/* Creazione indirizzo del server */
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4242);
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	
	/* Connessione */
	ret = connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	if(ret < 0){
		perror("Errore in fase di connessione: \n");
		exit(1);
	}

	// Reset dei descrittori
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

	// Aggiungo il socket di ascolto 'listener' e 'stdin' (0) ai socket monitorati
    FD_SET(sd, &master); 
    FD_SET(0, &master); 

	// Tengo traccia del nuovo fdmax
    fdmax = sd;

    // Stampo i comandi che il client può digitare
    printf(BENVENUTO_CLIENT);

	for(;;){
		// Inizializzo il set read_fds, manipolato dalla select()
        read_fds = master;

		// Controllo i descrittori
		ret = select(fdmax+1, &read_fds, NULL, NULL, NULL);
		if(ret < 0) {
			perror("Errore nella select!");
			exit(-1);
		}

		// Scorro ogni descrittore 'i'
		for(i = 0; i <= fdmax; i++) {
			if(FD_ISSET(i, &read_fds)){
				if(i == 0) { // Primo caso: comando da stdin
					scanf(" %[^\n]", buffer); // Lo inserisco nel buffer e poi lo analizzo
					clientCommand = strtok(buffer, " ");
					
				}
				else { // Secondo caso: il socket è sd

				}
			}
		}
	}
}