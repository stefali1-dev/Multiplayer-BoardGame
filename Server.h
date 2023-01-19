
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
    int idThread = -1; // thread id, for each client
    int cl;            // client descriptor
    GameBoard *gameBoard;
    int info[7]; // information to send to the clients

    int *thread_count; // numbers of threads/clients
    int *cl_arr; // array with the clients' descriptors
};

void citeste(void *arg);

static void *treat(void *arg);

class Server
{
private:
    struct sockaddr_in server; // server struct
    struct sockaddr_in from;
    int sd; // socket descriptor
    int pid;
    pthread_t th[4]; // thread identifiers

    GameBoard *gameBoard;

    int i;

public:
    int cl_arr[4];
    int *thread_count;

    int initialize();

    int acceptLoop();
};
