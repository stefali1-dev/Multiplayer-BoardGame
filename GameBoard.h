#pragma once

#include <stdio.h>

struct PathNode
{
    float x, y;
    int nodeIndex;
    int playerIndex; // only player who can step on this node;  -1 means everyone
    bool isBase;
    PathNode *next;
    PathNode *nextFinal; // first node of final path

    // int pawnCount[4]; // cati pioni are fiecare jucator in nod
    int playerPawn; // player id that has a pawn on this node;  -1 means nobody

    PathNode();

    void initializeIndex(int player_index);

    void initializeIndex(int player_index, bool is_base);

    bool isCrossingNode();
};

class PathList
{
private:
    void initImportantPoints();

    void createPath(PathNode *cp1, PathNode *cp2, PathNode *final2);

    void createBasePath(PathNode *b0, PathNode *b1);

public:
    int nodeCount;
    PathNode *cp[4];    // checkpoints
    PathNode *base1[4]; // last node of each base
    PathNode *base0[4]; // first node of each base
    PathNode *final[4]; // first node of final paths
    int dice = 6;

    PathList();
};

class Player
{
private:
    /* data */
    int playerIndex;
    PathNode *pawn[4];

public:
    Player(int index);

    void initializePawns(PathList *pathList);
    
    void movePawn(int pawnNr, int count, PathList *pathList);

    void moveToCheckpoint(int pawnNr, PathList *pathList);

    bool isValidMove(int dice, int pawnNr, PathList *pathList);

    void sendPawnToBase(int pawnNr, PathList *pathList);

    PathNode *getPawn(int nr);
};

class GameBoard
{
private:
    bool isFinished;

public:
    int turn;
    int dice;
    PathList *pathList;
    Player *player[4];
    GameBoard();

    bool hasNoValidMoves(int player_index, int dice);

    void movePawn(int playerIndex, int pawnNr, int dice, int &pawn_colision, int &player_collision);

    bool isValidPawnMove(int playerIndex, int pawnNr, int dice);

    bool hasFinnished(int playerIndex);
    
};