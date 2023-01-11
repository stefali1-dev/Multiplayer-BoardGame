
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
#include "GameBoard.cpp"

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
};

void citeste(void *arg);

static void *treat(void *arg);

class Server
{
private:
    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    int nr; // mesajul primit de trimis la client
    int sd; // descriptorul de socket
    int pid;
    pthread_t th[4]; // Identificatorii thread-urilor

    GameBoard *gameBoard;

    int i;

public:
    int cl_arr[4];
    int *thread_count;

    int initialize();

    int acceptLoop();
};
