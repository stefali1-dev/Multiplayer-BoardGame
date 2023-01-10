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

class Client
{
private:
    /* data */
    int port;
                       // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
public:
    int sd; 
    int player_index;
    Client();
    int connect_(const char *sv_adress, const char *input_port);
};
