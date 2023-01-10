
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
#define MAX_NR_OF_PLAYERS 4

struct thData
{
    int idThread = -1; // id-ul thread-ului tinut in evidenta de acest program
    int cl;            // descriptorul intors de accept
    GameBoard *gameBoard;
    int info[7]; // informatiile de trimis catre clienti

    int *thread_count; // numarul de clienti
    int *cl_arr; // array cu drescriptorii de clienti
    int *game_score; // arey cu scorurile jucatorilor
};

void citeste(void *arg)
{
    int chosen_pawn;
    thData *tdL;
    tdL = ((thData *)arg);
    if (read(tdL->cl, &chosen_pawn, sizeof(int)) <= 0) // primim pionul ales de jucator
    {
        printf("S-a deconectat jucatorul %d\n", tdL->idThread);
        tdL->thread_count--;
        shutdown(tdL->cl, SHUT_WR);
        pthread_exit(0);
    }

    printf("Jucator %d : mutare receptionata: %d\n", tdL->idThread, chosen_pawn);

    if (tdL->idThread == tdL->gameBoard->turn) // verificam daca e randul jucatorului respectiv
    {
        int player_index = tdL->idThread;
        int pawn_collision = -1, player_collision = -1;

        if (chosen_pawn != -1)
        {
            if (tdL->gameBoard->isValidPawnMove(player_index, chosen_pawn, tdL->gameBoard->dice))
            {
                tdL->gameBoard->movePawn(player_index, chosen_pawn, tdL->gameBoard->dice, pawn_collision, player_collision);
            }
        }

        tdL->gameBoard->dice = rand() % 6 + 1;

        tdL->gameBoard->turn = (tdL->gameBoard->turn + 1) % *tdL->thread_count;
        tdL->info[0] = player_index;
        tdL->info[1] = chosen_pawn;
        tdL->info[2] = tdL->gameBoard->dice;
        tdL->info[3] = tdL->gameBoard->hasFinnished(player_index);
        tdL->info[4] = tdL->gameBoard->turn;

        tdL->info[5] = pawn_collision;
        tdL->info[6] = player_collision;

        // printf("\n\n%d\n\n", *tdL->thread_count);

        for (int i = 0; i < *tdL->thread_count; i++)
        {

            if (write(tdL->cl_arr[i], tdL->info, sizeof(int) * 7) <= 0)
            {
                perror("[client]Eroare la write() spre server.\n");
                return;
            }
        }

        if (tdL->gameBoard->hasFinnished(player_index))
        {
            delete tdL->gameBoard;
            tdL->gameBoard = new GameBoard();

            tdL->game_score[player_index]++;

            FILE *fp;
            char *line = NULL;
            size_t len = 0;
            ssize_t read;

            int k = 0;
            char str[100];

            fp = fopen("scores.txt", "w");

            strcpy(str, "player 0 : x\n");
            str[11] = tdL->game_score[k++] + 48;

            fprintf(fp, "%s", str);

            strcpy(str, "player 1 : x\n");
            str[11] = tdL->game_score[k++] + 48;

            fprintf(fp, "%s", str);
            strcpy(str, "player 2 : x\n");
            str[11] = tdL->game_score[k++] + 48;

            fprintf(fp, "%s", str);
            strcpy(str, "player 3 : x");
            str[11] = tdL->game_score[k++] + 48;

            fprintf(fp, "%s", str);

            fclose(fp);
        }
    }

    
}

static void *treat(void *arg)
{
    
    thData *tdL;
    tdL = ((thData *)arg);

    if (write(tdL->cl, &tdL->idThread, sizeof(int)) <= 0) // trimitem numarul jucatorului
    {
        perror("[client]Eroare la write() spre server.\n");
    }

    while (1)
    {
        fflush(stdout);
        pthread_detach(pthread_self());
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
    pthread_t th[4]; // Identificatorii thread-urilor

    GameBoard *gameBoard;

    int game_score[4];

public:
    int cl_arr[4];
    int i;
    int *thread_count;
    int initialize()
    {
        i = 0;
        thread_count = &i;
        gameBoard = new GameBoard();
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        int k = 0;

        fp = fopen("scores.txt", "r");

        while ((read = getline(&line, &len, fp)) != -1)
        {
            game_score[k++] = line[strlen(line) - 1] - 48; // saving as int
        }

        fclose(fp);

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
        if (listen(sd, 4) == -1)
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

            /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
            if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
            {
                perror("[server]Eroare la accept().\n");
                continue;
            }

            /* s-a realizat conexiunea, se astepta mesajul */
            td = new thData();
            td->idThread = i;
            td->cl = client;
            cl_arr[*thread_count] = client;

            td->gameBoard = gameBoard;
            td->thread_count = thread_count;
            td->cl_arr = cl_arr;
            td->game_score = game_score;

            if (i < MAX_NR_OF_PLAYERS)
            {
                pthread_create(&th[i++], NULL, &treat, td);
            }
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