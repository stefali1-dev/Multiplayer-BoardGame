#include "Interface.h"

Button::Button()
{
}

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
TextBox::TextBox()
{
}

void Interface::generateBoard()
{
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
}

void Interface::firstScreen()
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
                        case 2:
                            // name input
                            strcpy(playerName, inputText);
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
                                // name input
                                TextInputBox.updateInfo("Server Port");
                                break;
                            case 1:
                                // ip input
                                TextInputBox.updateInfo("Player Name");
                                break;
                            case 2:
                                // ip input
                                break;
                            }

                            TextInputBox.display(window);

                            menuOrder++;
                            if (menuOrder > 2)
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

            C->set_cl_msg(playerName);
            C->write_(); // sending player name

            C->read_();
            C->player_index = (int)(C->get_sv_msg()[0] - 48); // saving player index

            break;
        }

        Btn.display(window);
        TextInputBox.display(window);
        window.display();
    }
}

void Interface::gameScreen()
{
    bool exitScreen = false;
    char sv_msg[100];
    // InfoText.setText(s);

    int sockets[2], child;

    socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
    child = fork();

    if (child)
    {
        // parent - interface
        while (window.isOpen())
        {
            close(sockets[0]);

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color(236, 233, 211));

            if (exitScreen == true)
            {
                break;
            }

            InfoText.display(window);
            gameBoard->display(window);
            window.display();

            // citire mesaj de la server de afisat
            read(sockets[1], sv_msg, 100);

            char last_char = sv_msg[strlen(sv_msg) - 1];
            printf("\n%c\n", last_char);

            if (last_char == '0') // informative message
                InfoText.setText(sv_msg);

            else if (sv_msg[strlen(sv_msg) - 1] == '1') // we must send a move back
            {
                InfoText.setText(sv_msg);
                int chosen_pawn = -1;

                while (chosen_pawn == -1)
                {
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                            window.close();
                        if (event.type == sf::Event::MouseButtonPressed)
                        {
                            if (event.mouseButton.button == sf::Mouse::Left)
                            {
                                // gameBoard->moveAndUpdatePawn(0, 0, 6);
                                chosen_pawn = gameBoard->clickedPawn(C->player_index, event.mouseButton.x, event.mouseButton.y);
                            }
                        }
                    }
                }

                printf("Interface: player-ul %d a ales pionul %d\n", C->player_index, chosen_pawn);

                char move[100];
                strcpy(move, std::to_string(chosen_pawn).c_str());

                write(sockets[1], move, 100);
            }
            else if (last_char == '2') // must update the board
            {
                int p_index = sv_msg[0] - 48;
                int pawn_nr = sv_msg[1] - 48;
                int dice = sv_msg[2] - 48;

                gameBoard->moveAndUpdatePawn(p_index, pawn_nr, dice);
            }
        }
    }
    else
    {
        // child - client
        while (1)
        {
            close(sockets[1]);

            if (C->read_() == -1)
            {
                printf("S-a deconectat serverul");
                break;
            }

            write(sockets[0], C->get_sv_msg(), 100); // trimite mesaj de la server pentru interfata

            if (C->must_reply) // has to select pawn to move
            {
                char interface_msg[100];
                read(sockets[0], interface_msg, 100); // asteapta sa primeasca mutare
                printf("Client: pionul mutat este %c si o trimit la server\n", interface_msg[0]);

                C->set_cl_msg(interface_msg);
                C->write_();
            }
        }
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
