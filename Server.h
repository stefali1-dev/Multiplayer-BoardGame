#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <array>

#define PORT 2728

class Server
{
private:
    struct sockaddr_in server; /* structurile pentru server si clienti */
    struct sockaddr_in from;
    fd_set readfds;    /* multimea descriptorilor de citire */
    fd_set actfds;     /* multimea descriptorilor activi */
    struct timeval tv; /* structura de timp pentru select() */
    int sd, client;    /* descriptori de socket */
    int optval = 1;    /* optiune folosita pentru setsockopt()*/
    int fd;            /* descriptor folosit pentru
                    parcurgerea listelor de descriptori */
    int nfds;          /* numarul maxim de descriptori */
    socklen_t len;     /* lungimea structurii sockaddr_in */
    char dArr[4];      // descriptor array
    int dArrIndex;

public:
    Server(/* args */);
    ~Server();
    int selectLoop();
    int writeToAll(char* str);
    int handleCommand(char* str);
    int sayHello(int fd);
};

char *conv_addr(struct sockaddr_in address);