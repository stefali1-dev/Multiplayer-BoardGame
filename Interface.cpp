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

GameBoard::GameBoard(/* args */)
{
}

Interface::Interface(float width, float height) : menuOrder(0)
{
    if (!font.loadFromFile("arial.ttf"))
    {
        printf("Eroare la font");
    }

    window.create(sf::VideoMode(width, height), "SFML works!", sf::Style::Close);
    TextInputBox.create((width - 500) / 2, height / 3, 500, 65, "Nume Jucator");
    SubmitBtn.create((width - 200) / 2, height / 1.8, 200, 100, "Submit");
}

char *Interface::getPlayerName()
{
    if (playerName)
    {
        return playerName;
    }
    printf("Player Name is NULL");
    return NULL;
}

void Interface::displayFirstScreen()
{
    bool exitScreen = false;
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
                    if (SubmitBtn.isClicked(event.mouseButton.x, event.mouseButton.y))
                    {
                        std::string str = static_cast<std::string>(TextInputBox.getInput());

                        char *inputText = str.data();

                        switch (menuOrder)
                        {
                        case 0:
                            // name input
                            playerName = inputText;
                            break;
                        case 1:
                            // ip input
                            ip = inputText;
                            break;
                        case 2:
                            // ip input
                            port = inputText;
                            break;
                        }

                        std::cout << inputText << std::endl;

                        if (playerName != NULL)
                        {
                            window.clear(sf::Color(236, 233, 211));
                            TextInputBox.ereaseText();

                            switch (menuOrder)
                            {
                            case 0:
                                // name input
                                TextInputBox.updateInfo("IP Server");
                                break;
                            case 1:
                                // ip input
                                TextInputBox.updateInfo("Port Server");
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
            break;
        }

        SubmitBtn.display(window);
        TextInputBox.display(window);
        window.display();
    }
}

void Interface::displayGameScreen()
{
    bool exitScreen = false;
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
                    if (SubmitBtn.isClicked(event.mouseButton.x, event.mouseButton.y))
                    {
                        std::string str = static_cast<std::string>(TextInputBox.getInput());

                        char *inputText = str.data();

                        switch (menuOrder)
                        {
                        case 0:
                            // name input
                            playerName = inputText;
                            break;
                        case 1:
                            // ip input
                            ip = inputText;
                            break;
                        case 2:
                            // ip input
                            port = inputText;
                            break;
                        }

                        std::cout << inputText << std::endl;

                        if (playerName != NULL)
                        {
                            window.clear(sf::Color(236, 233, 211));
                            TextInputBox.ereaseText();

                            switch (menuOrder)
                            {
                            case 0:
                                // name input
                                TextInputBox.updateInfo("IP Server");
                                break;
                            case 1:
                                // ip input
                                TextInputBox.updateInfo("Port Server");
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
            break;

        SubmitBtn.display(window);
        TextInputBox.display(window);
        window.display();
    }
}

int main()
{
    Interface *interface = new Interface(900.f, 900.f);

    interface->displayFirstScreen();

    interface->displayGameScreen();

    return 0;
}