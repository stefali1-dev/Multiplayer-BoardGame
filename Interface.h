#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include "Client.cpp"
#include <vector>
#include "GameBoard.h"

#define W 900.f
#define H 973.f

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
    /* data */
    sf::Text firstLine;
    sf::Text secondLine;
    sf::String text1, text2;
    void initialize(sf::Text &txt, float y, sf::String str)
    {
        txt.setFont(font);
        txt.setString(str);
        txt.setCharacterSize(24);
        txt.setFillColor(sf::Color::Black);

        float x = (W - txt.getLocalBounds().width) / 2;

        txt.setPosition(x, y);
    }

public:
    TextBox(/* args */);
    void setText(char *str)
    {
        sf::String s = str;

        text1 = s.substring(0, s.find("\n"));
        text2 = s.substring(s.find("\n") + 1);

        initialize(firstLine, 10.f, text1);
        initialize(secondLine, 45.f, text2);

        // printf("%f\n", (float)firstLine.getLocalBounds().height);
        // printf("%f\n", (float)secondLine.getLocalBounds().height);

        // std::cout<< static_cast<std::string>(text1) << "\n" << static_cast<std::string>(text2);
    }
    void display(sf::RenderWindow &window)
    {
        window.draw(firstLine);
        window.draw(secondLine);
    }
};

class BoardDot
{
private:
    /* data */
    sf::CircleShape circle;

public:
    BoardDot(int x, int y, sf::Color color)
    {
        circle.setRadius(25.f);
        circle.setFillColor(color);

        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(sf::Color::Black);
        circle.setPosition(25.f + x, 73.f + y);
    }
    void display(sf::RenderWindow &window)
    {
        window.draw(circle);
    }
};

class InteraceGameBoard : public GameBoard
{
private:
    std::array<BoardDot *, 40> mainPath;
    std::array<BoardDot *, 16> finalPaths;

    void setCoord()
    {
        float nodeSize = 72; // in pixels
        int x = 4, y = 10;
        PathNode *current = pathList->cp[0];

        // coord for main path
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y--;
            current = current->next;
        }
        x = 4, y = 6;
        for (int i = 0; i < 5; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x--;
            current = current->next;
        }
        x = 0, y = 5;

        current->x = x * nodeSize + nodeSize / 2;
        current->y = y * nodeSize + nodeSize / 2;
        current = current->next;

        x = 0, y = 4;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x++;
            current = current->next;
        }

        x = 4, y = 4;
        for (int i = 0; i < 5; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y--;
            current = current->next;
        }

        x = 5, y = 0;

        current->x = x * nodeSize + nodeSize / 2;
        current->y = y * nodeSize + nodeSize / 2;
        current = current->next;

        x = 6, y = 0;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y++;
            current = current->next;
        }

        x = 6, y = 4;
        for (int i = 0; i < 5; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x++;
            current = current->next;
        }

        x = 10, y = 5;
        current->x = x * nodeSize + nodeSize / 2;
        current->y = y * nodeSize + nodeSize / 2;
        current = current->next;

        x = 10, y = 6;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x--;
            current = current->next;
        }

        x = 6, y = 6;
        for (int i = 0; i < 5; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y++;
            current = current->next;
        }

        x = 5, y = 10;
        current->x = x * nodeSize + nodeSize / 2;
        current->y = y * nodeSize + nodeSize / 2;
        current = current->next;

        // coord for final nodes
        current = pathList->final[0];
        x = 5, y = 9;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y--;
            current = current->next;
        }

        current = pathList->final[1];
        x = 1, y = 5;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x++;
            current = current->next;
        }

        current = pathList->final[0];
        x = 5, y = 1;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y++;
            current = current->next;
        }

        current = pathList->final[0];
        x = 9, y = 5;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x--;
            current = current->next;
        }
    }

    void init()
    {
        sf::Color color;

        PathNode *current = pathList->cp[0];

        // main path
        for (int i = 0; i < 40; i++)
        {
            switch (i)
            {
            case 0:
                color = sf::Color::Black;
                break;
            case 10:
                color = sf::Color::Yellow;
                break;
            case 20:
                color = sf::Color::Green;
                break;
            case 30:
                color = sf::Color::Red;
                break;
            default:
                color = sf::Color::White;
                break;
            }

            mainPath[i] = new BoardDot(current->x, current->y, color);

            current = current->next;
        }

        // final nodes 0
        int index = 0;
        current = pathList->final[0];
        for(int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color::Black);
            current = current->next;
        }

        // final nodes 1
        current = pathList->final[1];
        for(int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color::Yellow);
            current = current->next;
        }

        // final nodes 2
        current = pathList->final[2];
        for(int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color::Green);
            current = current->next;
        }

        // final nodes 3
        current = pathList->final[3];
        for(int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color::Red);
            current = current->next;
        }
    }


public:
    InteraceGameBoard(/* args */)
    {
        setCoord();
        init();
    }

    void display(sf::RenderWindow &window)
    {
        for (int i = 0; i < 40; i++)
        {
            mainPath[i]->display(window);
        }

        for (int i = 0; i < 16; i++)
        {
            finalPaths[i]->display(window);
        }
        
    }
};

class Interface
{
private:
    /* data */
    int menuOrder;
    sf::RenderWindow window;

    InputBox TextInputBox;
    Button Btn;
    TextBox InfoText;
    InteraceGameBoard* gameBoard;

    char playerName[50];
    char ip[50];
    char port[50];

    void generateBoard();

public:
    Interface(float width, float height);

    char *getIp();

    char *getPort();

    char *getPlayerName();

    void displayFirstScreen();

    void displayGameScreen();

    void displayEndScreen();
};
