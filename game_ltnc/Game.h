#ifndef GAME_H_
#define GAME_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ThreatObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "ChasingMonster.h"
#include "GameMap.h"
#include "ImpTimer.h"
#include "Menu.h"
#include "FlyingEnemy.h"

enum GameState {
    MENU,
    PLAY,
    EXIT
};

class Game {
public:
    Game() : window(nullptr),
        renderer(nullptr),
        menu(nullptr),
        game_map(nullptr),
        p_player(nullptr),
        num_die(0),
        mark_value(0),
        is_quit(false),
        font_time(nullptr),
        difficulty_level(1)

    {
        // other initializations
    }
    ~Game();

    void initThreatsList();
    GameState showMenu();
    GameState run();
    bool initializeGame();
    GameState handleGameState(GameState menuState, bool& is_quit);
    void handleEvents();
    void handlePlayerActions();
    void handleUIUpdates();
    void handleFrameRate();
    void handleChasingMonster();
    void handleThreats();
    void handleCollision();
    void handlePlayerBullets();
    void renderGame(SDL_Renderer* renderer);

    ThreatObject* makeThreat(std::string image_path, int x_pos, int y_pos, ThreatObject::TypeMove move_type, int input_left, bool init_bullet);

    bool init();
    void cleanup();
    void runGameLoop(); 

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    Menu* menu;

    GameMap* game_map;
    MainObject* p_player;
    BaseObject g_background;

    TTF_Font* font_time;


    Map map_data;

    bool loadBackground();
    void close();
    std::vector<ThreatObject*> MakeThreatList();
    std::vector<ThreatObject*> threats_list;

    TextObject time_game;
    TextObject mark_game;
    TextObject money_game;
    ImpTimer fps_timer;
    ImpTimer chasing_monster_timer;

    PlayerPower player_power;
    PlayerMoney player_money;
    ChasingMonster chasing_monster;
    ExplosionObject exp_threat;

    bool is_quit;
    int num_die;
    UINT mark_value;
    int difficulty_level;

};

#endif // GAME_H