#include "Interface.h"

void *read_func(void *arg) // FUNCTIA THREAD DE CITIRE MESAJ SERVER
{
    int nr;
    thData *tdL;
    tdL = ((thData *)arg);

    while (1)
    {
        if (read(tdL->sd, tdL->info, sizeof(int) * 5) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
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

        if(tdL->info[0] < 0 || tdL->info[0] > 3)
            str[7] = 0 + 48;

        else
            str[7] = tdL->info[0] + 48;

        str[34] = tdL->info[4] + 48;
        str[44] = tdL->info[2] + 48;

        tdL->interface->updateStatusText(str);
    }

    return (NULL);
};

void *write_func(void *arg) // FUNCTIA THREAD DE TRIMITERE MESAJ CATRE SERVER
{
    thData *tdL;
    tdL = ((thData *)arg);

    /* trimiterea mesajului la server */
    if (write(tdL->sd, &tdL->chosen_pawn, sizeof(int)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return (NULL);
    }
    printf("Pionul ales este: %d\n\n\n", tdL->chosen_pawn);
    return (NULL);
};


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

                        // std::cout << inputText << std::endl;

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

    pthread_t th[2]; // Identificatorii thread-urilor de read si write
                     // 0 pentru citit; 1 pentru scris

    thData *td0, *td1;

    // initializing threads data
    td0 = new thData();
    td0->sd = C->sd;
    td0->interface = this;

    td1 = new thData();
    td1->sd = C->sd;

    if (read(C->sd, &C->player_index, sizeof(int)) <= 0) // primim numarul jucatorului
    {
        perror("[client]Eroare la write() spre server.\n");
        return;
    }
    printf("PLAYER NUMBER: %d\n", C->player_index);

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

void Interface::displayEndScreen()
{
    Btn.updateText("Restart");
}

int main()
{
    Interface *interface = new Interface(900.f, 900.f);

    interface->firstScreen();

    interface->gameScreen();

    return 0;
}
