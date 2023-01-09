#include "Interface.h"

void *read_func(void *arg) // FUNCTIA THREAD DE CITIRE MESAJ SERVER
{
    pthread_mutex_lock(&lock);
    int nr;
    thData *tdL;
    tdL = ((thData *)arg);

    if (read(tdL->sd, tdL->sv_msg, 100) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
    }

    /* afisam mesajul primit */
    printf("[client]Mesajul primit este: %s\n\n\n", tdL->sv_msg);
    tdL->has_read = true;

    pthread_mutex_unlock(&lock);
    return (NULL);
};

void *write_func(void *arg) // FUNCTIA THREAD DE TRIMITERE MESAJ CATRE SERVER
{
    pthread_mutex_lock(&lock);
    thData *tdL;
    tdL = ((thData *)arg);

    /* trimiterea mesajului la server */
    if (write(tdL->sd, &tdL->chosen_pawn, sizeof(int)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return (NULL);
    }
    printf("Pionul ales este: %d\n\n\n", tdL->chosen_pawn);
    pthread_mutex_unlock(&lock);
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

Interface::~Interface()
{
}

/*
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

            //C->set_cl_msg(playerName);
            //C->write_(); // sending player name

            
            pthread_create(&C->th[0], NULL, &read_func, C->read_td);

            // waiting to read player index from server
            pthread_join(C->th[0], NULL);

            C->player_index = (int)(C->read_td->sv_msg[0] - 48); // saving player index
            printf("Player index: %d\n", C->player_index);
            return (C->player_index);
            break;
        }

        Btn.display(window);
        TextInputBox.display(window);
        window.display();
    }
    return 0;
}
*/

void Interface::gameScreen()
{
    bool exitScreen = false;
    // char s[30] = "INFO TEXT\nSECOND LINE";
    // InfoText.setText(s);

    // pthread_create(&C->th[0], NULL, &read_func, C->read_td);

    strcpy(ip, "0");
    strcpy(port, "2908");

    C->connect_(ip, port);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int chosen_pawn = gameBoard->clickedPawn(C->player_index, event.mouseButton.x, event.mouseButton.y);
                    if(chosen_pawn != -1)
                    {
                        C->td0->chosen_pawn = chosen_pawn;
                        printf("pawn: %d\n", chosen_pawn);
                        pthread_create(&C->th[1], NULL, &write_func, C->td0);
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

        /*if (C->read_td->has_read)
        {
            C->read_td->has_read = false;

            char msg[100];
            strcpy(msg, C->read_td->sv_msg);

            char last_char = msg[strlen(msg) - 1];

            if (last_char == '0')
            {
                InfoText.setText(msg);
            }
            if (last_char == '1')
            {
                InfoText.setText(msg);
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

                strcpy(C->write_td->cl_msg, std::to_string(chosen_pawn).c_str()); // stocam mesajul de trimis

                // sending the move to the server with a thread
                pthread_create(&C->th[1], NULL, &write_func, C->write_td);
            }

            if(last_char == '2'){
            // must update the board
            int p_index = msg[0] - 48;
            int pawn_nr = msg[1] - 48;
            int dice = msg[2] - 48;

            gameBoard->moveAndUpdatePawn(p_index, pawn_nr, dice);
            }

            pthread_create(&C->th[0], NULL, &read_func, C->read_td); // INCEPE IAR LISTEN-UL PENTRU CITIRE
        }*/
        

    }
}


void Interface::displayEndScreen()
{
    Btn.updateText("Restart");
}

int main()
{
    Interface *interface = new Interface(900.f, 900.f);

    //nterface->firstScreen();

    interface->gameScreen();

    return 0;
}
