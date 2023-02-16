/* Strutture e definizioni necessarie al server */
/* -------------------------------------------- */

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define nTavoli 16
#define nPiatti 8
#define nMaxClient 16
#define nMaxTd nTavoli
#define nMaxKd 8
#define BUFFER_SIZE 1024
#define BENVENUTO_SERVER "???"

/*           Gestione dei tavoli 
   ---------------------------------------
   Viene parsato il file "tavoli.txt" dopo
   l'accensione del server.
   --------------------------------------- 
*/

struct tavolo
{
	int numero;	// Univoco nel ristorante
	int nPosti;
	char sala[32];
	char descrizione[64];
};

struct tavolo tavoli[nTavoli];

/*       Gestione delle prenotazioni 
   ---------------------------------------
   Per ogni tavolo, esiste una lista di 
   prenotazioni ordinata in base al time-
   stamp di esecuzione.
   --------------------------------------- 
*/

struct prenotazione
{
	char cognome[64];
	time_t data_ora;	// Non della richiesta, ma della prenotazione
	struct prenotazione *prossima;
	/* 
	Il tavolo non è necessario poiché corrisponde
	all'indice in cui viene salvato all'interno
	dell'array "prenotazioni".
	*/
};

struct prenotazione prenotazioni[nTavoli];

/*           Gestione dei piatti 
   ---------------------------------------
   Viene parsato il file "menu.txt" dopo
   l'accensione del server.
   --------------------------------------- 
*/

struct piatto
{
	char codice[2];
	char nome[64];
	int prezzo;
	/* 
	Il tavolo non è necessario poiché corrisponde
	all'indice in cui viene salvato all'interno
	dell'array "comande".
	*/
};

struct piatto piatti[nPiatti];

/*         Gestione delle comande 
   ---------------------------------------
   Per ogni tavolo, esiste una lista di 
   comande ordinata in base al timestamp
   di richiesta. Vengono cancellate alla
   richiesta del conto.
   --------------------------------------- 
*/

enum stato_comanda{in_attesa, in_preparazione, in_servizio};

struct comanda
{
	int quantita[nPiatti];
	/* 
	Quantità dell'i-esimo piatto corrispondete
	nell'array dei piatti.
	*/
	time_t timestamp;	// utilizzato per trovare la meno recente
	enum stato_comanda stato;
	int kd;
	/* 
	Il tavolo non è necessario poiché corrisponde
	all'indice in cui viene salvato all'interno
	dell'array "comande".
	*/
	struct comanda *prossima;
};

struct comanda comande[nTavoli];

/*    Gestione del tipo di dispositivi
   ---------------------------------------
   Ho 3 array di interi che tengono gli ID
   dei socket dei relativi dispositivi.
   ---------------------------------------
*/
