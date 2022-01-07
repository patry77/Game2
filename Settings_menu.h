#pragma once
#ifndef GAME_SETTINGS_MENU_H
#define GAME_SETTINGS_MENU_H
#include <SFML/Graphics.hpp>
#define MAX_NUMBER_OF_ITEMS 4

class Settings_menu {
public:
    Settings_menu(float width, float height);
    ~Settings_menu();

    void Change_sound_display(float volume);
    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem(){return selectedItemIndex;}
private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text settings_menu[MAX_NUMBER_OF_ITEMS];
};


#endif //GAME_SETTINGS_MENU_H
