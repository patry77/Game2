#include "Combat_menu.h"

Combat_menu::Combat_menu(float width, float height, sf::Vector2f player_position)
{
    if(!font.loadFromFile("../alagard.ttf"))
    {
        //error, bo nie ma czcionki
    }

    combat_menu[0].setFont(font);
    combat_menu[0].setFillColor(sf::Color::Red);
    combat_menu[0].setString("Fight");
    combat_menu[0].setPosition(player_position.x-480,player_position.y+320);

    combat_menu[1].setFont(font);
    combat_menu[1].setFillColor(sf::Color::White);
    combat_menu[1].setString("Use Item");
    combat_menu[1].setPosition(player_position.x-240,player_position.y+320);

    combat_menu[2].setFont(font);
    combat_menu[2].setFillColor(sf::Color::White);
    combat_menu[2].setString("Stats");
    combat_menu[2].setPosition(player_position.x+120,player_position.y+320);

    combat_menu[3].setFont(font);
    combat_menu[3].setFillColor(sf::Color::White);
    combat_menu[3].setString("Run");
    combat_menu[3].setPosition(player_position.x+390,player_position.y+320);

    selectedItemIndex = 0;
}

Combat_menu::~Combat_menu()
{}

void Combat_menu::ucieczka(sf::Vector2f player_position) {
    combat_menu[3].setPosition(0.0f, 0.0f);
    combat_menu[0].setPosition(player_position.x-370,player_position.y+320);
    combat_menu[1].setPosition(player_position.x-60,player_position.y+320);
    combat_menu[2].setPosition(player_position.x+310,player_position.y+320);
    combat_menu[0].setFillColor(sf::Color::Red);
    selectedItemIndex=0;
    poprawa = 1;
}

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
    if(selectedItemIndex + 1 < (MAX_NUMBER_OF_ITEMS - poprawa))
    {
        combat_menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        combat_menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}
