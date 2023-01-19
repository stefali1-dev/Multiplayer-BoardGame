#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <vector>
#include "GameBoard.cpp"

#define W 900.f
#define H 900.f

sf::Font font;


class Button
{
private:
    sf::Color color;
    float width, height;
    float x, y;

    sf::RectangleShape btn;
    sf::Text text;

public:
    void create(float pos_x, float pos_y, float w, float h, sf::String text_);

    void display(sf::RenderWindow &window);

    bool isClicked(float mouse_x, float mouse_y);

    void updateText(const char *str);
};


class InputBox
{
private:
    sf::RectangleShape box;
    sf::String input;
    sf::Text info;
    sf::Text inputText;

public:
    InputBox();

    void create(float x, float y, float width, float height, sf::String box_text);

    sf::String getInput();

    void addToInput(sf::String ch);

    void eraseLastCh();

    void ereaseText();

    void updateInfo(const char *str);

    void display(sf::RenderWindow &window);
};


class TextBox
{
private:
    sf::Text firstLine;
    sf::Text secondLine;
    sf::String text1, text2;
    void initialize(sf::Text &txt, float y, sf::String str);

public:
    void setText(char *str);

    void updateText(char *str);

    void display(sf::RenderWindow &window);
};

class BoardDot
{
private:
    sf::CircleShape circle;

public:
    BoardDot(float x, float y, sf::Color color);
    void display(sf::RenderWindow &window);
};


class GraphicPawn
{
private:
    float x, y;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape rectangle;

public:
    GraphicPawn(sf::Color color);

    void move(float pos_x, float pos_y);

    void display(sf::RenderWindow &window);

    bool isClicked(float mouse_x, float mouse_y);
};

class InterfaceGameBoard : public GameBoard
{
private:
    std::array<BoardDot *, 40> mainPath;
    std::array<BoardDot *, 16> finalPaths;
    std::array<BoardDot *, 16> bases;

    std::array<GraphicPawn *, 16> graphicPawn;

    void setCoord();

    void initBoard();

    void initPawns();

public:
    InterfaceGameBoard();

    void moveAndUpdatePawn(int playerIndex, int pawnNr, int dice);
    
    void display(sf::RenderWindow &window);

    int clickedPawn(int player_index, float mouse_x, float mouse_y);

};


class Client
{
private:
    int port;
    // server struct
    struct sockaddr_in server; // structura folosita pentru conectare
public:
    int sd;
    int player_index;
    int connect_(const char *sv_adress, const char *input_port);
};


class Interface
{
private:
    int menuOrder;
    sf::RenderWindow window;

    InputBox TextInputBox;
    Button Btn;
    TextBox InfoText;

    Client *C;

    char ip[100];
    char port[100];

public:
    InterfaceGameBoard *gameBoard;

    Interface(float width, float height);

    void updateStatusText(char *str);

    void restart();

    int firstScreen();

    void gameScreen();
};

struct thData
{
    int sd;
    bool has_read;
    char sv_msg[100];
    int info[7];
    int chosen_pawn;
    Interface *interface;
};