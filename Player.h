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
    void set_position(float pos_x, float pos_y) { body.setPosition(pos_x, pos_y); } //kiedyś może się przyda
    void collided();
    void update(float delta_time);
    RectangleShape get_body() const { return body;}
    void set_walkspeed(float speed) { walk_speed = speed;}
    float get_walkspeed(){return walk_speed;}
    Vector2f get_movementVelocity(){return movement;}
    void set_movement(Vector2f setMovement){movement=setMovement;}


private:
    Vector2f movement;
    RectangleShape body;
    Animation animation;
    unsigned int row;
    bool allow_player_input;
    float walk_speed { 600.0f };
    FloatRect nextPos;
    FloatRect playerBounds;
    void draw(RenderTarget &target, RenderStates state) const override;
};


#endif //GAME_PLAYER_H
