#include "Server.h"

int main()
{
    GameBoard *gameBoard = new GameBoard();

    gameBoard->movePawn(0, 0, 6);
    
    gameBoard->movePawn(1, 0, 6);

    gameBoard->movePawn(0, 0, 10);
    gameBoard->movePawn(0, 0, 1);

    gameBoard->movePawn(1, 0, 6);
    gameBoard->movePawn(1, 0, 1);

    printf("\n A CASTIGAT %d\n", gameBoard->playerFinnished());
}