
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include "GameBoard.h"

#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
pthread_mutex_t lock;

struct thData
{
    int idThread = -1; // id-ul thread-ului tinut in evidenta de acest program
    int cl;            // descriptorul intors de accept
    GameBoard *gameBoard;
    bool *must_send_info;
    pthread_t *th_arr;
    int *i;
};

void citeste(void *arg)
{
    int chosen_pawn, i = 0;
    thData *tdL;
    tdL = ((thData *)arg);
    if (read(tdL->cl, &chosen_pawn, sizeof(int)) <= 0) // primim pionul ales de jucator
    {
        printf("[Thread %d]\n", tdL->idThread);
        perror("Eroare la read() de la client.\n");
    }

    printf("[Thread %d]Mesajul a fost receptionat...%d\n", tdL->idThread, chosen_pawn);

    pthread_mutex_lock(&lock);
    if (tdL->idThread == tdL->gameBoard->turn) // verificam daca e randul jucatorului respectiv
    {
        int player_index = tdL->idThread;
        // int dice = tdL->gameBoard->dice;
        int dice = 6;

        if (tdL->gameBoard->isValidPawnMove(player_index, chosen_pawn, dice))
        {
            tdL->gameBoard->movePawn(player_index, chosen_pawn, dice);
        }

        // tdL->gameBoard->turn++;

        tdL->gameBoard->info[0] = player_index;
        tdL->gameBoard->info[1] = chosen_pawn;
        tdL->gameBoard->info[2] = dice;
        tdL->gameBoard->info[3] = tdL->gameBoard->hasFinnished(player_index);
        tdL->gameBoard->info[4] = tdL->gameBoard->turn;

        for (int j = 0; j < *(tdL->i); j++) // trebuie sa trimitem info catre toti clientii conectati
            tdL->gameBoard->must_send_info[j] = true;
    }

    pthread_mutex_unlock(&lock);
}
/*static void *send_move(void *arg)
{
    thData *tdL;
    tdL = ((thData *)arg);

    for(int i = 0; tdL->th_arr[i]->)
}*/

static void *treat(void *arg)
{

    thData *tdL;
    tdL = ((thData *)arg);

    while (1)
    {
        printf("[thread]- %d - Asteptam mesajul...\n", tdL->idThread);
        fflush(stdout);
        pthread_detach(pthread_self());
        /*if (tdL->gameBoard->must_send_info[tdL->idThread] == true)
        {
            pthread_mutex_lock(&lock);
            
            if (write(tdL->cl, tdL->gameBoard->info, sizeof(int)*5) <= 0) // trimitem info
            {
                printf("[Thread %d]\n", tdL->idThread);
                perror("Eroare la write() de la client.\n");
            }

            tdL->gameBoard->must_send_info[tdL->idThread] = false;

            pthread_mutex_unlock(&lock);
        }*/
        
        citeste((thData *)arg);
    }
    /* am terminat cu acest client, inchidem conexiunea */
    close((intptr_t)arg);
    return (NULL);
};

class Server2
{
private:
    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    int nr; // mesajul primit de trimis la client
    int sd; // descriptorul de socket
    int pid;
    pthread_t th[999]; // Identificatorii thread-urilor
    int i = 0;
    bool *must_send_info;

    GameBoard *gameBoard;

public:
    int initialize()
    {
        gameBoard = new GameBoard();

        *must_send_info = false;

        pthread_mutex_init(&lock, NULL);
        /* crearea unui socket */
        if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("[server]Eroare la socket().\n");
            return -1;
        }
        /* utilizarea optiunii SO_REUSEADDR */
        int on = 1;
        setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        /* pregatirea structurilor de date */
        bzero(&server, sizeof(server));
        bzero(&from, sizeof(from));

        /* umplem structura folosita de server */
        /* stabilirea familiei de socket-uri */
        server.sin_family = AF_INET;
        /* acceptam orice adresa */
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        /* utilizam un port utilizator */
        server.sin_port = htons(PORT);

        /* atasam socketul */
        if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
        {
            perror("[server]Eroare la bind().\n");
            return -1;
        }
        return 0;
    }

    int acceptLoop()
    {
        /* punem serverul sa asculte daca vin clienti sa se conecteze */
        if (listen(sd, 2) == -1)
        {
            perror("[server]Eroare la listen().\n");
            return -1;
        }
        /* servim in mod concurent clientii...folosind thread-uri */
        while (1)
        {
            int client;
            thData *td; // parametru functia executata de thread
            socklen_t length = sizeof(from);

            printf("[server]Asteptam la portul %d...\n", PORT);
            fflush(stdout);

            // client= malloc(sizeof(int));
            /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
            if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
            {
                perror("[server]Eroare la accept().\n");
                continue;
            }

            /* s-a realizat conexiunea, se astepta mesajul */

            // int idThread; //id-ul threadului
            // int cl; //descriptorul intors de accept

            td = new thData();
            td->idThread = i;
            td->cl = client;
            td->gameBoard = gameBoard;
            td->must_send_info = must_send_info;
            td->th_arr = th;
            td->i = &i;

            pthread_create(&th[i++], NULL, &treat, td);
        }
        return 0;
    }
};

int main()
{
    Server2 sv;
    sv.initialize();
    sv.acceptLoop();
}