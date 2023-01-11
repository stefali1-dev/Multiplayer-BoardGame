#include "GameBoard.h"

PathNode::PathNode() : nodeIndex(-1), playerIndex(-1), playerPawn(-1), isBase(0)
{
}

void PathNode::initializeIndex(int player_index) // set the player index for the nodes where only the player can step
{                                                // (final path nodes )
    playerIndex = player_index;
}

void PathNode::initializeIndex(int player_index, bool is_base) // set the player index for the nodes where only the player can step
{                                                              // (base nodes)
    playerIndex = player_index;
    isBase = is_base;
}

bool PathNode::isCrossingNode()
{
    if (nextFinal != nullptr)
        return true;
    return false;
}

void PathList::initImportantPoints()
{
    cp[0] = new PathNode();
    cp[1] = new PathNode();
    cp[2] = new PathNode();
    cp[3] = new PathNode();

    base1[0] = new PathNode();
    base1[1] = new PathNode();
    base1[2] = new PathNode();
    base1[3] = new PathNode();

    base0[0] = new PathNode();
    base0[1] = new PathNode();
    base0[2] = new PathNode();
    base0[3] = new PathNode();

    base1[0]->initializeIndex(0, true);
    base1[1]->initializeIndex(1, true);
    base1[2]->initializeIndex(2, true);
    base1[3]->initializeIndex(3, true);

    base0[0]->initializeIndex(0, true);
    base0[1]->initializeIndex(1, true);
    base0[2]->initializeIndex(2, true);
    base0[3]->initializeIndex(3, true);

    final[0] = new PathNode();
    final[1] = new PathNode();
    final[2] = new PathNode();
    final[3] = new PathNode();

    final[0]->initializeIndex(0);
    final[1]->initializeIndex(1);
    final[2]->initializeIndex(2);
    final[3]->initializeIndex(3);

    base1[0]->next = cp[0];
    base1[1]->next = cp[1];
    base1[2]->next = cp[2];
    base1[3]->next = cp[3];
}

