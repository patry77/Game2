/*
 * Mateusz Lugowski
 * Patryk Kucek
 * Oskar Maciaszek
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
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

Vector2u view_size(1000, 1000);
bool music_play=false;
void resize_view(const RenderWindow &window, View &view);
bool collision_detection(int enemy_count, vector<NPC> &enemy, Player &player_test);
void object_collision(Player &player_test, vector<RectangleShape> &Walls, Texture object);
void ucieczka_func(Text ucieczka, Vector2f player_test, Font font, RenderWindow& window);
void stats_func(Text stats, Vector2f player_test, Font font, RenderWindow& window, vector <int> states);
vector <int> fight_func_logic(int quantity_ststs, vector <int> states, vector <int> stats_after_item);
void fight_func_draw(Vector2f player_test, Font font, RenderWindow& window);
vector <int> use_item_func(int quantity_ststs);

int main() {
    //Muzyka
    float volume = 10.0f;
    sf::Music music;
    music.setVolume(volume);
    if (!music.openFromFile("../muzyka.wav"))
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
    player_texture.loadFromFile("../body_move.png");
    Player player_test(&player_texture, Vector2u(4,4), 0.3f);

    //NPC
    Texture NPC_texture;
    NPC_texture.loadFromFile("../body_move.png");
    NPC npc2(&NPC_texture,2800.0f,2800.0f);

    //Enemies
    Texture Enemy_texture;
    Enemy_texture.loadFromFile("../npc1.png");
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
    sukiennice_text.loadFromFile("../Sukiennice.png");
    Sprite sukiennice;
    sukiennice.setTexture(sukiennice_text);
    sukiennice.setPosition(64*20, 64*20);

    //granice
    std::vector<RectangleShape> Walls;

    RectangleShape wall;
    wall.setFillColor(Color::Transparent);
    wall.setSize(Vector2f(sukiennice_text.getSize().x,sukiennice_text.getSize().y));
    wall.setOrigin(wall.getSize().x/2, wall.getSize().y/2);
    wall.setPosition(64*20+(sukiennice_text.getSize().x/2), 64*20+(sukiennice_text.getSize().y/2));


    Walls.push_back(wall);

    //Kolizje
    FloatRect nextPos;

    //odswiezanie do animacji
    float delta_time = 0.0f;
    Clock clock;

    while(window.isOpen()){
        delta_time = clock.restart().asSeconds();
        Event ev{};
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
                                        player_test.update(delta_time);

                                        if(collision_detection(enemy_count, enemy, player_test)){
                                            Combat_menu combat_menu(window.getSize().x, window.getSize().y, player_test.get_position());
                                            bool ucieczka = false;
                                            while (!ucieczka && !Keyboard::isKeyPressed(Keyboard::X)) {

                                                //tlo podczas walki
                                                Texture arena;
                                                arena.loadFromFile("../arena.png");
                                                Sprite arena_sprite;
                                                arena_sprite.setTexture(arena);
                                                arena_sprite.setPosition(player_test.get_position().x,player_test.get_position().y);
                                                arena_sprite.setOrigin(1920/2,1080/2);

                                                //sprite walki:

                                                //update bedzie po nacisnieciu przyciku fight, dodac sprite do funkcji na dole maina



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
                                                                        case 0:{//walka
                                                                            states = fight_func_logic(quantity_ststs, states, stats_after_item);
                                                                            fight_func_draw(player_test.get_position(), font , window);

                                                                            //po uzyciu use item sie resetuje
                                                                            stats_after_item.at(0)=0;
                                                                            stats_after_item.at(1)=0;

                                                                            if(states.at(0)<=0){
                                                                                Text end_text;
                                                                                end_text.setFont(font);
                                                                                end_text.setFillColor(sf::Color::Red);
                                                                                end_text.setCharacterSize(60);
                                                                                end_text.setString("GAME OVER!");
                                                                                end_text.setPosition(player_test.get_position().x-180,player_test.get_position().y-60);
                                                                                window.draw(end_text);
                                                                                window.display();
                                                                                music.stop();
                                                                                Sleep (2000);
                                                                                window.close();
                                                                                return 0;
                                                                            }
                                                                            if(states.at(2)<=0){
                                                                                Text win_text;
                                                                                win_text.setFont(font);
                                                                                win_text.setFillColor(sf::Color::Yellow);
                                                                                win_text.setCharacterSize(60);
                                                                                win_text.setString("VICTORY!");
                                                                                win_text.setPosition(player_test.get_position().x-180,player_test.get_position().y-60);
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
                                                                            stats_after_item = use_item_func(quantity_ststs);
                                                                            break;
                                                                        case 2:{//stats
                                                                            while(!Keyboard::isKeyPressed(Keyboard::Escape)){
                                                                                Text stats;
                                                                                stats_func(stats, player_test.get_position(), font , window, states);
                                                                            }
                                                                        }
                                                                            break;
                                                                        case 3:{//ucieczka
                                                                            int szansa_na_ucieczke = (rand() %  99) + 1;
                                                                            if(szansa_na_ucieczke<=45) ucieczka = true;
                                                                            else{
                                                                                Text ucieczka;
                                                                                ucieczka_func(ucieczka, player_test.get_position(), font , window);
                                                                                combat_menu.ucieczka(player_test.get_position());
                                                                            }
                                                                        }
                                                                            break;
                                                                    }
                                                            }
                                                    }

                                                }
                                                window.clear();
                                                window.draw(arena_sprite);
                                                combat_menu.draw(window);
                                                window.display();
                                            }

                                        }
                                        view.setCenter(player_test.get_position());
                                        if(music_play==false){
                                            music.play();
                                            music_play=true;
                                        }
                                        object_collision(player_test, Walls, sukiennice_text);
                                        //rysowanie gracza, mapy i innych przydatnych rzeczy
                                        window.clear();
                                        window.setView(view);
                                        map.draw(window);
                                        window.draw(sukiennice);
                                        window.draw(npc2);
                                        for(int i=0; i<enemy_count; i++){
                                            window.draw(enemy.at(i));
                                        }
                                        window.draw(player_test);
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

void object_collision(Player &player_test, vector<RectangleShape> &Walls, Texture sukiennice_text){
//    for(auto &wall : Walls)
//    {
//        FloatRect playerBounds = player_test.get_body().getGlobalBounds();
//        FloatRect wallBounds = wall.getGlobalBounds();
//        nextPos = playerBounds;
//        nextPos.left += 600.0f;//+walk speed ręcznie bo jest prywatny
//        nextPos.top += 600.0f;
//
//        if (wallBounds.intersects(nextPos))
//        {
//            std::cout << "Collision" << endl;
//        }
//    }

//    for(auto &wall : Walls) {
//        if (player_test.get_position().x <= (wall.getPosition().x+sukiennice_text.getSize().x/2) && player_test.get_position().x >= (wall.getPosition().x-sukiennice_text.getSize().x/2)
//           && player_test.get_position().y <= (wall.getPosition().y+sukiennice_text.getSize().y/2) && player_test.get_position().y >= (wall.getPosition().y-sukiennice_text.getSize().y/2))
//        {
//            //kolizja lewo
//            if(player_test.get_position().x < wall.getPosition().x+sukiennice_text.getSize().x/2)
//                player_test.set_position(wall.getPosition().x+sukiennice_text.getSize().x/2, player_test.get_position().y);
//            //kolizja gora
//            if(player_test.get_position().y < wall.getPosition().y+sukiennice_text.getSize().y/2)
//                player_test.set_position(player_test.get_body().getPosition().x, wall.getPosition().y+sukiennice_text.getSize().y/2);
//            //kolizja prawo
//            if(player_test.get_position().x + player_test.get_body().getGlobalBounds().width > (sukiennice_text.getSize().x))
//                player_test.set_position((sukiennice_text.getSize().x)-player_test.get_body().getGlobalBounds().width, player_test.get_position().y);
//            //kolizja dol
//            if(player_test.get_position().y + player_test.get_body().getGlobalBounds().height > (sukiennice_text.getSize().x))
//                player_test.set_position(player_test.get_body().getPosition().x, (sukiennice_text.getSize().x)-player_test.get_body().getGlobalBounds().height);
//        }
//    }

}


void ucieczka_func(Text ucieczka, Vector2f player_test, Font font, RenderWindow& window){
    ucieczka.setFont(font);
    ucieczka.setFillColor(sf::Color::Magenta);
    ucieczka.setCharacterSize(50);
    ucieczka.setString("There is no Escape!");
    ucieczka.setPosition(player_test.x-240,player_test.y-60);
    window.draw(ucieczka);
    window.display();
    Sleep (2600);
}

void stats_func(Text stats, Vector2f player_test, Font font, RenderWindow& window, vector <int> states){
    Texture frame;
    frame.loadFromFile("../frame.jpg");
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
    my_health.setString("Health : "+ my_health_str);
    my_health.setPosition(player_test.x-310,player_test.y-120);

    my_damage.setFont(font);
    my_damage.setFillColor(sf::Color::Red);
    my_damage.setCharacterSize(30);
    my_damage.setString("Damage : "+ my_damage_str);
    my_damage.setPosition(player_test.x-310,player_test.y-40);

    oponent.setFont(font);
    oponent.setFillColor(sf::Color::Yellow);
    oponent.setCharacterSize(30);
    oponent.setString("OPONENT");
    oponent.setPosition(player_test.x+100,player_test.y-200);

    op_health.setFont(font);
    op_health.setFillColor(sf::Color::Red);
    op_health.setCharacterSize(30);
    op_health.setString("Health : "+ op_health_str);
    op_health.setPosition(player_test.x+100,player_test.y-120);

    op_damage.setFont(font);
    op_damage.setFillColor(sf::Color::Red);
    op_damage.setCharacterSize(30);
    op_damage.setString("Damage : "+ op_damage_str);
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

void fight_func_draw(Vector2f player_test, Font font, RenderWindow& window){

}