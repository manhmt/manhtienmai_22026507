#include "Game.h"
//Game::Game() {}
Game::~Game()
{
    cleanup();
}
bool Game::initializeGame()
{
    if (!loadBackground())
    {
        return false;
    }

    player_power.Init(renderer);
    player_money.Init(renderer);
    player_money.SetPos(SCREEN_WIDTH * 0.5 - 250, 8);

    threats_list = MakeThreatList();

    time_game.SetColor(TextObject::WHITE_TEXT);
    mark_game.SetColor(TextObject::WHITE_TEXT);
    money_game.SetColor(TextObject::WHITE_TEXT);

    bool is = chasing_monster.LoadImg("img/chasing_monster.png", renderer);
    chasing_monster.set_clips();
    chasing_monster.SetSpeed(1);
    chasing_monster.GetRectFrame();
    chasing_monster.SetRect(0, 50);
    chasing_monster.Show(renderer);

    bool tRet = exp_threat.LoadImg("img/exp3.png", renderer);
    if (!tRet)
    {
        return false;
    }
    exp_threat.set_clip();
    return true;
}
void Game::runGameLoop() {
    GameState gameState = GameState::MENU;

    while (gameState != GameState::EXIT) {
        switch (gameState) {
        case GameState::MENU:
            gameState = showMenu();
            break;
        case GameState::PLAY:
            gameState = run();
            break;
        default:
            throw std::runtime_error("Invalid game state");
        }
    }
}
GameState Game::run()
{
    if (!initializeGame())
    {
        return GameState::EXIT;
    }
    while (!is_quit)
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        g_background.Render(renderer, NULL);
        game_map->DrawMap(renderer);

        handleEvents();
        handlePlayerActions();
        handleChasingMonster();
        handleThreats();


        handlePlayerBullets();
        handleUIUpdates();
        handleFrameRate();
        SDL_RenderPresent(renderer);
    }
    return GameState::EXIT;
}
void Game::initThreatsList() {
    threats_list = MakeThreatList();
}
bool Game::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Create window
    window = SDL_CreateWindow("Adventure",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    //screen = renderer;
    SDL_SetRenderDrawColor(renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    

    // Load the font
    font_time = TTF_OpenFont("font//dlxfont_.ttf", 35);
    if (font_time == NULL)
    {
        return false;
    }

    // Create the menu
    menu = new Menu(renderer);
    menu->addOption("Play");
    //menu->addOption("LEVELS");
    menu->addOption("Instruction");
    menu->addOption("Exit");

    // Create the game map
    game_map = new GameMap();
    game_map->LoadMap("map/map01.dat");
    game_map->LoadTiles(renderer);

    // Create the player
    p_player = new MainObject();
    p_player->LoadImg("img/player_right.png", renderer);
    p_player->set_clips();
    //p_player.SetRect(150, 50);
    return true;
}
bool Game::loadBackground()
{
    bool ret = g_background.LoadImg("img/images.png", renderer);
    if (!ret)
    {
        std::cerr << "Failed to load background image" << std::endl;
        return false;
    }
    return true;
}
GameState Game::handleGameState(GameState menuState, bool& is_quit)
{
    if (menuState == GameState::MENU)
    {
        GameState newState = showMenu();
        if (newState != GameState::MENU) {
            menuState = newState;
        }
        std::cout << "Menu state: " << static_cast<int>(menuState) << std::endl;
    }
    if (menuState == GameState::EXIT)
    {
        is_quit = true;
    }
    return menuState;
}
void Game::handleEvents()
{
    while (SDL_PollEvent(&g_event) != 0)
    {
        if (g_event.type == SDL_QUIT) {
            is_quit = true;
            break;
        }
        p_player->HandleInputAction(g_event, renderer);
    }
}
void Game::handlePlayerActions()
{
    map_data = game_map->getMap();

    p_player->HandleBullet(renderer);
    p_player->setMapXY(map_data.start_x_, map_data.start_y_);
    p_player->DoPlayer(map_data);
    p_player->Show(renderer);

    game_map->setMap(map_data);
    game_map->DrawMap(renderer);
}
void Game::handleUIUpdates() {
    player_power.Show(renderer);
    player_money.Show(renderer);

    // Show time game
    Uint32 time_val = SDL_GetTicks() / 1000;
    Uint32 val_time = 150 - time_val;
    if (val_time <= 0) {
        if (MessageBox(NULL, L"Run out of Time !!!", L"Info", MB_OK | MB_ICONSTOP) == IDOK) {
            is_quit = true;
        }
    }
    else {
        std::string str_time = "Time: ";
        std::string str_val = std::to_string(val_time);
        str_time += str_val;

        time_game.SetText(str_time);
        time_game.LoadFromRenderText(font_time, renderer);
        time_game.RenderText(renderer, SCREEN_WIDTH - 200, 15);
    }

    // Show score
    std::string val_str_mark = std::to_string(mark_value);
    std::string strMark("Mark: ");
    strMark += val_str_mark;

    mark_game.SetText(strMark);
    mark_game.LoadFromRenderText(font_time, renderer);
    mark_game.RenderText(renderer, SCREEN_WIDTH * 0.5, 15);

    // Show money count
    int money_count = p_player->GetMoneyCount();
    std::string money_str = std::to_string(money_count);

    money_game.SetText(money_str);
    money_game.LoadFromRenderText(font_time, renderer);
    money_game.RenderText(renderer, SCREEN_WIDTH * 0.5 - 200, 15);
}
void Game::handleFrameRate()
{
    int real_imp_time = fps_timer.get_ticks();
    int time_one_frame = 750 / FRAME_PER_SECOND;

    if (real_imp_time < time_one_frame)
    {
        int delay_time = time_one_frame - real_imp_time;
        if (delay_time >= 0)
        {
            SDL_Delay(delay_time);
        }
    }
}
void Game::renderGame(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}
void Game::handleChasingMonster() {
    // Move and render the chasing monster
    chasing_monster.Move(*p_player, *game_map);
    chasing_monster.Show(renderer);

    // Collision check between the chasing monster and the main character
    SDL_Rect rect_player = p_player->GetRectFrame();
    SDL_Rect rect_chasing_monster = chasing_monster.GetRectFrame();
    bool is_collision = SDLCommonFunc::CheckCollision(rect_player, rect_chasing_monster);

    if (is_collision) {
        std::cout << "Collide with dragon" << std::endl;
        handleCollision();
    }
}
void Game::handleThreats() {
    bool bulletCollisionDetected = false;
    for (auto it = threats_list.begin(); it != threats_list.end(); ) {
        ThreatObject* p_threat = *it;
        if (p_threat != NULL) {
            p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
            p_threat->ImpMoveType(renderer);
            p_threat->DoPlayer(map_data);
            p_threat->MakeBullet(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, p_player->GetRect().x, p_player->GetRect().y);
            p_threat->Show(renderer);

            SDL_Rect rect_player = p_player->GetRectFrame();

            // Check collision between player and threat bullet
            std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
            for (int ij = tBullet_list.size() - 1; ij >= 0; --ij) {
                BulletObject* pt_bullet = tBullet_list.at(ij);
                if (pt_bullet) {
                    bool bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                    if (bCol1) {
                        std::cout << "player hit bullet";
                        handleCollision();
                        p_threat->RemoveBullet(ij);
                        bulletCollisionDetected = true;
                        break;
                    }
                }
            }

            // If a bullet collision has been detected, break from the outer loop
            if (bulletCollisionDetected) {
                break;
            }

            // Check collision between player and threat
            SDL_Rect rect_threat = p_threat->GetRectFrame();
            bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
            if (bCol2) {
                std::cout << "player and threat";
                handleCollision();
                it = threats_list.erase(it);
            }

            else {
                ++it;
            }
        }
    }
}


void Game::handleCollision()
{
    num_die++;
    if (num_die < 3)
    {
        player_power.Decrease();
        player_power.Render(renderer);
        //p_player->SetRect(0, 0);
        //p_player->set_comeback_time(40);
        SDL_Delay(1000);
    }
    else
    {
        if (MessageBox(NULL, L"Game Over...", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
        {
            close();
            SDL_Quit();
            exit(EXIT_SUCCESS);
        }
    }
}
void Game::handlePlayerBullets() {
    int frame_exp_width = exp_threat.get_frame_width();
    int frame_exp_height = exp_threat.get_frame_height();

    std::vector<BulletObject*> bullet_arr = p_player->get_bullet_list_();
    for (int r = 0; r < bullet_arr.size(); ++r) {
        BulletObject* p_bullet = bullet_arr.at(r);
        if (p_bullet) {
            for (auto it = threats_list.begin(); it != threats_list.end(); ) {
                ThreatObject* obj_threat = *it;
                if (obj_threat) {
                    SDL_Rect tRect;
                    tRect.x = obj_threat->GetRect().x;
                    tRect.y = obj_threat->GetRect().y;
                    tRect.w = obj_threat->get_width_frame();
                    tRect.h = obj_threat->get_height_frame();
                    SDL_Rect bRect = p_bullet->GetRect();
                    bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
                    if (bCol) {
                        mark_value++;
                        for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                        {
                            int x_pos_ = p_bullet->GetRect().x - frame_exp_width * 0.5;
                            int y_pos_ = p_bullet->GetRect().y - frame_exp_height * 0.5;

                            exp_threat.set_frame(ex);
                            exp_threat.SetRect(x_pos_, y_pos_);
                            exp_threat.Show(renderer);
                        }
                        p_player->RemoveBullet(r);
                        obj_threat->Free();
                        it = threats_list.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
        }
    }
}


void Game::cleanup() {
    // Cleanup code here

    if (menu != nullptr) {
        delete menu;
        menu = nullptr;
    }

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    if (game_map != nullptr) {
        delete game_map;
        game_map = nullptr;
    }

    if (p_player != nullptr) {
        delete p_player;
        p_player = nullptr;
    }

    if (font_time != nullptr)
    {
        TTF_CloseFont(font_time);
        font_time = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
void Game::close()
{
    g_background.Free();

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}
ThreatObject* Game::makeThreat(std::string image_path, int x_pos, int y_pos, ThreatObject::TypeMove move_type, int input_left, bool init_bullet) {
    ThreatObject* p_threat = new ThreatObject();
    if (p_threat != nullptr)
    {
        p_threat->LoadImg(image_path, renderer);
        p_threat->set_clips();
        p_threat->set_x_pos(x_pos);
        p_threat->set_y_pos(y_pos);
        p_threat->set_type_move(move_type);
        p_threat->set_input_left(input_left);

        if (init_bullet) {
            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, renderer);
        }
    }
    return p_threat;
}
std::vector<ThreatObject*> Game::MakeThreatList()
{
    std::vector<ThreatObject*> list_threats;

    for (int i = 0; i < 20 * difficulty_level; i++)
    {
        list_threats.push_back(makeThreat("img/threat_left.png", 600 + i * 700, 50, ThreatObject::MOVE_IN_SPACE_THREAT, 1, true));
    }

    for (int i = 0; i < 20 * difficulty_level; i++)
    {
        list_threats.push_back(makeThreat("img/threat_level.png", 1000 + i * 1000, 250, ThreatObject::STATIC_THREAT, 0, true));
    }
    for (int i = 0; i < 10 * difficulty_level; i++)
    {
        list_threats.push_back(makeThreat("img/spider.png", 300 + i * 900, 250, ThreatObject::MOVE_SPIDER, 1, false));
    }
    return list_threats;
}

/*
std::vector<ThreatObject*> Game::MakeThreatList()
{

     /*for (int i = 0; i < 20; i++)
    {
        FlyingEnemy* flying_enemy = new FlyingEnemy();
        flying_enemy->LoadImg("img/plane.png", renderer);
        flying_enemy->set_clips();
        flying_enemy->set_x_pos(40 + i * 500);
        flying_enemy->set_y_pos(250);
        flying_enemy->set_type_move(ThreatObject::STATIC_THREAT);
        flying_enemy->set_input_left(0);

        list_threats.push_back(flying_enemy);
    }
    

    return list_threats;
}
*/
GameState Game::showMenu()
{
    bool menu_active = true;
    int selectedOption = 0;
    bool backgroundLoaded = loadBackground();
    if (!backgroundLoaded) {
        std::cout << "background is not loaded";
    }
    bool menuRendered = false;
    while (menu_active)
    {
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                menu_active = false;
                return GameState::EXIT;
            }
            else if (g_event.type == SDL_KEYDOWN)
            {
                switch (g_event.key.keysym.sym) {
                case SDLK_UP:
                    menu->moveUp();
                    break;
                case SDLK_DOWN:
                    menu->moveDown();
                    break;
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    selectedOption = menu->getSelectedOption();
                    switch (selectedOption) {
                    case START_GAME:
                        menu_active = false;
                        std::cout << "Play button clicked" << std::endl;
                        initThreatsList();
                        return GameState::PLAY; 
                    case INSTRUCTION:
                        // Show instruction screen
                        break;
                    case QUIT:
                        menu_active = false;
                        return GameState::EXIT;
                    }
                    break;
                default:
                    break;
                }
            }
            else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                selectedOption = menu->processMouseClick(x, y, font_time, renderer);
                if (selectedOption >= 0) {
                    switch (selectedOption) {
                    case START_GAME:
                        menu_active = false;
                        std::cout << "Switching to PLAY state" << std::endl;
                        initThreatsList();

                        return GameState::PLAY;
                    case INSTRUCTION:
                        // Show instruction screen
                        break;
                    case QUIT:
                        menu_active = false;
                        return GameState::EXIT;
                    }
                }
            }
        }

        if (!menuRendered)
        {
            // Render the background image

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            g_background.Render(renderer, NULL);
            menu->renderMenu();
            SDL_RenderPresent(renderer);
            menuRendered = true;
        }
    }

    return GameState::EXIT;
}