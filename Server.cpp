#include "Server.h"

void addChar(char *str, char c)
{
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

char *conv_addr(struct sockaddr_in address)
{
    static char str[25];
    char port[7];

    /* adresa IP a clientului */
    strcpy(str, inet_ntoa(address.sin_addr));
    /* portul utilizat de client */
    bzero(port, 7);
    sprintf(port, ":%d", ntohs(address.sin_port));
    strcat(str, port);
    return (str);
}

Server::Server(/* args */) : dArrIndex(0)
{
    /* creare socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Eroare la socket().\n");
        exit(-1);
    }

    /*setam pentru socket optiunea SO_REUSEADDR */
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    /* pregatim structurile de date */
    bzero(&server, sizeof(server));

    /* umplem structura folosita de server */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        printf("Eroare la bind().\n");
        exit(-1);
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 4) == -1)
    {
        printf("Eroare la listen().\n");
        exit(-1);
    }

    /* completam multimea de descriptori de citire */
    FD_ZERO(&actfds);    /* initial, multimea este vida */
    FD_SET(sd, &actfds); /* includem in multime socketul creat */

    tv.tv_sec = 1; /* se va astepta un timp de 1 sec. */
    tv.tv_usec = 0;

    /* valoarea maxima a descriptorilor folositi */
    nfds = sd;

    printf("Asteptam la portul %d...\n", PORT);
    fflush(stdout);

    // game related
    gameBoard = new GameBoard();
}

int Server::writeToAllExcept(int playerIndex)
{
    addChar(sv_msg, '0');

    for (int i = 0; i < dArrIndex; i++)
    {

        if (i != playerIndex && dArr[i] != sd)
        {

            if (write(dArr[i], sv_msg, sizeof(sv_msg)) < 0)
            {
                printf(" Eroare la write() catre client.\n");
                return -1;
            }
        }
    }

    return 0;
}

int Server::writeTo(int playerIndex)
{
    addChar(sv_msg, '1');

    if (write(dArr[playerIndex], sv_msg, sizeof(sv_msg)) < 0)
    {
        printf(" Eroare la write() catre client.\n");
        return -1;
    }
    return 0;
}

void Server::getClientName(int fd)
{
    int bytes;
    bytes = read(fd, cl_msg, sizeof(cl_msg));
    if (bytes < 0)
    {
        perror("Eroare la read() de la client.\n");
    }

    names.push_back(cl_msg);
}

int Server::selectLoop()
{
    while (1)
    {
        bcopy((char *)&actfds, (char *)&readfds, sizeof(readfds));

        if (select(nfds + 1, &readfds, NULL, NULL, &tv) < 0)
        {
            perror(" Eroare la select().\n");
            return errno;
        }
        if (FD_ISSET(sd, &readfds))
        {
            len = sizeof(from);
            bzero(&from, sizeof(from));

            client = accept(sd, (struct sockaddr *)&from, &len);

            if (client < 0)
            {
                perror(" Eroare la accept().\n");
                continue;
            }

            FD_SET(client, &actfds);
            dArr[dArrIndex++] = client;

            printf(" S-a conectat clientul cu descriptorul %d, de la adresa %s.\n", client, conv_addr(from));
            fflush(stdout);

            getClientName(client);
        }

        // vedem daca e pregatit vreun socket client pentru a trimite numele
        if (gotAllNames())
        {
            printf("Got all names\n");
            strcpy(sv_msg, "All players connected");
            writeToAllExcept(-1);

            break;
        }
    }
}

void Server::gameLoop()
{
    turn = 0;
    std::string str;
    while (1)
    {
        int dice = rand() % 6 + 1;

        str = "Dice: " + std::to_string(dice) + " | Turn: " + names[turn];

        strcpy(sv_msg, str.c_str());
        writeToAllExcept(turn);

        str = "Dice: " + std::to_string(dice) + " | Your turn -> select pawn";
        strcpy(sv_msg, str.c_str());
        writeTo(turn);

        int *selectedPawn;
        if (read(dArr[turn], &selectedPawn, sizeof(int)) < 0)
        {
            perror("Eroare la read() de la client.\n");
            return;
        }

        while (gameBoard->isValidPawnMove(turn, *selectedPawn, dice) == false)
        {
            strcpy(sv_msg, "You can't move that pawn -> select another pawn");
            writeTo(turn);

            if (read(dArr[turn], &selectedPawn, sizeof(int)) < 0)
            {
                perror("Eroare la read() de la client.\n");
                return;
            }
        }

        gameBoard->movePawn(turn, *selectedPawn, dice);

        str = std::to_string(turn) + std::to_string(*selectedPawn) + std::to_string(dice) + std::to_string(2);
        strcpy(sv_msg, str.c_str());
        writeToAllExcept(-1);; // info to update on the board 
    }
}

/*
int Server::sayHello(int fd)
{
    char buffer[100];
    int bytes;
    char msg[100];           // mesajul primit de la client
    char msgrasp[100] = " "; // mesaj de raspuns pentru client

    bytes = read(fd, msg, sizeof(buffer));
    if (bytes < 0)
    {
        perror("Eroare la read() de la client.\n");
        return 0;
    }
    printf("Mesajul a fost receptionat...%s\n", msg);

    bzero(msgrasp, 100);
    strcat(msgrasp, "Hello ");
    strcat(msgrasp, msg);

    printf("Trimitem mesajul inapoi...%s\n", msgrasp);

    addChar(msgrasp, '0');

    if (bytes && write(fd, msgrasp, bytes) < 0)
    {
        perror(" Eroare la write() catre client.\n");
        return 0;
    }

    return bytes;
}
*/

int main(int argc, char *argv[])
{
    // printf("%s", conv_addr())

    Server *S = new Server();

    S->selectLoop();

    S->gameLoop();
}