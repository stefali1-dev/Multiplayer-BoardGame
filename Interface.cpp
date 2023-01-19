#include "Interface.h"

void *read_func(void *arg) // thread function to read from server
{
    int nr;
    thData *tdL;
    tdL = ((thData *)arg);

    while (1)
    {
        if (read(tdL->sd, tdL->info, sizeof(int) * 5) < 0)
        {
            printf("Error at read() from server.\n");
            return (NULL);
        }
        // tdL->info[0] player index
        // tdL->info[1] pawn number
        // tdL->info[2] dice
        // tdL->info[3] has the player finished?
        // tdL->info[4] player turn

        // tdL->info[6] collided pawn, -1 if none
        // tdL->info[6] player of collided pawn, -1 if none

        if (tdL->info[1] != -1)
        {
            tdL->interface->gameBoard->moveAndUpdatePawn(tdL->info[0], tdL->info[1], tdL->interface->gameBoard->dice);

            if (tdL->info[5] != -1) // pawn collision
            {
                PathList *pathList = tdL->interface->gameBoard->pathList;
                tdL->interface->gameBoard->player[tdL->info[6]]->sendPawnToBase(tdL->info[5], pathList);
            }
        }

        tdL->interface->gameBoard->dice = tdL->info[2];

        char str[100];
        char nr[2];
        nr[1] = '\0';
        strcpy(str, "Player x has moved - Turn: player   -  Dice:  ");

        if (tdL->info[0] < 0 || tdL->info[0] > 3)
            str[7] = 0 + 48;

        else
            str[7] = tdL->info[0] + 48;

        str[34] = tdL->info[4] + 48;
        str[44] = tdL->info[2] + 48;

        tdL->interface->updateStatusText(str);
    }

    return (NULL);
};

void *write_func(void *arg) // thread function to write to server
{
    thData *tdL;
    tdL = ((thData *)arg);

    // trimiterea mesajului la server 
    if (write(tdL->sd, &tdL->chosen_pawn, sizeof(int)) <= 0)
    {
        printf("Error at write() from server.\n");
        return (NULL);
    }
    printf("Chosen pawn: %d\n\n\n", tdL->chosen_pawn);
    return (NULL);
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

void TextBox::initialize(sf::Text &txt, float y, sf::String str)
{
    txt.setFont(font);
    txt.setString(str);
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color::Black);

    float x = (W - txt.getLocalBounds().width) / 2;

    txt.setPosition(x, y);
}

void TextBox::setText(char *str)
{
    sf::String s = str;

    initialize(firstLine, 10.f, s);
}

void TextBox::updateText(char *str)
{
    firstLine.setString(str);
}

void TextBox::display(sf::RenderWindow &window)
{
    window.draw(firstLine);
    // window.draw(secondLine);
}

BoardDot::BoardDot(float x, float y, sf::Color color)
{
    circle.setRadius(20.f);
    circle.setFillColor(color);

    circle.setOutlineThickness(3.f);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(25.f + x, 73.f + y);
}
void BoardDot::display(sf::RenderWindow &window)
{
    window.draw(circle);
}

GraphicPawn::GraphicPawn(sf::Color color) : x(0), y(0)
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

void GraphicPawn::move(float pos_x, float pos_y)
{
    this->x = pos_x + 26.3;
    this->y = pos_y + 47.f;

    sprite.setPosition(this->x, this->y);
    rectangle.setPosition(this->x - 3, this->y + 4);
}

void GraphicPawn::display(sf::RenderWindow &window)
{
    window.draw(sprite);
}

bool GraphicPawn::isClicked(float mouse_x, float mouse_y)
{
    int rec_x = x - 3;
    int rec_y = y + 4;
    return (mouse_x >= rec_x && mouse_x <= rec_x + rectangle.getLocalBounds().width) &&
           (mouse_y >= rec_y && mouse_y <= rec_y + rectangle.getLocalBounds().height);
}

void InterfaceGameBoard::setCoord()
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

void InterfaceGameBoard::initBoard()
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

void InterfaceGameBoard::initPawns()
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

InterfaceGameBoard::InterfaceGameBoard()
{
    setCoord();
    initBoard();
    initPawns();
}

void InterfaceGameBoard::moveAndUpdatePawn(int playerIndex, int pawnNr, int dice)
{
    int k; // dummy variable
    movePawn(playerIndex, pawnNr, dice, k, k);

    int x = player[playerIndex]->getPawn(pawnNr)->x;
    int y = player[playerIndex]->getPawn(pawnNr)->y;

    graphicPawn[4 * playerIndex + pawnNr]->move(x, y);
}

void InterfaceGameBoard::display(sf::RenderWindow &window)
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

