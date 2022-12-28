#include "Server.h"

void addChar(char* str, char c){
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

Server::Server(/* args */): dArrIndex(0)
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

int Server::writeToAll(char *str)
{

    char buffer[100];
    addChar(str, '0');

    for(int i = 0; i < dArrIndex; i++){

        if (dArr[i] != sd){

            if (write(dArr[i], str, sizeof(buffer)) < 0)
            {
                printf(" Eroare la write() catre client.\n");
                return -1;
            }
        }
    }

    return 0;
}

int Server::selectLoop()
{
    while (1)
    {
        /* ajustam multimea descriptorilor activi (efectiv utilizati) */
        bcopy((char *)&actfds, (char *)&readfds, sizeof(readfds));

        /* apelul select() */
        if (select(nfds + 1, &readfds, NULL, NULL, &tv) < 0)
        {
            perror(" Eroare la select().\n");
            return errno;
        }
        /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
        if (FD_ISSET(sd, &readfds))
        {
            /* pregatirea structurii client */
            len = sizeof(from);
            bzero(&from, sizeof(from));

            /* a venit un client, acceptam conexiunea */
            client = accept(sd, (struct sockaddr *)&from, &len);

            /* eroare la acceptarea conexiunii de la un client */
            if (client < 0)
            {
                perror(" Eroare la accept().\n");
                continue;
            }

            if (nfds < client) /* ajusteaza valoarea maximului */
                nfds = client;

            /* includem in lista de descriptori activi si acest socket */
            FD_SET(client, &actfds);
            dArr[dArrIndex++] = client;

            printf(" S-a conectat clientul cu descriptorul %d, de la adresa %s.\n", client, conv_addr(from));
            fflush(stdout);
        }
        /* vedem daca e pregatit vreun socket client pentru a trimite raspunsul */
        for (fd = 0; fd <= nfds; fd++) /* parcurgem multimea de descriptori */
        {
            /* este un socket de citire pregatit? */
            if (fd != sd && FD_ISSET(fd, &readfds))
            {
                sayHello(fd);
                /*
                char msg[100];
                strcpy(msg, "TO ALL");
                writeToAll(msg);*/
            }
        } /* for */
    }
}

/* realizeaza primirea si retrimiterea unui mesaj unui client */
int Server::sayHello(int fd)
{
    char buffer[100];        /* mesajul */
    int bytes;               /* numarul de octeti cititi/scrisi */
    char msg[100];           // mesajul primit de la client
    char msgrasp[100] = " "; // mesaj de raspuns pentru client

    bytes = read(fd, msg, sizeof(buffer));
    if (bytes < 0)
    {
        perror("Eroare la read() de la client.\n");
        return 0;
    }
    printf("Mesajul a fost receptionat...%s\n", msg);

    /*pregatim mesajul de raspuns */
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

int Server::handleCommand(char *str)
{
    return 0;
}

int main(int argc, char *argv[])
{
    // printf("%s", conv_addr())

    Server *S = new Server();

    S->selectLoop();
}