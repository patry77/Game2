#include "Settings_menu.h"

Settings_menu::Settings_menu(float width, float height)
{
    if(!font.loadFromFile("../alagard.ttf"))
    {
        //error, bo nie ma czcionki
    }

    settings_menu[0].setFont(font);
    settings_menu[0].setFillColor(sf::Color::Green);
    settings_menu[0].setString("Dzwiek: 10");
    settings_menu[0].setPosition(sf::Vector2f(width/2,height/(MAX_NUMBER_OF_ITEMS+1)*1));

    settings_menu[1].setFont(font);
    settings_menu[1].setFillColor(sf::Color::Red);
    settings_menu[1].setString("Glosniej");
    settings_menu[1].setPosition(sf::Vector2f(width/2,height/(MAX_NUMBER_OF_ITEMS+1)*2));

    settings_menu[2].setFont(font);
    settings_menu[2].setFillColor(sf::Color::White);
    settings_menu[2].setString("Ciszej");
    settings_menu[2].setPosition(sf::Vector2f(width/2,height/(MAX_NUMBER_OF_ITEMS+1)*3));

    settings_menu[3].setFont(font);
    settings_menu[3].setFillColor(sf::Color::White);
    settings_menu[3].setString("Powrot");
    settings_menu[3].setPosition(sf::Vector2f(width/2,height/(MAX_NUMBER_OF_ITEMS+1)*4));

    selectedItemIndex = 1;
}

Settings_menu::~Settings_menu()
{}

void Settings_menu::Change_sound_display(float volume){
    int int_volume = static_cast<int>(volume);
    sf::String string_volume=std::to_string(int_volume);
    sf::String glosnosc = "Dzwiek: " + string_volume;
    settings_menu[0].setString(glosnosc);
}

void Settings_menu::draw(sf::RenderWindow &window)
{
    for(int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        window.draw(settings_menu[i]);
    }
}

void Settings_menu::MoveUp()
{
    if(selectedItemIndex - 1 >= 1)
    {
        settings_menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        settings_menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Settings_menu::MoveDown()
{
    if(selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
    {
        settings_menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        settings_menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}