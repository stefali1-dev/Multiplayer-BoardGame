#include <SFML/Graphics.hpp>
#include <iostream>

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
    Button(float pos_x, float pos_y, float w, float h, sf::String text_)
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

    void display(sf::RenderWindow &window)
    {
        window.draw(btn);
        window.draw(text);
    }

    bool isClicked(float mouse_x, float mouse_y)
    {

        return (mouse_x >= x && mouse_x <= x + width) &&
               (mouse_y >= y && mouse_y <= y + height);
    }
};

class InputBox
{
private:
    sf::RectangleShape box;
    sf::String input;
    sf::Text info;
    sf::Text inputText;

public:
    InputBox(float x, float y, float width, float height, sf::String box_text) : input("")
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

    sf::String getInput()
    {
        return input;
    }

    void addToInput(sf::String ch)
    {
        this->input += ch;
    }

    void eraseLastCh()
    {

        if (!input.isEmpty())
        {
            this->input.erase(input.getSize() - 1);
        }
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(box);
        window.draw(info);

        inputText.setString(input);
        window.draw(inputText);
    }
};

int main()
{
    if (!font.loadFromFile("arial.ttf"))
    {
        printf("Eroare la font");
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(W, H), "SFML works!", sf::Style::Close);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(10.f, 300.f);

    InputBox *MenuInputBox = new InputBox((W - 500) / 2, H / 3, 500, 65, "Nume Jucator");
    Button *MenuBtn = new Button((W - 200) / 2, H / 1.8, 200, 100, "Submit");

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
                        MenuInputBox->addToInput(static_cast<sf::String>(event.text.unicode));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::BackSpace)
                {
                    MenuInputBox->eraseLastCh();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (MenuBtn->isClicked(event.mouseButton.x, event.mouseButton.y))
                    {
                        std::string s = static_cast<std::string>(MenuInputBox->getInput());
                        const char* cstr = s.c_str();
                        std::cout << cstr << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color(236, 233, 211));

        MenuBtn->display(window);
        MenuInputBox->display(window);
        window.display();
    }

    return 0;
}