void PathList::createPath(PathNode *cp1, PathNode *cp2, PathNode *final2)
{
    PathNode *current = cp1;
    cp1->nodeIndex = nodeCount++;

    // making a path of 9 nodes from chekcpoint 1 to checkpoint 2
    for (int i = 0; i < 9; i++)
    {
        PathNode *newNode = new PathNode();
        current->next = newNode;
        current = newNode;
        current->nodeIndex = nodeCount++;
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

void PathList::createBasePath(PathNode *b0, PathNode *b1)
{
    PathNode *current = b0;

    for (int i = 0; i < 2; i++)
    {
        PathNode *newNode = new PathNode();
        newNode->initializeIndex(b0->playerIndex, true);

        current->next = newNode;
        current = current->next;
    }

    current->next = b1;
}

PathList::PathList() : nodeCount(0)
{
    initImportantPoints();

    createPath(cp[0], cp[1], final[1]);
    createPath(cp[1], cp[2], final[2]);
    createPath(cp[2], cp[3], final[3]);
    createPath(cp[3], cp[0], final[0]);

    for (int i = 0; i < 4; i++)
    {
        createBasePath(base0[i], base1[i]);
    }
}

Player::Player(int index) : playerIndex(index)
{
}

void Player::initializePawns(PathList *pathList)
{
    // initializing payer pawns
    PathNode *current = pathList->base0[playerIndex];
    for (int i = 0; i < 4; i++)
    {
        // pawn[i]->pawnCount[i]++;

        pawn[i] = current;
        pawn[i]->playerPawn = playerIndex;
        current = current->next;
    }
}

void Player::movePawn(int pawnNr, int count, PathList *pathList)
{
    // pawn[pawnNr]->pawnCount[playerIndex]--;
    pawn[pawnNr]->playerPawn = -1;

    while (count)
    {

        if (pawn[pawnNr]->nextFinal == pathList->final[playerIndex])
        {
            //printf("Pionul %d a ajuns la final nodes\n", pawnNr);
            pawn[pawnNr] = pawn[pawnNr]->nextFinal;
        }
        else
        {
            pawn[pawnNr] = pawn[pawnNr]->next;
        }

        count--;
    }

    //printf("Am terminat de mutat pionul %d al playerului %d\n", pawnNr, playerIndex);

    // pawn[pawnNr]->pawnCount[playerIndex]++;
    pawn[pawnNr]->playerPawn = playerIndex;
}

void Player::moveToCheckpoint(int pawnNr, PathList *pathList)
{
    pawn[pawnNr]->playerPawn = -1;
    pawn[pawnNr] = pathList->cp[playerIndex];
    pawn[pawnNr]->playerPawn = playerIndex;
}

bool Player::isValidMove(int dice, int pawnNr, PathList *pathList)
{
    // when pawn is in base
    if (pawn[pawnNr]->isBase)
    {
        if (dice < 6)
        {
            // printf("Iti trebuie 6 sa iesi din baza\n");
            return false;
        }

        if (dice == 6)
        {
            if (pathList->cp[playerIndex]->playerPawn == playerIndex)
            {
                printf("player already has a pawn in checkpoint\n");
                return false;
            }

            else
            {
                return true;
            }
        }
    }

    // when pawn is outside base

    bool isValid = true;
    int count = dice;
    PathNode *current = pawn[pawnNr];

    while (count)
    {
        if (current == nullptr)
        {
            printf("mutare invalida\n");
            return false;
        }
        if (current->nextFinal == pathList->final[playerIndex])
        {
            current = current->nextFinal;
        }

        else
            current = current->next;

        count--;
    }
    if (isValid == true)
    {
        if (current->playerPawn == playerIndex) // player already has a pawn there
            isValid = false;
    }
    // printf("pion %d e valid? %d\n", pawnNr, isValid);

    return isValid;
}

void Player::sendPawnToBase(int pawnNr, PathList *pathList)
{
    // pawn[pawnNr]->pawnCount[playerIndex]--;
    pawn[pawnNr]->playerPawn = -1;

    PathNode *current = pathList->base0[playerIndex];
    for (int i = 0; i < 4; i++)
    {
        if (current->playerPawn == -1)
        {
            pawn[pawnNr] = current;
            pawn[pawnNr]->playerPawn = playerIndex;
            break;
        }
    }
}

PathNode *Player::getPawn(int nr)
{
    return pawn[nr];
}

GameBoard::GameBoard(/* args */) : isFinished(0), allConnected(0), turn(0), dice(6)
{
    pathList = new PathList();

    for (int i = 0; i < 4; i++)
    {
        player[i] = new Player(i);
        player[i]->initializePawns(pathList);
    }
}

bool GameBoard::hasNoValidMoves(int player_index, int dice)
{
    for (int i = 0; i < 4; i++)
    {
        if (isValidPawnMove(player_index, i, dice) == true)
            return false;
    }
    return true;
}

void GameBoard::movePawn(int playerIndex, int pawnNr, int dice, int &pawn_colision, int &player_collision)
{
    if (player[playerIndex]->getPawn(pawnNr)->isBase)
    {
        if (dice < 6)
        {
            // printf("Trebuie un 6 ca sa iesi din baza\n");
            return;
        }
        else
        {
            if (player[playerIndex]->isValidMove(dice, pawnNr, pathList))
            {
                player[playerIndex]->moveToCheckpoint(pawnNr, pathList);
            }
            else
                printf("Nu poti muta pionul ala\n");
        }
    }
    else
    {
        if (player[playerIndex]->isValidMove(dice, pawnNr, pathList))
        {
            player[playerIndex]->movePawn(pawnNr, dice, pathList);
        }
    }

    for (int i = 0; i < 4; i++) // iterate through players
    {

        if (i != playerIndex)
        {
            for (int j = 0; j < 4; j++) // iterate through pawns
            {

                if (player[i]->getPawn(j) == player[playerIndex]->getPawn(pawnNr)) // is pawn collision
                {
                    player_collision = i;
                    pawn_colision = j;

                    player[i]->sendPawnToBase(j, pathList);
                    printf("Am trimis la baza pionul %d al playerului %d\n", j, i);
                }
            }
        }
        else
        {
            //printf("player %d, pion %d, pozitia %d, baza? %d\n", i, 0, player[playerIndex]->getPawn(0)->nodeIndex, player[playerIndex]->getPawn(0)->isBase);
            //printf("player %d, pion %d, pozitia %d, baza? %d\n", i, 1, player[playerIndex]->getPawn(1)->nodeIndex, player[playerIndex]->getPawn(1)->isBase);
            //printf("player %d, pion %d, pozitia %d, baza? %d\n", i, 2, player[playerIndex]->getPawn(2)->nodeIndex, player[playerIndex]->getPawn(2)->isBase);
            //printf("player %d, pion %d, pozitia %d, baza? %d\n", i, 3, player[playerIndex]->getPawn(3)->nodeIndex, player[playerIndex]->getPawn(3)->isBase);
        }
    }
}

bool GameBoard::isValidPawnMove(int playerIndex, int pawnNr, int dice)
{
    return player[playerIndex]->isValidMove(dice, pawnNr, pathList);
}

bool GameBoard::hasFinnished(int playerIndex)
{
    PathNode *current = pathList->final[playerIndex];
    while (current != nullptr)
    {
        if (current->playerIndex == -1)
            return false;

        current = current->next;
    }

    return true; // player[playerIndex] has won
}