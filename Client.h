#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>

struct thData
{
    int sd;
    bool has_read;
    int info[5];
    int chosen_pawn;
};

class Client
{
private:
    /* data */
    int port;
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
    bool connected;
    char sv_msg[100];
    char cl_msg[100];

public:
    pthread_t th[100]; // Identificatorii thread-urilor de read si write
                     // 0 pentru citit; 1 pentru scris
    int i = 0;

    thData *td0, *td1;

    bool must_reply;
    int player_index;
    Client();
    ~Client();
    void set_cl_msg(char *str);
    char *get_sv_msg();
    int connect_(const char *sv_adress, const char *input_port);
    int read_();
    int write_();
    void mainLoop();
};
