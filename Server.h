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

struct PathNode
{
    int playerIndex; // only player who can step on this node;  0 means everyone
    bool isBase;
    PathNode *next;
    PathNode *nextFinal; // first node of final path
    int pawnCount[4];    // cati pioni are fiecare jucator in nod

    PathNode() : playerIndex(0), pawnCount({0}), isBase(0)
    {
    }

    void initializeIndex(int player_index) // set the player index for the nodes where only the player can step
    {                                      // (final path nodes )
        playerIndex = player_index;
    }

    void initializeIndex(int player_index, bool is_base) // set the player index for the nodes where only the player can step
    {                                      // (base nodes)
        playerIndex = player_index;
        isBase = is_base;
    }

    bool isCrossingNode()
    {
        if (next && nextFinal)
            return true;
        return false;
    }
};

class PathList
{
private:
    void initImportantPoints()
    {
        cp[0] = new PathNode();
        cp[1] = new PathNode();
        cp[2] = new PathNode();
        cp[3] = new PathNode();

        base[0] = new PathNode();
        base[1] = new PathNode();
        base[2] = new PathNode();
        base[3] = new PathNode();

        base[0]->initializeIndex(1, true);
        base[1]->initializeIndex(2, true);
        base[2]->initializeIndex(3, true);
        base[3]->initializeIndex(4, true);

        final[0] = new PathNode();
        final[1] = new PathNode();
        final[2] = new PathNode();
        final[3] = new PathNode();

        final[0]->initializeIndex(1);
        final[1]->initializeIndex(2);
        final[2]->initializeIndex(3);
        final[3]->initializeIndex(4);

        base[0]->next = cp[0];
        base[1]->next = cp[1];
        base[2]->next = cp[2];
        base[3]->next = cp[3];
    }

    void createPath(PathNode *cp1, PathNode *cp2, PathNode *final2)
    {
        PathNode *current = cp1;

        // making a path of 9 nodes from chekcpoint 1 to checkpoint 2
        for (int i = 0; i < 9; i++)
        {
            PathNode *newNode = new PathNode();
            current->next = newNode;
            current = newNode;
        }

        current->next = cp2;
        current->nextFinal = final2;

        // making a path of 3 nodes from first node of final path
        current = final2;
        for (int i = 0; i < 3; i++)
        {
            PathNode *newNode = new PathNode();
            newNode->initializeIndex(cp2->playerIndex);

            current->next = newNode;
            current = current->next;
        }
    }

public:
    PathNode *cp[4];    // checkpoints
    PathNode *base[4];  // bases
    PathNode *final[4]; // first node of final paths

    PathList()
    {
        initImportantPoints();

        createPath(cp[0], cp[1], final[1]);
        createPath(cp[1], cp[2], final[2]);
        createPath(cp[2], cp[3], final[3]);
        createPath(cp[3], cp[0], final[0]);
    }
};

class Player
{
private:
    /* data */
    int playerIndex;
    PathNode *pawn[4];

public:
    Player(int index) : playerIndex(index)
    {
    }

    void initializePawns(PathList *pathList)
    {
        // initializing payer pawns
        for (int i = 0; i < 4; i++)
        {
            pawn[i] = pathList->base[i];
        }
    }

    void movePawn(int pawnNr, int count)
    {
        pawn[pawnNr]->pawnCount[playerIndex]--;

        while (count)
        {
            if (pawn[pawnNr]->isCrossingNode() && pawn[pawnNr]->playerIndex == playerIndex)
            {
                pawn[pawnNr] = pawn[pawnNr]->nextFinal;
            }
            else
            {
                if (pawn[pawnNr]->next == nullptr)
                {
                    printf("Eroare, nu poti muta pionul ala");
                }
                else
                {
                    pawn[pawnNr] = pawn[pawnNr]->next;
                }
            }
            count--;
        }

        pawn[pawnNr]->pawnCount[playerIndex]++;
    }

    void sendPawnToBase(int pawnNr, PathList *pathList)
    {
        pawn[pawnNr]->pawnCount[playerIndex]--;

        pawn[pawnNr] = pathList->base[playerIndex];
        pawn[pawnNr]->pawnCount[playerIndex]++;
    }

    void sendPawnToCheckpoint(int pawnNr)
    {
        pawn[pawnNr] = pawn[pawnNr]->next;
    }
};

class GameBoard
{
private:
    PathList *pathList;
    Player *player[4];
    bool isFinished;
    bool allConnected;
    int turn;

public:
    GameBoard(/* args */) : isFinished(0), allConnected(0), turn(1)
    {
        pathList = new PathList();

        for (int i = 0; i < 4; i++)
        {
            player[i] = new Player(i + 1);
            player[i]->initializePawns(pathList);
        }
    }
    ~GameBoard()
    {
    }

    void movePawn(int index, int pawnNr, int dice)
    {
        ///////////////// AICI ERAM
        ///////////////// TRE SA FAC SA MUTE SI SA IASA DIN BAZA
    }
};

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

    // game related
    GameBoard *gameBoard;

public:
    Server(/* args */);
    ~Server();
    int selectLoop();
    int writeToAll(char *str);
    int handleCommand(char *str);
    int sayHello(int fd);
};

char *conv_addr(struct sockaddr_in address);