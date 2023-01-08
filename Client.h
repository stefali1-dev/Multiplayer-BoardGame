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
    char sv_msg[100];
    char cl_msg[100];
    
public:
    bool must_reply;
    int player_index;
    Client();
    ~Client();
    void set_cl_msg(char* str);
    char* get_sv_msg();
    int connect_(const char * sv_adress, const char * input_port);
    int read_();
    int write_();
    void mainLoop();
};

