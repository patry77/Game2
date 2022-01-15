#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H
#include <SFML/Graphics.hpp>

class Map {
public:
    Map();
    void draw(sf::RenderWindow &window);

private:
    void initIntRect();

private:
    const int tileWidth;
    const int tileHeight;

    sf::Texture tileSheet;
    sf::Sprite tile;

    sf::IntRect flor;
    sf::IntRect Border;
    sf::IntRect RightDownCorner;
    sf::IntRect RightUpCorner;
    sf::IntRect LeftBorder;
    sf::IntRect RightBorder;
    sf::IntRect LeftDownCorner;
    sf::IntRect LeftUpCorner;
    sf::IntRect KantLewy;
    sf::IntRect Top;
    sf::IntRect KantPrawy;
    sf::IntRect LeftWallUP;
    sf::IntRect MidWallUP;
    sf::IntRect RightWallUP;
    sf::IntRect LeftWallMid;
    sf::IntRect MidWallMid;
    sf::IntRect RightWallMid;
    sf::IntRect LeftWallDown;
    sf::IntRect MidWallDown;
    sf::IntRect RightWallDown;
    sf::IntRect FirstFlorOrangeWindow;
    sf::IntRect FirstFlorLeftBorder;
    sf::IntRect FirstFlorRightBorder;
    sf::IntRect FirstFlorLeftWindowBorder;
    sf::IntRect FirstFlorRightWindowBorder;
    sf::IntRect LeftPilarBorder;
    sf::IntRect RughtPilarBorder;

};


#endif //GAME_MAP_H
