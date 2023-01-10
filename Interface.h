#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include "Client.cpp"
#include <vector>
#include "GameBoard.h"

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

void Button::create(float pos_x, float pos_y, float w, float h, sf::String text_)
{
    width = w;
    height = h;
    x = pos_x;
    y = pos_y;

    btn.setSize(sf::Vector2f(width, height));
    btn.setPosition(x, y);
    btn.setFillColor(color);

    text.setFont(font);
    text.setString(text_);
    text.setCharacterSize(22);

    float t_x = x + (width - text.getLocalBounds().width) / 2;
    float t_y = y + (height - text.getLocalBounds().height) / 2;
    text.setPosition(t_x, t_y);
}

void Button::display(sf::RenderWindow &window)
{
    window.draw(btn);
    window.draw(text);
}

bool Button::isClicked(float mouse_x, float mouse_y)
{

    return (mouse_x >= x && mouse_x <= x + width) &&
           (mouse_y >= y && mouse_y <= y + height);
}

void Button::updateText(const char *str)
{
    text.setString(str);
}

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

InputBox::InputBox() : input("")
{
}

void InputBox::create(float x, float y, float width, float height, sf::String box_text)
{
    box.setSize(sf::Vector2f(width, height));
    box.setPosition(x, y);

    box.setFillColor(sf::Color(236, 233, 211));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::Black);

    info.setFont(font);
    info.setString(box_text);
    info.setCharacterSize(28);
    info.setFillColor(sf::Color::Black);

    float t_x = x + (width - info.getLocalBounds().width) / 2;
    float t_y = y + (height - info.getLocalBounds().height) / 2 - 90;
    info.setPosition(t_x, t_y);

    inputText.setFont(font);
    inputText.setString(input);
    inputText.setCharacterSize(28);
    inputText.setFillColor(sf::Color::Black);

    t_x = x + 5;
    t_y = y + height / 2 - 20;
    inputText.setPosition(t_x, t_y);
}

sf::String InputBox::getInput()
{
    return input;
}

void InputBox::addToInput(sf::String ch)
{
    if (inputText.getLocalBounds().left + inputText.getLocalBounds().width <
        box.getLocalBounds().left + box.getLocalBounds().width - 25)
        this->input += ch;
}

void InputBox::eraseLastCh()
{

    if (!input.isEmpty())
    {
        this->input.erase(input.getSize() - 1);
    }
}

void InputBox::ereaseText()
{
    this->input = "";
}

void InputBox::updateInfo(const char *str)
{
    this->info.setString(str);
}

void InputBox::display(sf::RenderWindow &window)
{
    window.draw(box);
    window.draw(info);

    inputText.setString(input);
    window.draw(inputText);
}

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
    void setText(char *str)
    {
        sf::String s = str;

        initialize(firstLine, 10.f, s);
    }

    void updateText(char *str){
        firstLine.setString(str);
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(firstLine);
        //window.draw(secondLine);
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
    float x, y;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape rectangle;

public:
    GraphicPawn(sf::Color color) : x(0), y(0)
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

        rectangle.setSize({0.06f * sprite.getLocalBounds().width, 0.06f * sprite.getLocalBounds().height});
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setOutlineThickness(1);
    }

    void move(float pos_x, float pos_y)
    {
        this->x = pos_x + 26.3;
        this->y = pos_y + 47.f;

        sprite.setPosition(this->x, this->y);
        rectangle.setPosition(this->x-3, this->y+4);
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }

    bool isClicked(float mouse_x, float mouse_y)
    {
        int rec_x = x - 3;
        int rec_y = y + 4;
        return (mouse_x >= rec_x && mouse_x <= rec_x + rectangle.getLocalBounds().width) &&
               (mouse_y >= rec_y && mouse_y <= rec_y + rectangle.getLocalBounds().height);
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
                color = sf::Color(210, 210, 16);
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
            finalPaths[index++] = new BoardDot(current->x, current->y, sf::Color(210, 210, 16));
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
            bases[index++] = new BoardDot(current->x, current->y, sf::Color(9, 37, 149));
            current = current->next;
        }

        // base player 1
        current = pathList->base0[1];
        for (int i = 0; i < 4; i++)
        {
            bases[index++] = new BoardDot(current->x, current->y, sf::Color(210, 210, 16));
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
        int k; // dummy variable
        movePawn(playerIndex, pawnNr, dice, k, k);

        int x = player[playerIndex]->getPawn(pawnNr)->x;
        int y = player[playerIndex]->getPawn(pawnNr)->y;

        graphicPawn[4 * playerIndex + pawnNr]->move(x, y);
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

    int clickedPawn(int player_index, float mouse_x, float mouse_y)
    {
        for (int i = 0; i < 4; i++)
        {
            if (graphicPawn[player_index * 4 + i]->isClicked(mouse_x, mouse_y))
            {
                return i;
            }
        }
        return -2;
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

    Client *C;

    char ip[100];
    char port[100];


public:
    InterfaceGameBoard *gameBoard;

    Interface(float width, float height);

    void updateStatusText(char *str)
    {
        InfoText.updateText(str);
    }

    void restart()
    {
        delete gameBoard;

        gameBoard = new InterfaceGameBoard();

        char s[40] = "First move - Turn: player 0 - Dice: 6";
        InfoText.updateText(s);
    }

    int firstScreen();

    void gameScreen();

    void displayEndScreen();
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