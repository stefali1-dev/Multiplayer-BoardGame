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
#include "Interface.cpp"

class Client
{
private:
    /* data */
    bool must_reply;
    int port;
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
    bool connected;
    char sv_msg[100];
    char cl_msg[100];
    int player_index;
    Interface* interface;
    
public:
    
    Client();
    ~Client();
    void setCl_msg(char* str);
    int connect_(const char * sv_adress, const char * input_port);
    int read_();
    int writeCl_msg();
    void mainLoop();
};

