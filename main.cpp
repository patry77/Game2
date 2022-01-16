/*
 * Mateusz Lugowski
 * Patryk Kucek
 * Oskar Maciaszek
*/
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <limits>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Map.h"
#include "Menu.h"
#include "Settings_menu.h"
#include "Combat_menu.h"
#include "NPC.h"

using namespace sf;
using namespace std;
int player_lvl=1;
Vector2u view_size(1000, 1000);
bool music_play=false;
void resize_view(const RenderWindow &window, View &view);
bool collision_detection(int enemy_count, vector<NPC> &enemy, Player &player_test);
void ucieczka_func(Text ucieczka, Vector2f player_test, Font font, RenderWindow& window);
void stats_func(Text stats, Vector2f player_test, Font font, RenderWindow& window, vector <int> states);
vector <int> fight_func_logic(int quantity_ststs, vector <int> states, vector <int> stats_after_item);
void fight_func_draw(Vector2f player_test, Font font, RenderWindow& window);
vector <int> use_item_func(int quantity_ststs);
void dialog_box(RenderWindow& window, Font font, string text, Sprite dialog_box, Vector2f player_position);


int main() {
    bool dialog=false;
    //Muzyka
    float volume = 10.0f;
    sf::Music music;
    music.setVolume(volume);
    if (!music.openFromFile("../muzyka.wav"))
        return -1;

    sf::Music battle_music;
    battle_music.setVolume(volume);
    if(!battle_music.openFromFile("../muzyka.wav"))
        return -1;

    //Czcionka
    sf::Font font;
    if (!font.loadFromFile("../alagard.ttf")) {
        cerr << "błąd" << endl;
    }
    RenderWindow window {VideoMode(view_size.x, view_size.y),"Game"};
    window.setFramerateLimit(60);

    //Menu
    Menu menu(window.getSize().x, window.getSize().y);
    Settings_menu settings_menu(window.getSize().x, window.getSize().y);

    //vector statystyk
    int quantity_ststs=4;
    vector <int> states (quantity_ststs);
    vector <int> stats_after_item (quantity_ststs);
    states.at(0)=100;
    states.at(1)=20;
    states.at(2)=100;
    states.at(3)=20;

    //Gracz
    Texture player_texture;
    player_texture.loadFromFile("../Grafika/body_move.png");
    Player player_test(&player_texture, Vector2u(4,4), 0.3f);
    FloatRect playerBounds;
//    FloatRect nextPos;
    //NPC
    Texture NPC_texture;
    NPC_texture.loadFromFile("../walka_op.png");
    NPC npc2(&NPC_texture,2800.0f,2400.0f);
    NPC npc3(&NPC_texture,2900.0f,2400.0f);
    NPC npc4(&NPC_texture,3000.0f,2800.0f);
    NPC npc5(&NPC_texture,2800.0f,3000.0f);
    NPC npc6(&NPC_texture,2800.0f,2960.0f);


    RectangleShape nextBox;
    nextBox.setSize(Vector2f(player_test.get_body().getSize().x, player_test.get_body().getSize().y));
    nextBox.setFillColor(Color::Transparent);
    nextBox.setOutlineColor(Color::Red);
    nextBox.setOutlineThickness(1.f);
    nextBox.setPosition(player_test.get_position().x, player_test.get_position().y);



    //Enemies
    Texture Enemy_texture;
    Enemy_texture.loadFromFile("../Grafika/npc1.png");
    int enemy_count = 100;
    vector<NPC> enemy;
    srand( time( NULL ) );
    for(int i =0; i<enemy_count; i++){ //losowanie pozycji w rozmiarach granic
        float pos_x = (( rand() % (4799-1100) ) + 970);
        float pos_y = (( rand() % (5184-700) ) + 580);

        NPC temp_enemy {&Enemy_texture, pos_x, pos_y};
        enemy.push_back(temp_enemy);
    }

    //kamera - widok, zeby dobrze scalowal‚ obiekty przy zmianie rozmiaru okna i kamera przesuwala sie z postacia
    View view(FloatRect (0.0f, 0.0f, view_size.x, view_size.y));

    //mapa
    Map map;
    //Sukiennice co mi się nie chce wrzucić na mapę
    Texture sukiennice_text;
    sukiennice_text.loadFromFile("../Grafika/Sukiennice.png");
    Sprite sukiennice;
    sukiennice.setTexture(sukiennice_text);
    sukiennice.setPosition(64*35, 64*40);

//    RectangleShape sukieBox;
//    sukieBox.setSize(Vector2f(sukiennice_text.getSize().x, sukiennice_text.getSize().y));
//    sukieBox.setFillColor(Color::Transparent);
//    sukieBox.setOutlineColor(Color::Red);
//    sukieBox.setOutlineThickness(3.f);
//    sukieBox.setPosition(sukiennice.getPosition().x, sukiennice.getPosition().y);

    Texture uczelnia_text;
    uczelnia_text.loadFromFile("../Grafika/Test.png");
    Sprite uczelnia;
    uczelnia.setTexture(uczelnia_text);
    uczelnia.setPosition(64*55, 64*7);

    Texture uczelnia_2_text;
    uczelnia_2_text.loadFromFile("../Grafika/Uczelnia_2.png");
    Sprite uczelnia_2;
    uczelnia_2.setTexture(uczelnia_2_text);
    uczelnia_2.setPosition(64*20, 64*55);

    Texture uczelnia_3_text;
    uczelnia_3_text.loadFromFile("../Grafika/Uczelnia_3.png");
    Sprite uczelnia_3;
    uczelnia_3.setTexture(uczelnia_3_text);
    uczelnia_3.setPosition(64*60, 64*67);

    //granice
    std::vector<RectangleShape> Walls;

    RectangleShape wall;
    wall.setFillColor(Color::Transparent);
    wall.setSize(Vector2f(sukiennice_text.getSize().x-20,sukiennice_text.getSize().y-(192)));
    wall.setOrigin(wall.getSize().x/2, wall.getSize().y/4);
    wall.setPosition(sukiennice.getPosition().x+(sukiennice_text.getSize().x/2), sukiennice.getPosition().y+(sukiennice_text.getSize().y/2));

    RectangleShape wall_2;
    wall_2.setFillColor(Color::Transparent);
    wall_2.setSize(Vector2f(uczelnia_text.getSize().x-84,uczelnia_text.getSize().y-(250)));
    wall_2.setOrigin(wall_2.getSize().x/2, wall_2.getSize().y/4);
    wall_2.setPosition(uczelnia.getPosition().x+(uczelnia_text.getSize().x/2), uczelnia.getPosition().y+(uczelnia_text.getSize().y/2));

    RectangleShape wall_3;
    wall_3.setFillColor(Color::Transparent);
    wall_3.setSize(Vector2f(uczelnia_2_text.getSize().x-180,uczelnia_2_text.getSize().y-(190)));
    wall_3.setOrigin((wall_3.getSize().x/2), wall_3.getSize().y/4);
    wall_3.setPosition(uczelnia_2.getPosition().x+(uczelnia_2_text.getSize().x/2), uczelnia_2.getPosition().y+(uczelnia_2_text.getSize().y/2));

    RectangleShape wall_4;
    wall_4.setFillColor(Color::Transparent);
    wall_4.setSize(Vector2f(uczelnia_3_text.getSize().x-120,uczelnia_3_text.getSize().y-(230)));
    wall_4.setOrigin(wall_4.getSize().x/2, wall_4.getSize().y/4);
    wall_4.setPosition(uczelnia_3.getPosition().x+(uczelnia_3_text.getSize().x/2), uczelnia_3.getPosition().y+(uczelnia_3_text.getSize().y/2));

    Walls.push_back(wall);
    Walls.push_back(wall_2);
    Walls.push_back(wall_3);
    Walls.push_back(wall_4);
    //dialog box
    RectangleShape dialogbox;
    dialogbox.setSize(Vector2f(900, 200));
    dialogbox.setFillColor(Color::Black);
    dialogbox.setOutlineColor(Color::White);
    dialogbox.setOutlineThickness(5.f);
    dialogbox.setPosition(player_test.get_position().x-450, player_test.get_position().y+200);

    //odswiezanie do animacji
    float delta_time = 0.0f;
    Clock clock;

    while(window.isOpen()){
        delta_time = clock.restart().asSeconds();
        Event ev{};
        //MENU
        while(window.pollEvent(ev)){
            switch (ev.type){
                case Event::KeyReleased:
                    switch (ev.key.code){
                        case Keyboard::Up:
                            menu.MoveUp();
                            break;

                        case Keyboard::W:
                            menu.MoveUp();
                            break;

                        case Keyboard::Down:
                            menu.MoveDown();
                            break;

                        case Keyboard::S:
                            menu.MoveDown();
                            break;
                        case Keyboard::Return:
                            switch (menu.GetPressedItem()){
                                case 0: //play
                                    while (!Keyboard::isKeyPressed(Keyboard::Escape)) {
                                        while (window.pollEvent(ev)) {
                                            switch (ev.type) {
                                                case Event::Closed:
                                                    window.close();
                                                    return 0;

                                                case Event::Resized:
                                                    resize_view(window, view);
                                                    break;
                                            }
                                        }
                                        player_test.update(delta_time, Walls);

                                        if(collision_detection(enemy_count, enemy, player_test)){
                                            Combat_menu combat_menu(window.getSize().x, window.getSize().y, player_test.get_position());
                                            music.stop();
                                            bool ucieczka = false;
                                            bool fight_ongoing = false;
                                            while (!ucieczka && !Keyboard::isKeyPressed(Keyboard::X)) {

                                                //tlo podczas walki
                                                Texture arena;
                                                arena.loadFromFile("../Grafika/arena.png");
                                                Sprite arena_sprite;
                                                arena_sprite.setTexture(arena);
                                                arena_sprite.setPosition(player_test.get_position().x,
                                                                         player_test.get_position().y);
                                                arena_sprite.setOrigin(1920 / 2, 1080 / 2);

                                                //sprite postaci walki:
                                                Texture my_body;
                                                my_body.loadFromFile("../walka_ja.png");
                                                Sprite my_body_sprite;
                                                my_body_sprite.setTexture(my_body);
                                                my_body_sprite.setPosition(player_test.get_position().x - 660,
                                                                           player_test.get_position().y - 120);
                                                my_body_sprite.setOrigin(358 / 2, 488 / 2);

                                                Texture op_body;
                                                op_body.loadFromFile("../Student_Zombie_girl.png");
                                                Sprite op_body_sprite;
                                                op_body_sprite.setTexture(op_body);
                                                op_body_sprite.setPosition(player_test.get_position().x + 660,
                                                                           player_test.get_position().y - 120);
                                                op_body_sprite.setOrigin(360 / 2, 360 / 2);

                                                //krew
                                                Texture blood;
                                                blood.loadFromFile("../blood.png");
                                                Sprite blood_sprite_op, blood_sprite_myself;
                                                blood_sprite_myself.setTexture(blood);
                                                blood_sprite_myself.setPosition(player_test.get_position().x - 660,
                                                                                player_test.get_position().y - 80);
                                                blood_sprite_myself.setOrigin(250 / 2, 300 / 2);

                                                blood_sprite_op.setTexture(blood);
                                                blood_sprite_op.setPosition(player_test.get_position().x + 660,
                                                                            player_test.get_position().y - 100);
                                                blood_sprite_op.setOrigin(250 / 2, 300 / 2);

                                                while (window.pollEvent(ev)) {
                                                    switch (ev.type) {
                                                        case Event::Closed:
                                                            window.close();
                                                            return 0;

                                                        case Event::Resized:
                                                            resize_view(window, view);
                                                            break;

                                                        case Event::KeyReleased:
                                                            switch (ev.key.code) {
                                                                case Keyboard::Left:
                                                                    combat_menu.MoveUp();
                                                                    break;
                                                                case Keyboard::A:
                                                                    combat_menu.MoveUp();
                                                                    break;
                                                                case Keyboard::Right:
                                                                    combat_menu.MoveDown();
                                                                    break;
                                                                case Keyboard::D:
                                                                    combat_menu.MoveDown();
                                                                    break;
                                                                case Keyboard::Return:
                                                                    switch (combat_menu.GetPressedItem()) {
                                                                        case 0: {//walka
                                                                            states = fight_func_logic(quantity_ststs,
                                                                                                      states,
                                                                                                      stats_after_item);
                                                                            fight_ongoing = true;

                                                                            //po uzyciu use item sie resetuje
                                                                            stats_after_item.at(0) = 0;
                                                                            stats_after_item.at(1) = 0;

                                                                            if (states.at(0) <= 0) {
                                                                                fight_ongoing = false;
                                                                                Text end_text;
                                                                                end_text.setFont(font);
                                                                                end_text.setFillColor(sf::Color::Red);
                                                                                end_text.setCharacterSize(60);
                                                                                end_text.setString("GAME OVER!");
                                                                                end_text.setPosition(
                                                                                        player_test.get_position().x -
                                                                                        180,
                                                                                        player_test.get_position().y -
                                                                                        60);
                                                                                window.draw(end_text);
                                                                                window.display();
                                                                                Sleep (2000);
                                                                                window.close();
                                                                                return 0;
                                                                            }
                                                                            if (states.at(2) <= 0) {
                                                                                fight_ongoing = false;
                                                                                Text win_text;
                                                                                win_text.setFont(font);
                                                                                win_text.setFillColor(
                                                                                        sf::Color::Yellow);
                                                                                win_text.setCharacterSize(60);
                                                                                win_text.setString("VICTORY!");
                                                                                win_text.setPosition(
                                                                                        player_test.get_position().x -
                                                                                        180,
                                                                                        player_test.get_position().y -
                                                                                        60);
                                                                                window.draw(win_text);
                                                                                window.display();
                                                                                Sleep (2000);
                                                                                ucieczka = true;
                                                                                //reset stats
                                                                                states.at(0) = 100;
                                                                                states.at(1) = 20;
                                                                                states.at(2) = 100;
                                                                                states.at(3) = 20;
                                                                            }
                                                                        }
                                                                            break;
                                                                        case 1://use item
                                                                            stats_after_item = use_item_func(
                                                                                    quantity_ststs);
                                                                            break;
                                                                        case 2: {//stats
                                                                            while (!Keyboard::isKeyPressed(
                                                                                    Keyboard::Escape)) {
                                                                                Text stats;
                                                                                stats_func(stats,
                                                                                           player_test.get_position(),
                                                                                           font, window, states);
                                                                            }
                                                                        }
                                                                            break;
                                                                        case 3:{//ucieczka
                                                                            int szansa_na_ucieczke = (rand() %  99) + 1;
                                                                            if(szansa_na_ucieczke<=45) ucieczka = true;
                                                                            else{
                                                                                Text ucieczka;
                                                                                ucieczka_func(ucieczka,
                                                                                              player_test.get_position(),
                                                                                              font, window);
                                                                                combat_menu.ucieczka(
                                                                                        player_test.get_position());
                                                                            }
                                                                        }
                                                                            break;
                                                                    }
                                                            }
                                                    }

                                                }
                                                window.clear();
                                                window.draw(arena_sprite);
                                                window.draw(my_body_sprite);
                                                window.draw(op_body_sprite);
                                                if (fight_ongoing) {
                                                    window.draw(blood_sprite_op);
                                                    window.draw(blood_sprite_myself);
                                                }
                                                combat_menu.draw(window);
                                                window.display();
                                            }
                                            music.play();

                                        }
                                        view.setCenter(player_test.get_position());
                                        if (music_play == false) {
                                            music.play();
                                            music_play = true;
                                        }
//                                        object_collision(player_test, Walls, sukiennice_text,
//                                                         player_test.get_body().getGlobalBounds(),
//                                                         player_test.get_walkspeed(), nextBox);
                                        if (player_test.get_body().getGlobalBounds().intersects(npc2.npcBounds())) {
                                            if (Keyboard::isKeyPressed(Keyboard::E)) {
                                                if(!dialog){
                                                    dialog=true;
                                                }
                                            }
                                        }
//                                        if (dialog == true) {
//                                            while (!Keyboard::isKeyPressed(Keyboard::E)) {
//                                            dialog_box(window, font, "Test siema", dialogbox,
//                                                       player_test.get_position());
//                                            }
//                                        }
                                        FloatRect testPos=player_test.get_body().getGlobalBounds();
                                        nextBox.setPosition(testPos.left, testPos.top);
                                        //rysowanie gracza, mapy i innych przydatnych rzeczy
                                        window.clear();
                                        window.setView(view);
                                        map.draw(window);
                                        window.draw(sukiennice);
                                        window.draw(npc2);
                                        window.draw(npc3);
                                        window.draw(npc4);
                                        window.draw(npc5);
                                        window.draw(npc6);
                                        for(int i=0; i<enemy_count; i++){
                                            window.draw(enemy.at(i));
                                        }
                                        window.draw(player_test);
                                        window.draw(nextBox);
                                        window.draw(uczelnia);
                                        window.draw(uczelnia_2);
                                        window.draw(uczelnia_3);
                                        if(dialog==true) {
                                            dialogbox.setPosition(player_test.get_position().x-450, player_test.get_position().y+200);
                                            window.draw(dialogbox);
                                        }
                                        //rysowanie collision boxów
                                        for (auto &i : Walls)
                                        {
                                            window.draw(i);
                                        }

                                        window.display();
                                    }

                                    view.setCenter(540.0f, 540.0f);
                                    window.setView(view);
                                    break;

                                case 1: { //opcje
                                    bool move_back = false;
                                    while (!move_back && !Keyboard::isKeyPressed(Keyboard::Escape)) {
                                        while (window.pollEvent(ev)) {
                                            switch (ev.type) {
                                                case Event::Closed:
                                                    window.close();
                                                    return 0;

                                                case Event::Resized:
                                                    resize_view(window, view);
                                                    break;

                                                case Event::KeyReleased:
                                                    switch (ev.key.code) {
                                                        case Keyboard::Up:
                                                            settings_menu.MoveUp();
                                                            break;
                                                        case Keyboard::W:
                                                            settings_menu.MoveUp();
                                                            break;
                                                        case Keyboard::Down:
                                                            settings_menu.MoveDown();
                                                            break;
                                                        case Keyboard::S:
                                                            settings_menu.MoveDown();
                                                            break;
                                                        case Keyboard::Return:
                                                            switch (settings_menu.GetPressedItem()) {
                                                                case 1:
                                                                    if (volume <= 95) {
                                                                        volume += 5;
                                                                        music.setVolume(volume);
                                                                        settings_menu.Change_sound_display(volume);
                                                                    }
                                                                    break;
                                                                case 2:
                                                                    if (volume >= 5) {
                                                                        volume -= 5;
                                                                        music.setVolume(volume);
                                                                        settings_menu.Change_sound_display(volume);
                                                                    }
                                                                    break;
                                                                case 3:
                                                                    if (ev.key.code == Keyboard::Return)
                                                                        move_back = true;
                                                                        break;
                                                            }
                                                    }
                                            }
                                        }
                                        window.clear();
                                        settings_menu.draw(window);
                                        window.display();
                                    }
                                }
                                    break;

                                case 2: //wyjscie
                                    window.close();
                                    break;

                            }
                            break;

                    }
                    break;

                case Event::Closed:
                    window.close();
                    return 0;

                case Event::Resized:
                    resize_view(window, view);
                    break;
            }
        }
        window.clear();
        menu.draw(window);
        window.display();
    }

    //system ("pause");
    return 0;
}
void resize_view(const RenderWindow &window, View &view){
    float proporcja = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(view_size.x * proporcja, view_size.x);
}