int InterfaceGameBoard::clickedPawn(int player_index, float mouse_x, float mouse_y)
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

int Client::connect_(const char *sv_adress, const char *input_port)
{
    // setting port
    port = atoi(input_port);

    // creating socket 
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Eroare la socket().\n");
        return -1;
    }

    // filling server struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(sv_adress);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        printf("Error at connect()\n");
        return -1;
    }

    return 0;
}

Interface::Interface(float width, float height) : menuOrder(0)
{
    C = new Client();

    font.loadFromFile("./src/arial.ttf");

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(width, height), "SFML works!", sf::Style::Close, settings);
    TextInputBox.create((width - 500) / 2, height / 3, 500, 65, "Server IP");
    Btn.create((width - 200) / 2, height / 1.8, 200, 100, "Submit");
    gameBoard = new InterfaceGameBoard();

    char s[40] = "First move - Turn: player 0 - Dice: 6";
    InfoText.setText(s);
}

void Interface::updateStatusText(char *str)
{
    InfoText.updateText(str);
}

void Interface::restart()
{
    delete gameBoard;

    gameBoard = new InterfaceGameBoard();

    char s[40] = "First move - Turn: player 0 - Dice: 6";
    InfoText.updateText(s);
}

int Interface::firstScreen()
{
    bool exitScreen = false;
    Btn.updateText("Submit");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    char c = static_cast<char>(event.text.unicode);

                    if (isalnum(c) || c == '.')
                    {
                        TextInputBox.addToInput(static_cast<sf::String>(event.text.unicode));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::BackSpace)
                {
                    TextInputBox.eraseLastCh();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (Btn.isClicked(event.mouseButton.x, event.mouseButton.y))
                    {
                        std::string str = static_cast<std::string>(TextInputBox.getInput());

                        char *inputText = str.data();

                        switch (menuOrder)
                        {
                        case 0:
                            // ip input
                            strcpy(ip, inputText);
                            break;
                        case 1:
                            // port input
                            strcpy(port, inputText);
                            break;
                        }

                        if (inputText != NULL)
                        {
                            window.clear(sf::Color(236, 233, 211));
                            TextInputBox.ereaseText();

                            switch (menuOrder)
                            {
                            case 0:
                                // ip input
                                TextInputBox.updateInfo("Server Port");
                                break;
                            case 1:
                                // port input
                                break;
                            }

                            TextInputBox.display(window);

                            menuOrder++;
                            if (menuOrder > 1)
                                exitScreen = true;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(236, 233, 211));

        if (exitScreen == true)
        {
            C->connect_(ip, port);
            break;
        }

        Btn.display(window);
        TextInputBox.display(window);
        window.display();
    }
    return 0;
}

void Interface::gameScreen()
{
    bool exitScreen = false;

    pthread_t th[2]; // Thread identifiers for read and write
                     // 0 for read; 1 for write

    thData *td0, *td1;

    // initializing threads data
    td0 = new thData();
    td0->sd = C->sd;
    td0->interface = this;

    td1 = new thData();
    td1->sd = C->sd;

    if (read(C->sd, &C->player_index, sizeof(int)) <= 0) // receiving player number
    {
        perror("Error at write() to server\n");
        return;
    }
    printf("YOUR PLAYER NUMBER: %d\n", C->player_index);

    switch (C->player_index)
    {
    case 0:
        printf("YOUR COLOR: BLUE\n\n");
        break;
    case 1:
        printf("YOUR COLOR: YELLOW\n\n");
        break;
    case 2:
        printf("YOUR COLOR: GREEN\n\n");
        break;
    case 3:
        printf("YOUR COLOR: RED\n\n");
        break;
    default:
        break;
    }

    pthread_create(&th[0], NULL, &read_func, td0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                close(C->sd);
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int chosen_pawn = gameBoard->clickedPawn(C->player_index, event.mouseButton.x, event.mouseButton.y);
                    printf("Dice: %d\n", gameBoard->dice);

                    if (chosen_pawn != -2)
                    {
                        if (gameBoard->hasNoValidMoves(C->player_index, gameBoard->dice))
                        {
                            td1->chosen_pawn = -1;
                        }
                        else if (gameBoard->isValidPawnMove(C->player_index, chosen_pawn, gameBoard->dice))
                        {
                            td1->chosen_pawn = chosen_pawn;
                        }
                        pthread_create(&th[1], NULL, &write_func, td1);
                    }
                }
            }
        }

        window.clear(sf::Color(236, 233, 211));

        if (exitScreen == true)
        {
            break;
        }

        InfoText.display(window);
        gameBoard->display(window);
        window.display();
    }
}

int main()
{
    Interface *interface = new Interface(W, H);

    interface->firstScreen();

    interface->gameScreen();

    return 0;
}
