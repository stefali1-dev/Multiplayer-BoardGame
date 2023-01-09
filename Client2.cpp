/* cliTCPIt.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".

   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

/* codul de eroare returnat de anumite apeluri */

/* portul de conectare la server*/
int port;

pthread_mutex_t lock;

struct thData
{
    int idThread; // id-ul thread-ului tinut in evidenta de acest program
    int cl;       // descriptorul intors de accept
    int nr_to_write;
    int nr_read;
    bool read_msg;
    bool sent_msg;
    int sd;
};

void *read_td(void *arg) // FUNCTIA THREAD DE CITIRE MESAJ SERVER
{
    int nr;
    thData *tdL;
    tdL = ((thData *)arg);

    if (read(tdL->sd, &nr, sizeof(int)) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
    }

    /* afisam mesajul primit */
    printf("[client]Mesajul primit este: %d\n\n\n", nr);
    tdL->nr_read = nr;
    tdL->read_msg = true;

    return (NULL);
};

void *write_td(void *arg) // FUNCTIA THREAD DE TRIMITERE MESAJ CATRE SERVER
{
    int nr;
    thData *tdL;
    tdL = ((thData *)arg);

    /* trimiterea mesajului la server */
    nr = tdL->nr_to_write;
    if (write(tdL->sd, &nr, sizeof(int)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return (NULL);
    }

    tdL->sent_msg = true;

    return (NULL);
};

int main(int argc, char *argv[])
{
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
                               // mesajul trimis
    int nr = 0;
    char buf[10];

    pthread_t th[100]; // Identificatorii thread-urilor care se vor crea

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return -1;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return -1;
    }

    int i = 0;

    int client;
    thData *td, *td2;

    td = new thData();
    td->idThread = i++;
    td->cl = client;
    td->read_msg = false;
    td->sd = sd;

    td2 = new thData();
    td2->idThread = i++;
    td2->cl = client;
    td2->read_msg = false;
    td2->sd = sd;

    //pthread_create(&th[1], NULL, &read_td, td);

    int x = 0;

    while (1) // WHILE-UL IN CARE AFISEZ INTERFATA
    {

        // writing message

        printf("Introdu numarul: ");
        fflush(stdout);
        read(0, buf, sizeof(buf));
        nr = atoi(buf);
        td2->nr_to_write = nr;
        td2->sent_msg = false;
        pthread_create(&th[2], NULL, &write_td, td2); // TRIMITE CATRE SERVER

        /*if (td->read_msg) // DACA A PRIMIT MESAJ
        {
            td->read_msg = false;
            printf("mesaj: %d\n", td->nr_read);

            // sleep(1);

            pthread_create(&th[1], NULL, &read_td, td); // INCEPE IAR LISTEN-UL PENTRU CITIRE
        }*/

        /* inchidem conexiunea, am terminat */
        // close (sd);
    }
}