bool collision_detection(int enemy_count, vector<NPC> &enemy, Player &player_test){
    for(size_t i = 0; i<enemy_count; i++){
        const float radius_mistake = 32.0f;
        if(player_test.get_position().x <= (enemy[i].get_position().x+radius_mistake) && player_test.get_position().x >= (enemy[i].get_position().x-radius_mistake)
        && player_test.get_position().y <= (enemy[i].get_position().y+radius_mistake) && player_test.get_position().y >= (enemy[i].get_position().y-radius_mistake)){
            enemy.at(i).set_position();
            return true;
        }
    } return false;
}




void ucieczka_func(Text ucieczka, Vector2f player_test, Font font, RenderWindow& window){
    ucieczka.setFont(font);
    ucieczka.setFillColor(sf::Color::Magenta);
    ucieczka.setCharacterSize(50);
    ucieczka.setString("Nie ma ucieczki!");
    ucieczka.setPosition(player_test.x-240,player_test.y-60);
    window.draw(ucieczka);
    window.display();
    Sleep (2600);
}

void stats_func(Text stats, Vector2f player_test, Font font, RenderWindow& window, vector <int> states){
    Texture frame;
    frame.loadFromFile("../Grafika/frame.jpg");
    frame.setSmooth(true);

    Sprite frame_sprite;
    frame_sprite.setTexture(frame);
    frame_sprite.setPosition(player_test.x-400,player_test.y-300);

    String my_health_str, my_damage_str;
    String op_health_str, op_damage_str;

    my_health_str = to_string(states.at(0));
    my_damage_str = to_string(states.at(1));
    op_health_str = to_string(states.at(2));
    op_damage_str = to_string(states.at(3));

    Text student, my_health, my_damage;
    Text oponent, op_health, op_damage;

    student.setFont(font);
    student.setFillColor(sf::Color::Yellow);
    student.setCharacterSize(30);
    student.setString("STUDENT");
    student.setPosition(player_test.x-310,player_test.y-200);

    my_health.setFont(font);
    my_health.setFillColor(sf::Color::Red);
    my_health.setCharacterSize(30);
    my_health.setString("Życie : "+ my_health_str);
    my_health.setPosition(player_test.x-310,player_test.y-120);

    my_damage.setFont(font);
    my_damage.setFillColor(sf::Color::Red);
    my_damage.setCharacterSize(30);
    my_damage.setString("Atak : "+ my_damage_str);
    my_damage.setPosition(player_test.x-310,player_test.y-40);

    oponent.setFont(font);
    oponent.setFillColor(sf::Color::Yellow);
    oponent.setCharacterSize(30);
    oponent.setString("OPONENT");
    oponent.setPosition(player_test.x+100,player_test.y-200);

    op_health.setFont(font);
    op_health.setFillColor(sf::Color::Red);
    op_health.setCharacterSize(30);
    op_health.setString("Życie : "+ op_health_str);
    op_health.setPosition(player_test.x+100,player_test.y-120);

    op_damage.setFont(font);
    op_damage.setFillColor(sf::Color::Red);
    op_damage.setCharacterSize(30);
    op_damage.setString("Atak : "+ op_damage_str);
    op_damage.setPosition(player_test.x+100,player_test.y-40);

        window.draw(frame_sprite);
        window.draw(student);
        window.draw(my_health);
        window.draw(my_damage);
        window.draw(oponent);
        window.draw(op_health);
        window.draw(op_damage);
        window.display();
}
vector <int> fight_func_logic(int quantity_ststs, vector <int> states, vector <int> stats_after_item){
    vector <int> stats {states};
    int my_damage =( std::rand() % 20 ) + 10;
    int op_damage =( std::rand() % 20 ) + 10;

    int shield = stats_after_item.at(0);
    int sword = stats_after_item.at(1);

    //health student
    stats.at(0)=(stats.at(0)-op_damage)+shield;
    //damage student
    stats.at(1)=my_damage+sword;
    //health oponent
    stats.at(2)-=(my_damage+sword);
    //damage oponent
    stats.at(3)=op_damage;

    return stats;
}

vector <int> use_item_func(int quantity_ststs){
    vector <int> stats_after_item(quantity_ststs);
    int shield =( std::rand() % 20 ) + 5;
    int sword =( std::rand() % 10 ) + 5;

    //my health shield
    stats_after_item.at(0) = shield;
    //my damage sword
    stats_after_item.at(1) = sword;

    return stats_after_item;
}
//do poprawy
void dialog_box(RenderWindow& window, Font font, string text, Sprite dialog_box, Vector2f player_position){
    window.draw(dialog_box);
    Text dialog;
    dialog.setFont(font);
    dialog.setFillColor(sf::Color::Red);
    dialog.setCharacterSize(30);
    dialog.setPosition(player_position.x+100,player_position.y-40);
    String animation;
    for(int i;i<text.length();i++){
        animation+=text[i];
        dialog.setString(animation);
        Sleep(200);
        window.draw(dialog);
    }
}