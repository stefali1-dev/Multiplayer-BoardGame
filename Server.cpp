#include "Server.h"

void read_(void *arg)
{
    int chosen_pawn;
    thData *tdL;
    tdL = ((thData *)arg);
    if (read(tdL->cl, &chosen_pawn, sizeof(int)) <= 0) // receiving chosen pawn from the player
    {
        printf("Player %d has disconnected\n", tdL->idThread);
        tdL->thread_count--;
        shutdown(tdL->cl, SHUT_WR);
        pthread_exit(0);
    }

    printf("Player %d : chosen pawn: %d\n", tdL->idThread, chosen_pawn);

    if (tdL->idThread == tdL->gameBoard->turn) // checking player turn
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

        for (int k = 0; k < *tdL->thread_count; k++)
        {

            if (write(tdL->cl_arr[k], tdL->info, sizeof(int) * 7) <= 0)
            {
                perror("Error at write() to client.\n");
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
        perror("Error at write() to client\n");
    }

    while (1)
    {
        fflush(stdout);
        pthread_detach(pthread_self());
        read_((thData *)arg);
    }

    // closing connection to this client
    close((intptr_t)arg);

    return (NULL);
};

int Server::initialize()
{
    i = 0;
    thread_count = &i;

    gameBoard = new GameBoard();

    // socket creation
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket() error\n");
        return -1;
    }

    // utilising SO_REUSEADDR option
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // data structures preparation
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    // filling server struct 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // attaching socket
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return -1;
    }
    return 0;
}

int Server::acceptLoop()
{
    if (listen(sd, 4) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return -1;
    }

    // serving clients using threads
    while (1)
    {
        int client;
        thData *td; // to be given as parameter for the thread function
        socklen_t length = sizeof(from);

        printf("Waiting on port %d...\n", PORT);
        fflush(stdout);

        // accepting a client (blocking state)
        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
        {
            perror("Error at accept().\n");
            continue;
        }

        // connection made
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