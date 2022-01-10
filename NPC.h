#pragma once
#ifndef GAME_NPC_H
#define GAME_NPC_H
#include "Animation.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class NPC: public sf::Drawable {
public:
    NPC(Texture *texture, float pos_x, float pos_y);
    ~NPC() = default;
    FloatRect npcBounds;
    float pos_x=0.0f;
    float pos_y=0.0f;

    Vector2f get_position() const { return body_npc.getPosition(); }
    void set_position() { body_npc.setPosition(pos_x, pos_y); }

private:
    RectangleShape body_npc;

    void draw(RenderTarget &target, RenderStates state) const override;
};


#endif //GAME_NPC_H
