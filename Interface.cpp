#include "Interface.h"

Interface::Interface(float width, float height)
{
    if (!font.loadFromFile("arial.ttf"))
    {
        printf("Eroare la font");
    }

    window.create(sf::VideoMode(width, height), "SFML works!", sf::Style::Close);
    MenuInputBox.create((width - 500) / 2, height / 3, 500, 65, "Nume Jucator");
    MenuBtn.create((width - 200) / 2, height / 1.8, 200, 100, "Submit");
}

void Interface::display()
{

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

                    if (isalnum(c))
                    {
                        MenuInputBox.addToInput(static_cast<sf::String>(event.text.unicode));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::BackSpace)
                {
                    MenuInputBox.eraseLastCh();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (MenuBtn.isClicked(event.mouseButton.x, event.mouseButton.y))
                    {
                        std::string str = static_cast<std::string>(MenuInputBox.getInput());

                        playerName = str.data();

                        std::cout << getPlayerName() << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color(236, 233, 211));

        MenuBtn.display(window);
        MenuInputBox.display(window);
        window.display();
    }
}

int main()
{
    Interface* interface = new Interface(900.f, 900.f);

    interface->display();

    return 0;
}