#include "Combat_menu.h"

Combat_menu::Combat_menu(float width, float height, sf::Vector2f player_position)
{
    if(!font.loadFromFile("../alagard.ttf"))
    {
        //error, bo nie ma czcionki
    }

    combat_menu[0].setFont(font);
    combat_menu[0].setFillColor(sf::Color::Green);
    combat_menu[0].setString("Walcz");
    combat_menu[0].setPosition(player_position.x-512,player_position.y);

    combat_menu[1].setFont(font);
    combat_menu[1].setFillColor(sf::Color::Red);
    combat_menu[1].setString("Glwwwwosniej");
    combat_menu[1].setPosition(player_position.x-256,player_position.y);

    combat_menu[2].setFont(font);
    combat_menu[2].setFillColor(sf::Color::White);
    combat_menu[2].setString("Ciszwwwwwwej");
    combat_menu[2].setPosition(player_position.x,player_position.y);

    combat_menu[3].setFont(font);
    combat_menu[3].setFillColor(sf::Color::White);
    combat_menu[3].setString("Powrwwwwwwot");
    combat_menu[3].setPosition(player_position.x+256,player_position.y);

    selectedItemIndex = 1;
}

Combat_menu::~Combat_menu()
{}

void Combat_menu::draw(sf::RenderWindow &window)
{
    for(int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        window.draw(combat_menu[i]);
    }
}

void Combat_menu::MoveUp()
{
    if(selectedItemIndex - 1 >= 0)
    {
        combat_menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        combat_menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Combat_menu::MoveDown()
{
    if(selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
    {
        combat_menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        combat_menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}
