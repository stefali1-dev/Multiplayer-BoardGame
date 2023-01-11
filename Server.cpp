#include "Server.h"

void citeste(void *arg)
{
    int chosen_pawn;
    thData *tdL;
    tdL = ((thData *)arg);
    if (read(tdL->cl, &chosen_pawn, sizeof(int)) <= 0) // primim pionul ales de jucator
    {
        printf("Player %d has disconnected\n", tdL->idThread);
        tdL->thread_count--;
        shutdown(tdL->cl, SHUT_WR);
        pthread_exit(0);
    }

    printf("Player %d : chosen pawn: %d\n", tdL->idThread, chosen_pawn);

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

int Server::initialize()
{
    i = 0;
    thread_count = &i;

    gameBoard = new GameBoard();

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

int Server::acceptLoop()
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

        printf("Waiting on port %d...\n", PORT);
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

        if (i < MAX_NR_OF_PLAYERS)
        {
            pthread_create(&th[i++], NULL, &treat, td);
        }
    }
    return 0;
}

int main()
{
    Server sv;
    sv.initialize();
    sv.acceptLoop();
}