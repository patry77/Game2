/*
 * Mateusz Lugowski
 * Patryk Kucek
 * Oskar Maciaszek
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Menu.h"
#include "NPC.h"
#include "UI.h"
#include "Pause.h"
#include "BattleSystem.h"


using namespace sf;
using namespace std;

Vector2u view_size(1000, 1000);
bool music_play=false;
void resize_view(const RenderWindow &window, View &view);
void drawUI(sf::RenderWindow& window, sf::View playerView, std::vector<UI*>& sysWindows, float elapsedTime);
bool UI_visible(std::vector<UI*>& sysWindows);
bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows);

int main() {
    //Muzyka
    float volume = 20.0f;
    sf::Music music;
    if (!music.openFromFile("../muzyka.wav"))
        return -1; // error

    //Dzwieki
    sf::Sound sfx_blip1;
    sf::Sound sfx_blip2;

    sf::SoundBuffer bleep1;
    if (!bleep1.loadFromFile("../text_blip.wav"))
        return -1; // error

    sf::SoundBuffer bleep2;
    if (!bleep2.loadFromFile("../text_blip2.wav"))
        return -1; // error

    sfx_blip1.setBuffer(bleep1);
    sfx_blip2.setBuffer(bleep2);

    //tekstura
    sf::Texture titleTexture;
    if (!titleTexture.loadFromFile("../cursor.png")) {
        cerr << "Blad tekstury" << endl;
    }

    //Czcionka
    sf::Font sysFont;
    if (!sysFont.loadFromFile("../alagard.ttf")) {
        cerr << "błąd" << endl;
    }
    RenderWindow window {VideoMode(view_size.x, view_size.y),"Game"};
    window.setFramerateLimit(60);

    //UI
    std::vector<UI*> sysWindows;

    Pause* pausePtr = new Pause(sysFont, view_size);
    BattleSystem* battlePtr = new BattleSystem(sysFont, view_size, titleTexture, sfx_blip1);
    sysWindows.push_back(battlePtr);
    sysWindows.push_back(pausePtr);

    //UI input
    map<int, bool> ui_kb;
    ui_kb[sf::Keyboard::Return] = false;
    ui_kb[sf::Keyboard::Left] = false;
    ui_kb[sf::Keyboard::Right] = false;
    ui_kb[sf::Keyboard::Escape] = false;
    ui_kb[sf::Keyboard::Up] = false;
    ui_kb[sf::Keyboard::Down] = false;

    //Menu
    Menu menu(window.getSize().x, window.getSize().y);

    //Gracz
    Texture player_texture;
    player_texture.loadFromFile("../body_move.png");
    Player player_test(&player_texture, Vector2u(4,4), 0.3f);

    //NPC
    Texture NPC_texture;
    NPC_texture.loadFromFile("../npc1.png");
    NPC npc2(&NPC_texture,550.0f,550.0f);

    //Enemies
    Texture Enemy_texture;
    Enemy_texture.loadFromFile("../npc1.png");
    int enemy_count = 10;
    vector<NPC> enemy;
    srand( time( NULL ) );
    for(int i =0; i<enemy_count; i++){
        float pos_x = ( rand() % 1921 );
        float pos_y = ( rand() % 1081 );

        NPC temp_enemy {&Enemy_texture, pos_x, pos_y};
        enemy.push_back(temp_enemy);
    }

    //kamera - widok, zeby dobrze scalowal‚ obiekty przy zmianie rozmiaru okna i kamera przesuwala sie z postacia
    View view(FloatRect (0.0f, 0.0f, view_size.x, view_size.y));

    //mapa
    Map map;

    //odswiezanie do animacji
    float delta_time = 0.0f;
    Clock clock;

    while(window.isOpen()){
        delta_time = clock.restart().asSeconds();
        Event ev{};
        if (battlePtr->isVisible() && !UI_visible_excluding(battlePtr, sysWindows)) {
            if (ui_kb[ev.key.code])
                battlePtr->change_selection(ev.key.code);
            if (ui_kb[sf::Keyboard::Return]) {
                switch (battlePtr->getSelection()) {
                    case(Battle::Fight) :
                        break;
                    case(Battle::Items) :
                        std::cout << "Nie dziala jeszcze" << std::endl;
                        break;
                    case(Battle::Status) :
                        std::cout << "Nie dziala jeszcze" << std::endl;
                        break;
                    case(Battle::Escape) :
                        battlePtr->setVisible(false);
                        break;
                }
            }
        }
        if (ui_kb[sf::Keyboard::Escape]) {
            if (!UI_visible_excluding(pausePtr, sysWindows)) {
                pausePtr->setVisible(!pausePtr->isVisible());
                if (pausePtr->isVisible())
                    music.pause();
                else
                    music.play();
            }
        }
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
                        case Keyboard::M:
//                            battlePtr->draw()
                        case Keyboard::Return:
                            switch (menu.GetPressedItem()){
                                case 0: //play
                                    while (!Keyboard::isKeyPressed(Keyboard::Escape)) {
                                        while (window.pollEvent(ev)) {
                                            switch (ev.type) {
                                                case Event::Closed:
                                                    window.close();
                                                    break;

                                                case Event::Resized:
                                                    resize_view(window, view);
                                                    break;
                                            }
                                        }
                                        player_test.update(delta_time);
                                        view.setCenter(player_test.get_position());
                                        if(music_play==false){
                                            music.play();
                                            music_play=true;
                                        }
                                        window.clear();
                                        window.setView(view);
                                        map.draw(window);
                                        window.draw(npc2);
                                        for(int i=0; i<enemy_count; i++){
                                            window.draw(enemy.at(i));
                                        }
                                        window.draw(player_test);
                                        drawUI(window, view, sysWindows, delta_time);
                                        window.display();
                                    }
                                    view.setCenter(540.0f, 540.0f);
                                    window.setView(view);
                                    break;

                                case 1: //opcje
                                    music.setVolume(volume);
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
                    break;

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

void drawUI(sf::RenderWindow& window, sf::View playerView, std::vector<UI*>& sysWindows, float elapsedTime) {
    for (auto sys = sysWindows.begin(); sys != sysWindows.end(); sys++) {
        if ((*sys)->isVisible()) {
            (*sys)->update(playerView.getCenter(), elapsedTime);
            window.draw(**sys);
        }
    }
}
bool UI_visible(std::vector<UI*>& sysWindows) {
    for (auto sys = sysWindows.begin(); sys != sysWindows.end(); sys++) {
        if ((*sys)->isVisible()) {
            return true;
        }
    }
    return false;
}

bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows) {
    sysWindows.erase(std::find(sysWindows.begin(), sysWindows.end(), sysWindow));
    return UI_visible(sysWindows);
}