#pragma once
#ifndef GAME_COMBAT_MENU_H
#define GAME_COMBAT_MENU_H
#include <SFML/Graphics.hpp>
#define MAX_NUMBER_OF_ITEMS 4


class Combat_menu {
public:
    Combat_menu(float width, float height, sf::Vector2f player_position);
    ~Combat_menu();

    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem(){return selectedItemIndex;}

    void ucieczka(sf::Vector2f player_position);

private:
    int poprawa = 0;
    int selectedItemIndex;
    sf::Font font;
    sf::Text combat_menu[MAX_NUMBER_OF_ITEMS];
};


#endif //GAME_COMBAT_MENU_H
