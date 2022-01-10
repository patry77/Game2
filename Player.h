#pragma once
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#include "Animation.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class Player: public sf::Drawable{
public:
    Player(Texture *texture, Vector2u image_count, float switch_time);
    Player() = delete;
    ~Player() = default;
    void allow_input();
    void block_input();
    Vector2f get_position() const { return body.getPosition(); }
    void collided();
    void update(float delta_time);

private:
    RectangleShape body;
    Animation animation;
    unsigned int row;
    bool allow_player_input;
    float walk_speed { 100.0f };
    FloatRect nextPos;
    FloatRect playerBounds;
    void draw(RenderTarget &target, RenderStates state) const override;
};


#endif //GAME_PLAYER_H
