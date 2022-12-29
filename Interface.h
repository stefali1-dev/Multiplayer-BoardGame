#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include "GameBoard.h"

#define W 900
#define H 900

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
    Button();

    void create(float pos_x, float pos_y, float w, float h, sf::String text_);

    void display(sf::RenderWindow &window);

    bool isClicked(float mouse_x, float mouse_y);
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

    void updateInfo(const char* str);

    void display(sf::RenderWindow &window);
};



class Interface
{
private:
    /* data */
    int menuOrder;
    sf::RenderWindow window;

    InputBox TextInputBox;
    Button SubmitBtn;

    char playerName[50];
    char ip[50];
    char port[50];

public:
    Interface(float width, float height);

    char *getIp();

    char *getPort();

    char *getPlayerName();

    void displayFirstScreen();

    void displayGameScreen();
};
