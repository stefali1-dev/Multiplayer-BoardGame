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
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
    bool connected;

public:
    Client();
    ~Client();
    int connect_(const char * sv_adress, const char * input_port);
    int read_(char* sv_msg);
    int write_(char* cl_msg);
};

