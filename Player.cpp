#include "Player.h"
#include "NPC.h"
 Player::Player(Texture *texture, Vector2u image_count, float switch_time)
 : animation(texture, image_count, switch_time){
    row = 0;
    body.setSize(Vector2f(64.0f, 64.0f));
    body.setOrigin(body.getSize() / 2.0f);
    body.setPosition(2880.0f, 2880.0f);
    body.setTexture(texture);
}

void Player::draw(RenderTarget &target, RenderStates state) const{
    target.draw(this->body, state);
}

void Player::update(float delta_time) {
    Vector2f movement(0.0f, 0.0f);
    //kolizja lewo
    if(body.getPosition().x < 930.0f)
        this->body.setPosition(930.0f, body.getPosition().y);
    //kolizja gora
    if(body.getPosition().y < 576.0f)
        body.setPosition(body.getPosition().x, 576.0f);
    //kolizja prawo
    if(body.getPosition().x + body.getGlobalBounds().width > (5760.0f-930.0f))
        body.setPosition((5760.0f-930.0f)-body.getGlobalBounds().width, body.getPosition().y);
    //kolizja dol
    if(body.getPosition().y + body.getGlobalBounds().height > (5760.0f-576.0f))
        body.setPosition(body.getPosition().x, (5760.0f-576.0f)-body.getGlobalBounds().height);

    //poruszanie sie
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){
        movement.x -= walk_speed * delta_time;
    }
    if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
        movement.x += walk_speed * delta_time;
    }
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)){
        movement.y -= walk_speed * delta_time;
    }
    if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)){
        movement.y += walk_speed * delta_time;
    }

    if(movement.x == 0.0f && movement.y == 0.0f){
        row = 0;
    }
    //gora-lewo
    else if(movement.x < 0.0f && movement.y < 0.0f ){
        row = 1;
        animation.update(row, delta_time);
    }
    //gora-prawo
    else if(movement.x > 0.0f && movement.y < 0.0f ){
        row = 2;
        animation.update(row, delta_time);
    }
    //dol-lewo
    else if(movement.x < 0.0f && movement.y > 0.0f ){
        row = 1;
        animation.update(row, delta_time);
    }
    //dol-prawo
    else if(movement.x > 0.0f && movement.y > 0.0f ){
        row = 2;
        animation.update(row, delta_time);
    }
    //lewo
    else if(movement.x < 0.0f && movement.y == 0.0f ){
        row = 1;
        animation.update(row, delta_time);
    }
    //prawo
    else if(movement.x > 0.0f && movement.y == 0.0f ){
        row = 2;
        animation.update(row, delta_time);
    }
    //dol
    else if(movement.x == 0.0f && movement.y > 0.0f ){
        row = 0;
        animation.update(row, delta_time);
    }
    //gora
    else if(movement.x == 0.0f && movement.y < 0.0f ){
        row = 3;
        animation.update(row, delta_time);
    }

    body.setTextureRect(animation.uv_rect);
    body.move(movement);
}
void Player::allow_input() {
    allow_player_input = true;
}

void Player::block_input() {
    allow_player_input = false;
}
