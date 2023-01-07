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
    BoardDot(float x, float y, sf::Color color)
    {
        circle.setRadius(20.f);
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

class GraphicPawn
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    GraphicPawn(sf::Color color)
    {
        if (color == sf::Color::Blue)
        {
            texture.loadFromFile("./src/blue.png");
        }
        if (color == sf::Color::Yellow)
        {
            texture.loadFromFile("./src/yellow.png");
        }
        if (color == sf::Color(29, 104, 22))
        {
            texture.loadFromFile("./src/green.png");
        }
        if (color == sf::Color(180, 34, 34))
        {
            texture.loadFromFile("./src/red.png");
        }

        texture.setSmooth(true);
        sprite.setTexture(texture);
        sprite.setScale(0.06, 0.06);
    }

    void move(float x, float y)
    {
        sprite.setPosition(26.3 + x, 73.f + y - 26.f);
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }
};

class InterfaceGameBoard : public GameBoard
{
private:
    std::array<BoardDot *, 40> mainPath;
    std::array<BoardDot *, 16> finalPaths;
    std::array<BoardDot *, 16> bases;

    std::array<GraphicPawn *, 16> graphicPawn;

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

        current = pathList->final[2];
        x = 5, y = 1;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            y++;
            current = current->next;
        }

        current = pathList->final[3];
        x = 9, y = 5;
        for (int i = 0; i < 4; i++)
        {
            current->x = x * nodeSize + nodeSize / 2;
            current->y = y * nodeSize + nodeSize / 2;
            x--;
            current = current->next;
        }

        // coord for bases
        current = pathList->base0[0];
        for (x = 0; x <= 1; x++)
        {
            for (y = 10; y >= 9; y--)
            {
                current->x = x * nodeSize + nodeSize / 2;
                current->y = y * nodeSize + nodeSize / 2;
                current = current->next;
            }
        }

        current = pathList->base0[1];
        for (x = 0; x <= 1; x++)
        {
            for (y = 0; y <= 1; y++)
            {
                current->x = x * nodeSize + nodeSize / 2;
                current->y = y * nodeSize + nodeSize / 2;
                current = current->next;
            }
        }

        current = pathList->base0[2];
        for (x = 10; x >= 9; x--)
        {
            for (y = 0; y <= 1; y++)
            {
                current->x = x * nodeSize + nodeSize / 2;
                current->y = y * nodeSize + nodeSize / 2;
                current = current->next;
            }
        }

        current = pathList->base0[3];
        for (x = 10; x >= 9; x--)
        {
            for (y = 10; y >= 9; y--)
            {
                current->x = x * nodeSize + nodeSize / 2;
                current->y = y * nodeSize + nodeSize / 2;
                current = current->next;
            }
        }
    }

    void initBoard()
    {
        sf::Color color;

        PathNode *current = pathList->cp[0];

        // main path
        for (int i = 0; i < 40; i++)
        {
            switch (i)
            {
            case 0:
                color = sf::Color(9, 37, 149);
                break;
            case 10:
                color = sf::Color(210, 210, 16  );
                break;
            case 20:
                color = sf::Color(29, 104, 22);
                break;
            case 30:
                color = sf::Color(180, 34, 34);
                break;
            default:
                color = sf::Color::White;
                break;
            }

            mainPath[i] = new BoardDot(current->x, current->y, color);

            current = current->next;
        }

        // final nodes player 0
        int index = 0;
        current = pathList->final[0];
        for (int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color(9, 37, 149));
            current = current->next;
        }

        // final nodes player 1
        current = pathList->final[1];
        for (int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color(210, 210, 16  ));
            current = current->next;
        }

        // final nodes player 2
        current = pathList->final[2];
        for (int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color(29, 104, 22));
            current = current->next;
        }

        // final nodes player 3
        current = pathList->final[3];
        for (int i = 0; i < 4; i++)
        {
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color(180, 34, 34));
            current = current->next;
        }

        // base player 0
        index = 0;
        current = pathList->base0[0];
        for (int i = 0; i < 4; i++)
        {
            bases[index++] = new BoardDot(current->x, current->y, sf::Color(9, 37, 149 ));
            current = current->next;
        }

        // base player 1
        current = pathList->base0[1];
        for (int i = 0; i < 4; i++)
        {
            bases[index++] = new BoardDot(current->x, current->y, sf::Color(210, 210, 16  ));
            current = current->next;
        }

        // base player 2
        current = pathList->base0[2];
        for (int i = 0; i < 4; i++)
        {
            bases[index++] = new BoardDot(current->x, current->y, sf::Color(29, 104, 22));
            current = current->next;
        }

        // base player 3
        current = pathList->base0[3];
        for (int i = 0; i < 4; i++)
        {
            bases[index++] = new BoardDot(current->x, current->y, sf::Color(180, 34, 34));
            current = current->next;
        }
    }

    void initPawns()
    {
        PathNode *current0 = pathList->base0[0];
        PathNode *current1 = pathList->base0[1];
        PathNode *current2 = pathList->base0[2];
        PathNode *current3 = pathList->base0[3];

        for (int i = 0; i < 16; i++)
        {
            int playerIndex = i / 4;

            switch (playerIndex)
            {
            case 0:
                graphicPawn[i] = new GraphicPawn(sf::Color::Blue);
                graphicPawn[i]->move(current0->x, current0->y);

                current0 = current0->next;
                break;
            case 1:
                graphicPawn[i] = new GraphicPawn(sf::Color::Yellow);
                graphicPawn[i]->move(current1->x, current1->y);

                current1 = current1->next;
                break;
            case 2:
                graphicPawn[i] = new GraphicPawn(sf::Color(29, 104, 22));
                graphicPawn[i]->move(current2->x, current2->y);

                current2 = current2->next;
                break;
            case 3:
                graphicPawn[i] = new GraphicPawn(sf::Color(180, 34, 34));
                graphicPawn[i]->move(current3->x, current3->y);

                current3 = current3->next;
                break;
            default:
                break;
            }
        }
    }

public:
    InterfaceGameBoard(/* args */)
    {
        setCoord();
        initBoard();
        initPawns();
    }

    void moveAndUpdatePawn(int playerIndex, int pawnNr, int dice)
    {
        movePawn(playerIndex, pawnNr, dice);

        int x = player[playerIndex]->getPawn(pawnNr)->x;
        int y = player[playerIndex]->getPawn(pawnNr)->y;

        graphicPawn[4*playerIndex + pawnNr]->move(x, y);
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
            bases[i]->display(window);
        }

        for (int i = 0; i < 16; i++)
        {
            graphicPawn[i]->display(window);
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
    InterfaceGameBoard *gameBoard;

    char playerName[50];
    char ip[50];
    char port[50];

    void generateBoard();

public:
    Interface(float width, float height);

    void displayFirstScreen();

    void displayGameScreen();

    void displayEndScreen();
};
